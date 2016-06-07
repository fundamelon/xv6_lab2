#include "types.h"
#include "user.h"

Semaphore *sem_A;
Semaphore *sem_B;

void A(void* arg_ptr) {
    sem_acquire(sem_A);
    int i;
    for(i = 0; i < 10000000; i++);
    printf(0, "A\n");
    sem_signal(sem_B);
    texit();
}


void B( void* arg_ptr) {
    sem_acquire(sem_B);
    int i;
    for(i = 0; i < 10000000; i++);
    printf(0, "B\n");
    sem_signal(sem_A);
    texit();
}




int main(){

    printf(0, "Testing basic semaphore... ");
    Semaphore *s = sem_make(1);
    sem_acquire(s);
    sem_signal(s);
    printf(0, "success\n");

    printf(0, "Testing two threads...\n");
    
    printf(0, "Expected order is AB\n");
    sem_A = sem_make(1);
    sem_B = sem_make(0);

    void *tid;
    tid = thread_create(A, (void*)0);
    if(tid <= 0) printf(1, "thread error\n");

    tid = thread_create(B, (void*)0);
    if(tid <= 0) printf(1, "thread error\n");

    while(wait()>=0);

    printf(0, "Expected order is BA\n");
    sem_A = sem_make(0);
    sem_B = sem_make(1);


    tid = thread_create(A, (void*)0);
    if(tid <= 0) printf(1, "thread error\n");

    tid = thread_create(B, (void*)0);
    if(tid <= 0) printf(1, "thread error\n");

    while(wait()>=0);

    printf(0, "\nsuccess\n");
    
    exit();
}
