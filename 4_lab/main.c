#include <stdio.h>
#include <unistd.h>

#define N 3

int nod(int,int); //нахождение НОД (Наибольший общий делитель)
int nok(int,int); //нахождение НОК (наименьшее общее кратное)
int findModelingTime(); //нахождение времени моделирования

int main(){
    int modellingTime = findModelingTime();
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
int findModelingTime(){
    int array[N], tmp_nok = 0;

    printf("Введите %d числа(-ел): ", N);
    for(int i=0; i<N; i++){
        array[i] = 0;
        scanf("%d", &array[i]);
    }
    
    tmp_nok = array[0];
    for(int i=1; i<N; i++){
        tmp_nok = nok(tmp_nok,array[i]);
    }
}