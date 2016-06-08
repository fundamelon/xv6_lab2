#include "types.h"
#include "user.h"

#define MIN(a,b) ((a)<(b)?(a):(b))


Semaphore *sem_H;
Semaphore *mutex;

int water_molecules;


void hReady(void *argptr) {
    
    // fake work
    int i;
    for(i = 0; i < 1000000; i++);
    printf(0, "H");
    sem_signal(sem_H);
    texit();
}


void oReady(void *argptr) {

    sem_acquire(sem_H);
    sem_acquire(sem_H);

    // fake work
    int i;
    for(i = 0; i < 1000000; i++);
    printf(0, "O");

    sem_acquire(mutex);
    water_molecules++;
    sem_signal(mutex);

    texit();
}


// runs threads and resets water_molecules to result
void run_test(int h_atoms, int o_atoms) {

    printf(0, "\tInput atoms: %d H, %d O\n", h_atoms, o_atoms);

    int expected_result = MIN(h_atoms/2, o_atoms);

    printf(0, "\tExpected: %d H2O\n", expected_result);

    water_molecules = 0;
    
    sem_H = sem_make(0);
    mutex = sem_make(1);

    printf(0, "\t");

    void *tid;
    int i;
    for(i = 0; i < o_atoms; i++) {
        tid = thread_create(oReady, (void *)0);
        if(tid <= 0) printf(1, "Thread error\n");
    }

    for(i = 0; i < h_atoms; i++) {
        tid = thread_create(hReady, (void *)0);
        if(tid <= 0) printf(1, "Thread error\n");
    }

    while(wait() >= 0);

    printf(0, "\n\tResult: %d ", water_molecules);
    if(water_molecules == expected_result) 
        printf(0, "(correct)\n");
    else 
        printf(0, "(incorrect)\n");
}



int main() {

    printf(0, "H20 problem\n");

    printf(0, "\nBasic test\n");
    run_test(2, 1);

    printf(0, "\nMany test\n");
    run_test(20, 10);

    // This will cause 5 O threads to be stuck on semaphore.
    //printf(0, "Equal test\n");
    //run_test(10, 10);

    printf(0, "\nRandom test\n");
    int amt = random(10);
    run_test(amt*2, amt);

    printf(0, "H2O problem complete\n");
    exit();
}
