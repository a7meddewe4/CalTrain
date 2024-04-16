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
      // printf("i dead here a\n"); 
     pthread_cond_wait(&(station->TrainWaiting),&(station->mutex));   
    }

 // printf("train left  empty :%d  %d  %d\n",station->numberOfEmptySeats,station->numberOfPassengersWalkinginTheTrain , station->numberOfPassengersinTheStation);
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
    //     printf("i dead here c\n"); 
         pthread_cond_wait(&(station->Traincame),&(station->mutex));    
    }
  //printf("i found a %d chairs we are %d person and %d standing in the train\n",station->numberOfEmptySeats, station->numberOfPassengersinTheStation,station->numberOfPassengersWalkinginTheTrain);
    //wait untill a train with at least 1 empty chair
    
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
