#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define MaxMoney 1000
 
void *CachierReturn();
void *CachierTake();
 
int threads_running=0;
int Money=500;
char *SAFE;
 
pthread_cond_t Safe_Not_Full=PTHREAD_COND_INITIALIZER;
pthread_cond_t Safe_Not_Empty=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mVar=PTHREAD_MUTEX_INITIALIZER;
 
int main()
{    
    pthread_t crtid,ctid;
 
    SAFE=(char *) malloc(sizeof(char) * MaxMoney);            
 
    pthread_create(&crtid,NULL,CachierReturn,NULL);
    pthread_create(&ctid,NULL,CachierTake,NULL);
 
    pthread_join(crtid,NULL);
    pthread_join(ctid,NULL);

 

    return 0;
}
 
void *CachierReturn()
{    
    while(1)
    {
    	int tempMoney = ((rand()&(100-1))+1);
        pthread_mutex_lock(&mVar);
     	if(Money>MaxMoney)
        {                        
            pthread_cond_wait(&Safe_Not_Full,&mVar);
        }  
                            
        SAFE[Money++]='@';
        int returnedMoney=Money+tempMoney;
        sleep(1);
        printf("Cachier return %d$ in cachebox and now there is: %d$\n",(tempMoney+1) ,returnedMoney);
        Money=returnedMoney;
        pthread_mutex_unlock(&mVar);
        pthread_cond_signal(&Safe_Not_Empty);  
    }
}


void *CachierTake()
{
    while(1)
    {
    	int tempMoney = ((rand()&(150-1))+1);
    	
        pthread_mutex_lock(&mVar);
        while(Money<=0||Money<tempMoney)
        {            
      	 pthread_cond_wait(&Safe_Not_Empty,&mVar);
        }
         
        int takenMoney=Money-tempMoney;  
        sleep(1);
        printf("Cachier take %d$ from cachebox and now there is: %d$\n",tempMoney,takenMoney);  
        Money=takenMoney;      
        pthread_mutex_unlock(&mVar);        
        pthread_cond_signal(&Safe_Not_Full);   
    }   
}
