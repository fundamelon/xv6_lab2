#include "types.h"
#include "user.h"
#define PSIZE (4096)
#include "mmu.h"
#include "spinlock.h"
#include "x86.h"
#include "proc.h"

// Thread table
int ttable[16];
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


typedef struct {
    int count;
    int queue[16];
    int queue_head;
    int queue_tail;
    lock_t *lock;
} Semaphore;

void sem_acquire(Semaphore *s) {
    int got = 0;
    while(!got) {
        lock_acquire(s->lock);
        if(s->count > 0) {
            s->count--;
            got = 1;
        }
        lock_release(s->lock);
    }
}

void sem_signal(Semaphore *s) {
    lock_acquire(s->lock);
    s->count++;
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
        return garbage_stack;
    }
    if(tid == 0){
        printf(1,"tid = 0 return \n");
    }
//    wait();
//    free(garbage_stack);

    return 0;
}

// generate 0 -> max random number exclude max.
int random(int max){
    rands = rands * 1664525 + 1013904233;
    return (int)(rands % max);
}
