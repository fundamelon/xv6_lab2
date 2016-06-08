#include "types.h"
#include "user.h"


int missionaries_waiting;
int cannibals_waiting;

int missionaries_onboard;
int cannibals_onboard;

Semaphore* sem_arrival;
Semaphore* sem_send_missionary;
Semaphore* sem_send_cannibal;

lock_t* msg_mutex;


// check if the boat is OK to go
int valid_combination() {
    return ((missionaries_onboard == 3 && cannibals_onboard == 0) ||
            (missionaries_onboard == 2 && cannibals_onboard == 1) ||
            (missionaries_onboard == 0 && cannibals_onboard == 3));
}


// send the boat with currently onboard people
void rowBoat() {
    // signal onboard threads to remove themselves
    int i;
    for(i = 0; i < missionaries_onboard; i++)
        sem_signal(sem_send_missionary);
    for(i = 0; i < cannibals_onboard; i++)
        sem_signal(sem_send_cannibal);

    lock_acquire(msg_mutex);
    printf(0, "Boat left with %d missionaries and %d cannibals\n", missionaries_onboard, cannibals_onboard);
    lock_release(msg_mutex);

    // reset with a new boat; add waiting people
    missionaries_onboard = missionaries_waiting;
    cannibals_onboard = cannibals_waiting;
    missionaries_waiting = cannibals_waiting = 0;

    // tell last guy he can try
    //while(waiting >= 0)
    //sem_signal(sem_waitlist);
}


void missionaryArrives(void* arg_ptr) {

    sem_acquire(sem_arrival);

    lock_acquire(msg_mutex);
    printf(0, "Missionary arrives\n");
    lock_release(msg_mutex);

    missionaries_onboard++; // board the boat

    if(valid_combination())
        rowBoat();
    else if(missionaries_onboard == 2 && cannibals_onboard >= 2) {
        cannibals_waiting = cannibals_onboard-1;
        cannibals_onboard = 1;
        rowBoat();
    }

    sem_signal(sem_arrival);
    sem_acquire(sem_send_missionary);

    texit();
}



void cannibalArrives(void* arg_ptr) {

    sem_acquire(sem_arrival);

    lock_acquire(msg_mutex);
    printf(0, "Cannibal arrives\n");
    lock_release(msg_mutex);
    
    cannibals_onboard++; // board the boat

    if(valid_combination())
        rowBoat();
    else if(cannibals_onboard >= 3) {
        missionaries_waiting = missionaries_onboard;
        missionaries_onboard = 0;
        rowBoat();
    }

    sem_signal(sem_arrival);
    sem_acquire(sem_send_cannibal);

    texit();
}


void sendMissionaries(int num) {
    
    void* tid;
    int i;
    for(i = 0; i < num; i++) {
        tid = thread_create(missionaryArrives, (void*)0);
        if(tid<=0) printf(1, "thread error\n");
    }
}


void sendCannibals(int num) {

    void* tid;
    int i;
    for(i = 0; i < num; i++) {
        tid = thread_create(cannibalArrives, (void*)0);
        if(tid<=0) printf(1, "thread error\n");
    }
}


int main() {
    
    msg_mutex = malloc(sizeof(lock_t));
    lock_init(msg_mutex);

    missionaries_onboard = 0;
    cannibals_onboard = 0;

    sem_arrival = sem_make(1);
    sem_send_missionary = sem_make(0);
    sem_send_cannibal = sem_make(0);

    printf(0, "Missionary problem\n\n");

    printf(0, "\nBasic test\n");

    printf(0, "sending 3 missionaries...\n");
    sendMissionaries(3);
    while(wait() >= 0);

    printf(0, "sending 2 missionaries and 1 cannibals...\n");
    sendMissionaries(2);
    sendCannibals(1);
    while(wait() >= 0);

    printf(0, "sending 3 cannibals...\n");
    sendCannibals(3);
    while(wait() >= 0);

    printf(0, "\nLarge test 1\n");

    sendMissionaries(1);
    sendCannibals(3);
    sendMissionaries(2);
    sendCannibals(2);
    sendMissionaries(4);

    while(wait() >= 0);

    printf(0, "\nLarge test 2\n");
    sendCannibals(2);
    sendMissionaries(2);
    sendCannibals(1);
    sendMissionaries(2);
    sendCannibals(1);
    sendMissionaries(1);
    sendCannibals(1);
    sendMissionaries(1);
    sendCannibals(1);

    while(wait() >= 0);

    exit();
}
