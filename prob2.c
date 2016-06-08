#include "types.h"
#include "user.h"

#define MAXMONKEYS 128
#define TREESIZE 3


Semaphore *tree;
lock_t *msg_mutex; // lock to prevent printf() from being interrupted

Semaphore *climb;
Semaphore *up_mutex;
Semaphore *down_mutex;


int on_tree = 0;
int comingup = 0;
int comingdown = 0;
int dominant_present = 0;
int dominant_tid = 0;
int dominant_waiting = 0;

// queue of monkeys waiting on dominant guy
int q_dom[MAXMONKEYS];
int q_dom_head = 0;
int q_dom_tail = 0;


void work(int a) { int i; for(i=0; i<a; i++); }

void dominant_appears() {
    lock_acquire(msg_mutex);
    printf(0, "\tmonkey %d (DOMINANT) appears\n", getpid());
    lock_release(msg_mutex);
}

void climb_up() { 
    lock_acquire(msg_mutex);
    printf(0, "\tmonkey %d %s starts climbing up\n", getpid(), ((getpid()==dominant_tid)?"(DOMINANT)":""));
    lock_release(msg_mutex); 
    work(100000); 
    lock_acquire(msg_mutex);
    printf(0, "\tmonkey %d %s climbed up (%d going up)\n", getpid(), ((getpid()==dominant_tid)?"(DOMINANT)":""), comingup);
    lock_release(msg_mutex);
}
 
void get_coconut() { 
    work(500000); // takes 5 times as long as to get up/down (decrease consistency)
    lock_acquire(msg_mutex);
    printf(0, "\tmonkey %d %s got a coconut (%d on the tree)\n", getpid(), ((getpid()==dominant_tid)?"(DOMINANT)":""), on_tree);
    lock_release(msg_mutex);
}
 
void climb_down() {
    lock_acquire(msg_mutex);
    printf(0, "\tmonkey %d %s starts climbing down\n", getpid(), ((getpid()==dominant_tid)?"(DOMINANT)":""));
    lock_release(msg_mutex); 
    work(100000); 
    lock_acquire(msg_mutex);
    printf(0, "\tmonkey %d %s climbed down (%d coming down)\n", getpid(), ((getpid()==dominant_tid)?"(DOMINANT)":""), comingdown);
    lock_release(msg_mutex);
}


void advanced_monkey(void* arg_ptr) {

    int* arg = (int*)arg_ptr;
    int dominant = *arg;

    if(dominant) {
        dominant_tid = getpid();
        dominant_appears();
        if(comingup == TREESIZE || on_tree == TREESIZE) {
            dominant_waiting = 1;
            tsleep();
            dominant_waiting = 0;
        }
    } else
        sem_acquire(tree);


    sem_acquire(up_mutex);
    on_tree++;
    comingup++;
    if(comingup == 1) //first monkey coming up
        sem_acquire(climb);
    sem_signal(up_mutex);

    if(!dominant && dominant_present) thread_yield();
    climb_up();

    sem_acquire(up_mutex);
    comingup--;
    if(comingup == 0) // last monkey coming up
        sem_signal(climb);
    sem_signal(up_mutex);

    if(!dominant && dominant_present) thread_yield();
    get_coconut();

    sem_acquire(down_mutex);
    comingdown++;
    if(comingdown == 1) // first monkey coming down
        sem_signal(climb);
    sem_signal(down_mutex);

    if(!dominant && dominant_present) thread_yield();
    climb_down();

    sem_acquire(down_mutex);
    comingdown--;
    if(comingdown == 0) // last monkey coming down
        sem_signal(climb);
    on_tree--;
    sem_signal(down_mutex);

    if(!dominant) {
        if(dominant_waiting) { 
            twakeup(dominant_tid); // let dominant know one monkey left
            thread_yield();
        }
        sem_signal(tree);
    }


    texit();
}


void basic_monkey(void* argptr) {

    sem_acquire(tree);

    climb_up();
    get_coconut();
    climb_down();

    sem_signal(tree);

    texit();
}

int main() {

    printf(0, "Monkey problem\n");
    msg_mutex = malloc(sizeof(lock_t));
    lock_init(msg_mutex);
  

    printf(0, "\nBasic monkeys\n");
    tree = sem_make(3);

    void *tid;
    int i;
    for(i = 0; i < 5; i++) {
        tid = thread_create(basic_monkey, (void*)0);
        if(tid <= 0) printf(1, "thread error\n");
    }

    while(wait()>=0);


    printf(0, "\nAdvanced monkeys (with Dominant monkey)\n");
    tree = sem_make(TREESIZE);
    climb = sem_make(1);
    up_mutex = sem_make(1);
    down_mutex = sem_make(1);

    int dominant_monkey = random(15);
    int argval;
    // hack
    int one = 1;
    int zero = 0;

    for(i = 0; i < 15; i++) {
        argval = (i == dominant_monkey-1);
        if(argval)
            tid = thread_create(advanced_monkey, (void*)&one);
        else
            tid = thread_create(advanced_monkey, (void*)&zero);
        if(tid<=0) printf(1, "thread error\n");
    }

    while(wait()>=0);


    printf(0, "Monkey problem complete\n");

    exit();
}
