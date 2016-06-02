#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int *nul = (int *)0;
    int deref_nul = *nul;
    printf(1, "%d\n", deref_nul);
    exit();
}
