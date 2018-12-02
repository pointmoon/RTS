#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define RESET "\033[0m"
#define YELLOW  "\033[1;33m"
#define GREEN  "\033[1;32m"
#define RED "\033[1;35m"
#define BLUE "\033[1;36m"

#define M 5 //Количество писателей
#define N 10 //Количесвто читателей\

unsigned int iter; //итерация
sem_t accessM,readresM,orderM; //семафоры
unsigned int readers = 0;	// Количество читателей, обращающихся к ресурсу

void *reader(void *prm)
{
	int num1=(int)prm;
	int i=0,r;
	for(i;i<iter;i++)
	{

		if (sem_wait(&orderM)==0)
		{
			printf("%s%d Читатель %d в очереди__________Ч%d%s\n",YELLOW,i,num1,num1,RESET);	// Помните наш порядок прибытия
		} 
		sem_wait(&readresM);
		if (readers == 0)
		{
			sem_wait(&accessM);
			printf("%sЧитатели захватили семафор доступа к ресурсам!%s\n",RED,RESET);
		}
		readers++;				
		sem_post(&orderM);		
		sem_post(&readresM);	

		printf("%s%d Работает читатель %d________________Ч%d%s\n",YELLOW,i,num1,num1,RESET);      // Здесь читатель может прочитать ресурс по своему усмотрению
		sem_wait(&readresM);				 // Мы можем манипулировать счетчиками читателей
		readers--;							 // Мы уходим, есть еще один читатель
		if (readers == 0)				// Если в настоящее время читателей больше нет...
		{
			sem_post(&accessM);				// ...rосвободить эксклюзивный доступ к ресурсу
			printf("%sЧитатели освободили семафор доступа к ресурсам!%s\n",RED,RESET);
		}
		sem_post(&readresM);				 // На данный момент мы получили доступ к числу читателей
	}
}

void *writer(void *prm)
{
	int num2=(int)prm;
	int j=0,r;
	for(j;j<iter;j++)
	{
		if(sem_wait(&orderM)==0)
		{
			printf("%s%d Писатель %d в очереди__________П%d%s\n",GREEN,j,num2,num2,RESET); // Помните наш порядок прибытия
		}
		sem_wait(&accessM);					// Запросить эксклюзивный доступ к ресурсу
		printf("%sПисатель %d захватил семафор доступа к ресурсам!%s\n",BLUE,num2,RESET);
		sem_post(&orderM);					 // Оформить заказ семафора прибытия (мы обслуживались)

		printf("%s%d Работает писатель %d________________П%d%s\n",GREEN,j,num2,num2,RESET); // Здесь писатель может модифицировать ресурс по своему усмотрению
		sem_post(&accessM);					// Отключить эксклюзивный доступ к ресурсу
		printf("%sПисатель %d освободил семафор доступа к ресурсам!%s\n",BLUE,num2,RESET);
	}
}

int main()
{
	pthread_t threadRE[N]; //массив потоков читателей
	pthread_t threadWR[M]; // массив потоков писателей
	
	//инициализация семафоров
	if(sem_init(&accessM,0,1) != 0){
		printf("ERROR (sem_init(&accessM,0,1))\n");
		exit(EXIT_FAILURE);
	}
	if(sem_init(&readresM,0,1) != 0){
		printf("ERROR sem_init(&readresM,0,1))\n");
		sem_destroy(&accessM);
		exit(EXIT_FAILURE);
	}
	if(sem_init(&orderM,0,1) != 0){
		printf("ERROR sem_init(&orderM,0,1))\n");
		sem_destroy(&accessM);
		sem_destroy(&readresM);
		exit(EXIT_FAILURE);
	}

	printf("Введите количество итераций: ");
	scanf("%d",&iter);
	printf("Iter                         ОЧЕРЕДЬ/ВЫПОЛНЕНИЕ\n");
	int i;
	for(i=0;i<M;i++)
	{
		pthread_create(&(threadWR[i]),NULL,writer,(void*)i);
	}
	for(i=0;i<N;i++)
	{
		pthread_create(&(threadRE[i]),NULL,reader,(void*)i);
	}


	for(i=0;i<N;i++)
	{
		pthread_join(threadRE[i],NULL);
	}
	for(i=0;i<M;i++)
	{
		pthread_join(threadWR[i],NULL);
	}

	sem_destroy(&accessM);
	sem_destroy(&readresM);
	sem_destroy(&orderM);
	printf("All sem a destroy, good buy!\n");
	return EXIT_SUCCESS;
}