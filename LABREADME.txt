1.1: Semaphores

    The Semaphore is defined in thread.c.  It consists of a count int, lock, and a simple circular queue.  

1.2: Thread yield

1.3: Synchronization problems
    1: H2O problem
        The H2O problem is implemented as a series of tests in the program <proj1>, which can be invoked on the xv6 command line. This program runs 3 different test cases: basic, with 2 H and 1 O atoms, which proves the algorithm's basic functionality; many, which runs the program on 20 H and 10 O atoms, to prove it can handle many threads at once; and random, which results in H and O count somewhere between 1 and 10.  The test suite outputs the amount of input atoms, calculates an expected output, and matches the actual output to the expected.  It will also output an "H" or "O" when each respective atom is made available, to give a sense of progress during the program.  Letters may be somewhat out of order due to scheduling quirks.

    
