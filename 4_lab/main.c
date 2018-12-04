#include <stdio.h>

#define RESET "\033[0m"

struct task{
    int number;
    int C; //время выполнения задачи
    int T; //период задачи
    int D; //дедлайн задачи (T = D)
    int prio; //приоритет задачи
    int work; //признак осуществления работы
    int color; //цвет
};

int nod(int,int); //нахождение НОД (Наибольший общий делитель)
int nok(int,int); //нахождение НОК (наименьшее общее кратное)
int findModelingTime(int, struct task*); //нахождение времени моделирования
void setPrio(int,struct task*); //инициализация приоритетов задач
void printTasks(int,struct task*); //печать информации о задачах
int shedulder(int,struct task*,int); //функция - планировщик
void printOneTask(struct task*); //печать параметров одной задачи


int main(){
    int countTask = 0;
    printf("Моделирование работы алгоритма DM (Deadline monotonic)\n");
    printf("Введите количество задач (3 <= N <= 10): ");
    scanf("%d", &countTask);
    struct task arrTask[countTask];
    for(int i=0; i<countTask; i++){
        printf("Задача #%d\n",i);
        arrTask[i].number = i; //номер
        printf("\tC: "); scanf("%d", &arrTask[i].C);
        printf("\tT: "); scanf("%d", &arrTask[i].T);
        arrTask[i].D = arrTask[i].T; //период = дедлайн (условие задачи)
        arrTask[i].prio = 0; //обнуление (убираем мусор)
        arrTask[i].work = 0; //обнуление (убираем мусор)
        arrTask[i].color = i+1; //цвет = номер
    }

    //вычисление времени моделирования
    int modellingTime = findModelingTime(countTask,arrTask);
    printf("Время моделирования: %d\n", modellingTime);

    //задаем приоритет
    setPrio(countTask, arrTask);

    printTasks(countTask, arrTask);

    int result = shedulder(countTask, arrTask, modellingTime);

    return 0;
}

//нахождение НОД (Наибольший общий делитель)
int nod(int a, int b){
    return b ? nod(b, a % b) : a;
}

//нахождение НОК (наименьшее общее кратное)
int nok(int a, int b){
    return a / nod(a,b) * b;
}

//нахождение времени моделирования
int findModelingTime(int countTask, struct task* tasks){
    int array[countTask], tmp_nok = 0;
    tmp_nok = tasks[0].T;
    for(int i=1; i<countTask; i++){
        tmp_nok = nok(tmp_nok,tasks[i].T);
    }
    return tmp_nok;
}

void setPrio(int countTask,struct task* tasks){
    struct task tmpTask;

    for(int i=0; i<countTask; i++){
        for(int j=0; j<countTask-1; j++){
            if(tasks[j].D > tasks[j+1].D){
                tmpTask =  tasks[j+1];
                tasks[j+1] = tasks[j];
                tasks[j] = tmpTask;
            }
        }
    }

    for(int i=0; i<countTask; i++){
        tasks[i].prio = countTask - i - 1;
    }
}

void printTasks(int countTask,struct task* tasks){
    for(int i=0; i<countTask; i++){
        printf("Task #%d\n", tasks[i].number);
        printf("\tC:  %d\n", tasks[i].C);
        printf("\tT:  %d\n", tasks[i].T);
        printf("\tD:  %d\n", tasks[i].D);
        printf("\tprio:%d\n", tasks[i].prio);
    }
}

int shedulder(int countTask, struct task* tasks, int modellingTime){
    for(int i=1; i<modellingTime + 1; i++){
        printf("Текущее время моделирования: %d\n", i);

        //перебор задач (смотрим период и пропуск дедлайна)
        for(int j=0; j<countTask; j++){
            if(i % tasks[j].T == 0){
                printf("\033[0;3%dm",tasks[j].color);
                printf("\tЗадача #%d прошла период\n",tasks[j].number);
                printf("%s",RESET);
            }
        }

        for(int j=0; j<countTask; j++){
            if(tasks[j].work == tasks[j].C){
                continue;
            }
            printf("\033[0;3%dm",tasks[j].color);
            printf("\tРаботает задача #%d\n",tasks[j].number);
            printf("%s",RESET);
            printOneTask(&tasks[j]);
            tasks[j].work = tasks[j].work + 1;
            break;
        }

        //перебор задач (смотрим период и пропуск дедлайна)
        for(int j=0; j<countTask; j++){
            if(i % tasks[j].T == 0){
                //проверка на пропуск дедлайна
                if(tasks[j].work < tasks[j].C){
                    printf("\033[0;3%dm",tasks[j].color);
                    printf("\tЗадача #%d пропустила дедлайн\n", tasks[j].number);
                    printf("%s",RESET);
                    printf("Моделирование завершено.\n");
                    return 1;
                }
                tasks[j].work = 0;
            }
        }
    }
    return 0;
}

void printOneTask(struct task* tempTask){
    printf("\033[0;3%dm",tempTask->color);
    printf("\t\tC:  %d\n", tempTask->C);
    printf("\t\tT:  %d\n", tempTask->T);
    printf("\t\tD:  %d\n", tempTask->D);
    printf("\t\tprio:%d\n", tempTask->prio);
    printf("\t\twork:%d (+1)\n", tempTask->work);
    printf("%s",RESET);
}