#include <stdio.h>
#include <unistd.h>

struct task{
    int C; //время выполнения задачи
    int T; //период задачи
    int D; //дедлайн задачи (T = D)
};

int nod(int,int); //нахождение НОД (Наибольший общий делитель)
int nok(int,int); //нахождение НОК (наименьшее общее кратное)
int findModelingTime(int, struct task*); //нахождение времени моделирования

int main(){
    int countTask = 0;
    printf("Моделирование работы алгоритма DM (Deadline monotonic)\n");
    printf("Введите количество задач (3 <= N <= 10): ");
    scanf("%d", &countTask);
    struct task arrTask[countTask];
    for(int i=0; i<countTask; i++){
        printf("Задача #%d\n",i);
        printf("\tC: "); scanf("%d", &arrTask[i].C);
        printf("\tT: "); scanf("%d", &arrTask[i].T);
        arrTask[i].D = arrTask[i].T;
    }
    int modellingTime = findModelingTime(countTask,arrTask);
    printf("Время моделирования: %d\n", modellingTime);

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