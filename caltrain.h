#include <pthread.h>

struct station {
	int numberOfEmptySeats;
    int numberOfPassengersinTheStation;
    int numberOfPassengersWalkinginTheTrain;
    pthread_mutex_t mutex;
    pthread_cond_t TrainWaiting ;
    pthread_cond_t Traincame;

};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
