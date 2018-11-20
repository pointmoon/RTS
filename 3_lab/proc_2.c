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
    if((shm_fd=shm_open("my_shm", O_RDONLY, 0666)) == -1){
        perror("cannot open (shm_open)");
        return -1;
    }

    //подлючаем общую память в адресное пространство процесса
    if((vaddr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("can`t connect shm (mmap)");
        return -1;
    }

    while(1){
        
        char* temp_str;
        scanf("%s", temp_str);
        sprintf(vaddr, "%s", temp_str);
        sleep(16);
    }
    
    //отделяем общую память от адресного пространства процесса
    munmap(vaddr, SHM_SIZE);
    close(shm_fd);
    //удаляем сегмент общей памяти
    if(shm_unlink("my_shm") == -1){
        perror("can`t remove shm (unlink)");
        return -1;
    }
    return 0;
}