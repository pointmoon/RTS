#include <stdio.h>

struct task{
    int number;
    int C; //время выполнения задачи
    int T; //период задачи
    int D; //дедлайн задачи (T = D)
    int prio; //приоритет задачи
};

int nod(int,int); //нахождение НОД (Наибольший общий делитель)
int nok(int,int); //нахождение НОК (наименьшее общее кратное)
int findModelingTime(int, struct task*); //нахождение времени моделирования
void setPrio(int,struct task*); //инициализация приоритетов задач
void printTasks(int,struct task*); //печать информации о задачах
int shedulder(int,struct task*,int); //функция - планировщик


int main(){
    int countTask = 0;
    printf("Моделирование работы алгоритма DM (Deadline monotonic)\n");
    printf("Введите количество задач (3 <= N <= 10): ");
    scanf("%d", &countTask);
    struct task arrTask[countTask];
    for(int i=0; i<countTask; i++){
        printf("Задача #%d\n",i);
        arrTask[i].number = i;
        printf("\tC: "); scanf("%d", &arrTask[i].C);
        printf("\tT: "); scanf("%d", &arrTask[i].T);
        arrTask[i].D = arrTask[i].T;
        arrTask[i].prio = 0;
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
    struct task tmpTasks[countTask], tmpTask;
    
    for(int i=0; i<countTask; i++){
        tmpTasks[i] = tasks[i];
    }

    for(int i=0; i<countTask; i++){
        for(int j=0; j<countTask-1; j++){
            if(tmpTasks[j].D < tmpTasks[j+1].D){
                tmpTask =  tmpTasks[j+1];
                tmpTasks[j+1] = tmpTasks[j];
                tmpTasks[j] = tmpTask;
            }
        }
    }

    for(int i=0; i<countTask; i++){
        for(int j=0; j<countTask; j++){
            if(tasks[j].number == tmpTasks[i].number){
                tasks[j].prio = i;
                break;
            }
        }
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
    while(modellingTime--){
       
    }
    return 0;
}