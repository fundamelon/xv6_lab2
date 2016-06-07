#include "types.h"
#include "user.h"


Semaphore *tree;
Semaphore *msg_mutex;

Semaphore climb;
Semaphore up_mutex;
Semaphore down_mutex;

int comingup = 0;
int comingdown = 0;


void work(int a) { int i; for(i=0; i<t; i++); }

void climb_up() { 
    work(100000); 
    sem_acquire(msg_mutex);
    printf(0, "\tmonkey %d climbs up (%d monkeys on the tree)\n", getpid());
    sem_signal(msg_mutex);
}
 
void get_coconut() { 
    work(100000); 
    sem_acquire(msg_mutex);
    printf(0, "\tmonkey %d gets a coconut\n", getpid());
    sem_signal(msg_mutex);
}
 
void climb_down() { 
    work(100000); 
    sem_acquire(msg_mutex);
    printf(0, "\tmonkey %d climbs down (%d monkeys on the tree)\n", getpid());
    sem_signal(msg_mutex);
}


void advanced_monkey(void* argptr) {
    
    sem_acquire(tree);

    sem_acquire(up_mutex);
    comingup++;
    if(comingup == 1) //first monkey coming up
        sem_acquire(climb);
    sem_signal(up_mutex);

    climb_up();

    sem_acquire(up_mutex);
    comingup--;
    if(comingup == 0) // last monkey coming up
        sem_signal(climb);
    sem_signal(up_mutex);

    get_coconut();

    sem_acquire(down_mutex);
    comingdown++;
    if(comingdown == 1) // first monkey coming down
        sem_signal(climb);
    sem_signal(down_mutex);

    climb_down();

    sem_acquire(down_mutex);
    comingdown--;
    if(comingdown == 0) // last monkey coming down
        sem_signal(climb);
    sem_signal(down_mutex);

    sem_signal(tree);

    texit();
}


void basic_monkey(void* argptr) {

    sem_acquire(tree);
   
    get_coconut();

    sem_signal(tree);

    texit();
}


int main() {

    printf(0, "Monkey problem\n");
    msg_mutex = sem_make(1);
  

    printf(0, "\nBasic monkeys\n");
    
    tree = sem_make(3);

    void *tid;

    int i;
    for(i = 0; i < 5; i++) {
        tid = thread_create(basic_monkey, (void*)0);
        if(tid <= 0) printf(1, "thread error\n");
    }

    while(wait()>=0);


    printf(0, "\n\nAdvanced monkeys (with Dominant monkey)\n");
    
    
    tree = sem_make(3);
    climb = sem_make(1);
    up_mutex = sem_make(1);
    down_mutex = sem_make(1);

    exit();
}
