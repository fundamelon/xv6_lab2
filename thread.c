#include "types.h"
#include "user.h"
#define PSIZE (4096)
#include "mmu.h"
#include "spinlock.h"
#include "x86.h"
#include "proc.h"

#define TMAX 128
#define SEM_QMAX 128

#define _DEBUG 0

// Thread table
int ttable[TMAX];
unsigned long rands = 1;

void lock_init(lock_t *lock){
    lock->locked = 0;
}
void lock_acquire(lock_t *lock){
    while(xchg(&lock->locked,1) != 0);
}
void lock_release(lock_t *lock){
    xchg(&lock->locked,0);
}


struct Semaphore {
    int count;
    int q[SEM_QMAX];
    int q_head;
    int q_tail;
    lock_t *lock;
};


Semaphore *sem_make(int start) {
    Semaphore *s = malloc(sizeof(Semaphore));
    s->count = start;
    s->q_head = s->q_tail = 0;
    s->lock = malloc(sizeof(lock_t));
    lock_init(s->lock);
    return s;
}


void sem_acquire(Semaphore *s) {
    // lock sem
    lock_acquire(s->lock);

    // if sem is down: enqueue, unlock, block, lock
    if(s->count == 0) {
        s->q[s->q_head] = getpid();             // enqueue
        s->q_head = (s->q_head + 1) % SEM_QMAX; // increment queue head
        lock_release(s->lock);                  // unlock
        if(_DEBUG) printf(0, "thread %d sleep\n", getpid());
        tsleep();                               // block
        lock_acquire(s->lock);                  // lock
    }

    // take sem down
    s->count--;
    if(_DEBUG) printf(0, "thread %d acquired to %d\n", getpid(), s->count);

    // unlock sem
    lock_release(s->lock);
}

void sem_signal(Semaphore *s) {
    // lock sem
    lock_acquire(s->lock);

    // put sem up
    s->count++;
    if(_DEBUG) printf(0, "thread %d signaled to %d\n", getpid(), s->count);

    // if exists, wakeup a waiting thread
    if(s->q_head != s->q_tail) {
        // wakeup queue tail thread
        if(_DEBUG) printf(0, "thread %d wakeup\n", s->q[s->q_tail]);
        twakeup(s->q[s->q_tail]);
        // pop from queue tail
        if(_DEBUG) printf(0, "thread %d dequeue %d\n", getpid(), s->q[s->q_tail]);
        s->q_tail = (s->q_tail + 1) % SEM_QMAX; 
    }

    // unlock sem
    lock_release(s->lock);
}


void *thread_create(void(*start_routine)(void*), void *arg){
    int tid;
    void * stack = malloc(2 * 4096);
    void *garbage_stack = stack; 
   // printf(1,"start routine addr : %d\n",(uint)start_routine);


    if((uint)stack % 4096){
        stack = stack + (4096 - (uint)stack % 4096);
    }
    if (stack == 0){

        printf(1,"malloc fail \n");
        return 0;
    }

    tid = clone((uint)stack,PSIZE,(uint)start_routine,(int)arg);
    if(tid < 0){
        printf(1,"clone fails\n");
        return 0;
    }
    if(tid > 0){
        //store threads on thread table
        /*
        int i;
        for(i = 0; i <= TMAX; i++)
            if(i == TMAX) {
                printf(1, "max threads reached\n");
                return 0;
            } else if(ttable[i]==0)
                ttable[i] = getpid(); // thread id here
        */
        if(_DEBUG) printf(0, "thread %d created\n", tid);
        return garbage_stack;
    }
    if(tid == 0){
        printf(1,"tid = 0 return \n");
    }
    //wait();
    //free(garbage_stack);

    return 0;
}

// generate 0 -> max random number exclude max.
int random(int max){
    rands = rands * 1664525 + 1013904233;
    return (int)(rands % max);
}
