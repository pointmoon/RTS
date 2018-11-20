#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(){
    int shm_fd;
    void *vaddr;

    //получаем дескриптор общей памяти
    if((shm_fd=shm_open("my_shm", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open (shm_open)");
        return -1;
    }

    //устанавливаем размер общей памяти
    if(ftruncate(shm_fd, SHM_SIZE) != 0){
        perror("can`t set size (ftruncate");
        return -1;
    }

    //подлючаем общую память в адресное пространство процесса
    if((vaddr = mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("can`t connect shm (mmap)");
        return -1;
    }

    //блокируем общую память
    if(mlock(vaddr, SHM_SIZE) != 0){
        perror("can`t block shm (mlock)");
        return -1;
    }
    printf("Expected string:\n");
    printf("%s\n", (char*)vaddr);
    printf("End of string\n");
    
    pause();
    //отделяем общую память от адресного пространства процесса
    munmap(vaddr, SHM_SIZE);
    close(shm_fd);
    //удаляем сегмент общей памяти
    shm_unlink("my_shm");
    return 0;
}