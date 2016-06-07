#include "types.h"
#include "user.h"

Semaphore *sem_A;
Semaphore *sem_B;

void A(void* arg_ptr) {
    sem_acquire(sem_A);
    printf(0, "A starting\n");
    thread_yield();
    int i;
    for(i = 0; i < 10000000; i++);
    printf(0, "A complete\n");
    sem_signal(sem_B);
    texit();
}


void B( void* arg_ptr) {
    sem_acquire(sem_B);
    printf(0, "B starting\n");
    int i;
    for(i = 0; i < 10000000; i++);
    printf(0, "B complete\n");
    sem_signal(sem_A);
    texit();
}




int main(){

    printf(0, "Testing thread yield... ");

    sem_A = sem_make(1);
    sem_B = sem_make(0);

    printf(0, "\nsuccess\n");
    
    exit();
}
