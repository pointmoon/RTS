#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1

#define RESET   "\033[0m"
#define YELLOW  "\033[1;33m"
#define GREEN  "\033[0;32m"

#define Y 3
#define P 16
#define R 1
#define F 18
#define A 17
#define B 15
#define J 3
#define S 10

// 9-6-1-2-3-7-5
// 6-1-2-3-7-5
// 1-2-3-7-5
// 2-3-7-5
// 3-7-5
// 7-5
// 5

void* thread_func(void* p);
void* arrThreadFunc_1(void* COLOR);
void* arrThreadFunc_2();


int main(){
    pthread_t thread;
    int status;
    status = pthread_create(&thread, NULL, thread_func, (void*)pthread_self());
    if(status != 0){
        printf("main error: pthread_create, status = %d\n", status);
        exit(ERROR);
    }
    //end task 9

    struct sched_param state_sched;
    int tmp_policy;

    pthread_getschedparam(pthread_self(), &tmp_policy, &state_sched);
    printf("OLD MAIN THREAD PRIO = %d (POLICY %d)\n", state_sched.sched_priority, tmp_policy);
    state_sched.sched_priority = (int)S;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &state_sched);
    pthread_getschedparam(pthread_self(), &tmp_policy, &state_sched);
    printf("NEW MAIN THREAD PRIO = %d (POLICY %d)\n", state_sched.sched_priority, tmp_policy);
    //end task 6

    pthread_t arrThreads_1[Y];
    int i;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy (&attr,SCHED_FIFO);
    state_sched.sched_priority = (int)P;
    pthread_attr_setschedparam(&attr, &state_sched);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED); // не наследовать атрибуты

    for(i=0; i<Y; i++){
        status = pthread_create(&arrThreads_1[i], &attr, arrThreadFunc_1, (void*)i+1);
        if(status != 0){
            printf("main error: pthread_create, status = %d\n", status);
            exit(ERROR);
        }
    }
    //end task 1

    pthread_t arrThreads_2[Y];
    state_sched.sched_priority = (int)F;
    pthread_attr_setschedparam(&attr, &state_sched);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    for(i=0; i<R; i++){
        status = pthread_create(&arrThreads_2[i], &attr, arrThreadFunc_2, NULL);
        if(status != 0){
            printf("main error: pthread_create, status = %d\n", status);
            exit(ERROR);
        }
    }
    // end task 2

    status = pthread_join(arrThreads_1[Y-1], NULL);
    if(status != 0){
        printf("thread error: pthread_join, status = %d\n", status);
        exit(ERROR);
    }
    //end task 7

    for(i=0; i<R; i++){
        status = pthread_join(arrThreads_2[i], NULL);
        if(status != 0){
            printf("thread error: pthread_join, status = %d\n", status);
            exit(ERROR);
        }
    }
    //end task 5

    int g = 345;
    printf("Hello from main thread!\n");
    pthread_exit((void*)g);
return 0;
}


void* thread_func(void* p){
    int status;
    pthread_t idMainThread = (pthread_t)p;
    status = pthread_join(idMainThread, NULL);
    if(status != 0){
        printf("thread error: pthread_join, status = %d\n", status);
        exit(ERROR);
    }
    printf("Good Buy!\n");
    pthread_exit(0);
}

void* arrThreadFunc_1(void* COLOR){
    int tmp_policy, i, old_prio;
    struct sched_param state_sched;

    pthread_getschedparam(pthread_self(), &tmp_policy, &state_sched);
    printf("\033[7;3%dmTHREAD CREATE ID = %d PRIO = %d (POLICY %d)%s\n", (int)COLOR, (int)pthread_self(), state_sched.sched_priority, tmp_policy, RESET);
    sleep(5);
    old_prio = state_sched.sched_priority;
    //start task 3
    state_sched.sched_priority = (int)A;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &state_sched);
    pthread_getschedparam(pthread_self(), &tmp_policy, &state_sched);
    printf("\033[5;3%dmTHREAD %d CHANGE PRIO FROM %d TO %d (POLICY %d)%s\n", (int)COLOR, (int)pthread_self(), old_prio, state_sched.sched_priority, tmp_policy, RESET);
    sleep(5);
    for(i=0; i<B; i++){
        printf("\033[1;3%dmTHREAD %d MY PRIO %d%s\n", (int)COLOR, (int)pthread_self(), state_sched.sched_priority, RESET);
        if(i%J == 0){
            sleep(5);
        }
    }
    //end task 3

    pthread_exit((void*)pthread_self());
}

void* arrThreadFunc_2(){
    int tmp_policy;
    struct sched_param state_sched;

    pthread_getschedparam(pthread_self(), &tmp_policy, &state_sched);
    printf("\tTHREAD CREATE ID = %d PRIO = %d (POLICY %d)\n", (int)pthread_self(), state_sched.sched_priority, tmp_policy);
    pthread_exit((void*)pthread_self());
}