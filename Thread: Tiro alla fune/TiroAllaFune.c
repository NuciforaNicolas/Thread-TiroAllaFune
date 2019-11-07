#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_t giocatori[2];
pthread_mutex_t mut;
int posizione;
long vittorie_tp0, vittorie_tp1;
pthread_cond_t wait_player0, wait_player1;

void * play(void* ptr){
    int recupero = 0;
    int forza = 0;
    while(1){
        recupero = rand() % 4;
        forza = rand() % 6;
        sleep(recupero);

        printf("\t\t\t\tPosizione: %d\n\n", posizione);

        if(pthread_equal(pthread_self(), giocatori[0])){
            pthread_mutex_lock(&mut);

            if(posizione >= 10){
                vittorie_tp1++;
                posizione = 0;
                pthread_cond_signal(&wait_player1);
            }
            
            posizione -= forza;
            printf("Forza giocatore[0]: %d\n", forza);
            if(posizione <= -10){
                pthread_cond_wait(&wait_player0, &mut);
            }

            printf("Vittorie tp[0]: %ld\n", vittorie_tp0);
            pthread_mutex_unlock(&mut); 
        }
        else if(pthread_equal(pthread_self(), giocatori[0])){
            pthread_mutex_lock(&mut);

            if(posizione <= -10){
                vittorie_tp0++;
                posizione = 0;
                pthread_cond_signal(&wait_player0);
            }
            
            posizione += forza;
            printf("\t\t\t\t\t\t\tForza giocatore[1]: %d\n", forza);
            if(posizione >= 10){
                pthread_cond_wait(&wait_player1, &mut);  
            }

            printf("\t\t\t\t\t\t\tVittorie tp[1]: %ld\n", vittorie_tp1);
            pthread_mutex_unlock(&mut); 
        }

        if(vittorie_tp0 == 10)
            pthread_exit((void *)vittorie_tp0);
        else 
            if(vittorie_tp1 == 10)
                pthread_exit((void *) vittorie_tp1);

    }
}

int main(int argc, char** argv){

    long res = 0;
    posizione = 0;
    vittorie_tp0 = 0;
    vittorie_tp1 = 0;

    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&wait_player0, NULL);
    pthread_cond_init(&wait_player1, NULL);

    pthread_create(&giocatori[0], NULL, &play, NULL);
    pthread_create(&giocatori[1], NULL, &play, NULL);

    pthread_join(giocatori[1], (void *)&res);
    pthread_join(giocatori[0], (void *)&res);

    if(res == vittorie_tp0)
        printf("\n\nPlayer 0 won! Score: %ld\n", res);
    else
        printf("\n\nPlayer 1 won! Score: %ld\n", res);


}
