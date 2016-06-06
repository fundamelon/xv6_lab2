#include "types.h"
#include "user.h"

void A() {
    printf(0, "A");
}


void B() {
    printf(0, "B");
}


int main(){

    printf(0, "Testing basic semaphore... ");
    Semaphore *s = sem_make(1);
    sem_acquire(s);
    sem_signal(s);
    printf(0, "success\n");

    return 0;

    printf(0, "Testing two threads...\n");
    thread_create(A, 0);
    thread_create(B, 0);
    printf(0, "\nsuccess\n");
    
    return 0;
}
