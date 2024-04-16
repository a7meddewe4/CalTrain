#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "caltrain.h"


void station_init(struct station *station)
{
	
    station->numberOfEmptySeats=0;
    station->numberOfPassengersinTheStation=0;
    station->numberOfPassengersWalkinginTheTrain=0;
    pthread_mutex_init(&(station->mutex), NULL);
    pthread_cond_init(&(station->TrainWaiting), NULL);
    pthread_cond_init(&(station->Traincame), NULL);
    
}

void station_load_train(struct station *station, int count)
{

    pthread_mutex_lock(&(station->mutex));
   // printf("train came  now %d\n",count);
	station->numberOfEmptySeats=count;
    while((station->numberOfEmptySeats!=0)&&(station->numberOfPassengersinTheStation!=0))
    {
        pthread_cond_broadcast(&(station->Traincame));
      //  printf("i dead here b\n"); 
        pthread_cond_wait(&(station->TrainWaiting),&(station->mutex));
    }
    while(station->numberOfPassengersWalkinginTheTrain>0)
    {
     
     pthread_cond_wait(&(station->TrainWaiting),&(station->mutex));   
    }

    station->numberOfEmptySeats=0;
    station->numberOfPassengersWalkinginTheTrain=0;
    pthread_mutex_unlock(&(station->mutex));
   
}

void station_wait_for_train(struct station *station)
{
        
    pthread_mutex_lock(&(station->mutex));
  //  printf("i am passenger \n");
	station->numberOfPassengersinTheStation++;
     
    while(station->numberOfEmptySeats==station->numberOfPassengersWalkinginTheTrain)
    {
  
         pthread_cond_wait(&(station->Traincame),&(station->mutex));    
    }

    
    station->numberOfPassengersinTheStation--;
    station->numberOfPassengersWalkinginTheTrain++;
    pthread_mutex_unlock(&(station->mutex));
}

void station_on_board(struct station *station)
{
       
    pthread_mutex_lock(&(station->mutex));
            station->numberOfPassengersWalkinginTheTrain--;
            station->numberOfEmptySeats--;
             
     if((station->numberOfPassengersWalkinginTheTrain==0||station->numberOfEmptySeats==0))
    {
        pthread_cond_signal(&(station->TrainWaiting));
    }

    pthread_mutex_unlock(&(station->mutex));
}
