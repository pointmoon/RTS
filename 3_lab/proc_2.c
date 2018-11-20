#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(){
    int shm_fd;
    void *vaddr;

    //получаем дескриптор общей памяти
    if((shm_fd=shm_open("my_shm", O_RDWR, 0666)) == -1){
        perror("cannot open (shm_open)");
        return -1;
    }

    //подлючаем общую память в адресное пространство процесса
    if((vaddr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("can`t connect shm (mmap)");
        return -1;
    }

    while(1){
        //vaddr = "";
        char temp_str[SHM_SIZE];
        printf("enter string: ");
        fgets(temp_str, SHM_SIZE, stdin);
        //printf("%s",temp_str);
        sprintf((char*)vaddr, "%s", temp_str);
        sleep(16);
        if(strcmp((char*)vaddr, temp_str) ==0){
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
        printf("count symbols in string: %s\n", (char*)vaddr);
    }
    return -1;
}