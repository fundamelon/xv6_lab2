#include "types.h"
#include "user.h"

void A(void* arg_ptr) {
    int i;
    for(i = 0; i < 5; i++) {
        int j;
        for(j = 0; j < 10000; j++);
        printf(0, "A\n");
        thread_yield();
    }
    texit();
}


void B( void* arg_ptr) {
    int i;
    for(i = 0; i < 5; i++) {
        int j;
        for(j = 0; j < 10000; j++);
        printf(0, "B\n");
        thread_yield();
    }
    texit();
}




int main(){

    printf(0, "Testing thread yield...\n");

    printf(0, "Expected: A and B should alternate.\n");

    void *tid;
    tid = thread_create(A, (void *)0);
    if(tid <= 0) printf(1, "error\n");

    tid = thread_create(B, (void *)0);
    if(tid <= 0) printf(1, "error\n");

    while(wait() >= 0);

    printf(0, "\nsuccess\n");
    
    exit();
}
