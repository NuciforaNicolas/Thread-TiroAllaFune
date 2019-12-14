#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_t tp[2];
pthread_mutex_t mutex;
int pos = 0; 
long vittorie_tp0 = 0, vittorie_tp1 = 0;
pthread_cond_t wait_pt0, wait_pt1;

void *tp0_run(){
    while(1){

        if(vittorie_tp0 == 10 || vittorie_tp1 == 10)
            pthread_exit((void *)vittorie_tp0);

        int recupero = rand() % 4;
        int forza = rand() % 6;
        sleep(recupero);

        pthread_mutex_lock(&mutex);
            if(pos >= 10){
                vittorie_tp1++;
                pos = 0;
                printf("\t\t\ttp1 won: %ld\n", vittorie_tp1);
                pthread_cond_signal(&wait_pt1);
            }
            
            pos -= forza;
            printf("tp0 position: %d\n", pos);
            if(pos <= -10)
                pthread_cond_wait(&wait_pt0, &mutex);
            
        pthread_mutex_unlock(&mutex);
    }
}

void *tp1_run(){
    while(1){
        if(vittorie_tp1 == 10 || vittorie_tp0 == 10)
            pthread_exit((void *)vittorie_tp1);

        int recupero = rand() % 4;
        int forza = rand() % 6;
        sleep(recupero);

        pthread_mutex_lock(&mutex);
            if(pos <= -10){
                vittorie_tp0++;
                pos = 0;
                printf("tp0 won: %ld\n", vittorie_tp0);
                pthread_cond_signal(&wait_pt0);
            }

            pos += forza;
            printf("\t\t\ttp1 position: %d\n", pos);
            if(pos >= 10)
                pthread_cond_wait(&wait_pt1, &mutex);

        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char** argv){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&wait_pt0, NULL);
    pthread_cond_init(&wait_pt1, NULL);

    pthread_create(&tp[0], NULL, &tp0_run, NULL);
    pthread_create(&tp[1], NULL, &tp1_run, NULL);

    long restp0 = 0;
    long restp1 = 0;

    pthread_join(tp[1], (void *)&restp1);
    pthread_join(tp[0], (void *)&restp0);


    if(restp0 > restp1)
        printf("tp0 reached more victories than tp1: %ld\n", restp0);
    else
        printf("tp1 reached more victories than tp0: %ld\n", restp1);

    return 0;
}