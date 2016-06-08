1.1: Semaphores

    The Semaphore is defined in thread.c.  It consists of a count int, lock, and a simple circular queue.  

1.2: Thread yield

1.3: Synchronization problems
    1: H2O problem
        To run: run "prob1" in xv6.
        The H2O problem is implemented as a series of tests in the program <proj1>, which can be invoked on the xv6 command line. This program runs 3 different test cases: basic, with 2 H and 1 O atoms, which proves the algorithm's basic functionality; many, which runs the program on 20 H and 10 O atoms, to prove it can handle many threads at once; and random, which results in H and O count somewhere between 1 and 10.  The test suite outputs the amount of input atoms, calculates an expected output, and matches the actual output to the expected.  It will also output an "H" or "O" when each respective atom is made available, to give a sense of progress during the program.  Letters may be somewhat out of order due to scheduling quirks.

    2: Monkey problem
        To run: run "prob2" in xv6.
        They monkey problem is in two parts: the first with basic monkeys, and the second with the advanced monkeys along with a dominant instance.  In the first part, 5 equal monkey threads are created and compete to get coconuts.  Each of the three parts takes some work to do - here it's a simple spin block.  As seen in the output, only 3 monkeys maximum may be on the tree at once - those which want to get on, but have no space, are put to sleep by the tree semaphore.
        The second part is more complex.  Here, each monkey's work is separated into three parts - climb, coconut, descend - and there may also be one, the dominant, which takes priority over the others.  The dominant must begin climbing as soon as possible, once it appears.  In this implementation, the dominant monkey skips the tree semaphore - since it already has a queue of many other monkeys waiting to climb - and begins the climb once any other monkey has finished its own climb.  This is seen in the test output, as the monkey marked DOMINANT performs its climb almost right after appearing.  However, there are other monkeys already up in the "tree", and they are able to perform their own jobs before the Dominant gets to where they are.  This is why their actions appear in between the Dominant monkey getting to the top and it getting a coconut.  However, even though the Dominant is one of the last monkeys to appear (21 in the image), its job is finished before a monkey 11 places before it. (monkey 10 in the image).  Verbose output helps to demonstrate the flow of the program.

    3: Missionary problem


