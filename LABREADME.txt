1.1: Semaphores
    Test program: run "test_sem" in xv6.
    The Semaphore is defined in thread.c.  It consists of a count int, lock, and a simple circular queue.  The test program runs the semaphore through three tests - one for basic functionality, one for testing two threads and making sure they run in the right order, and one for testing if one thread can wait to acquire() five signal()s from another thread.  This demonstrates the necessary functionality of semaphores needed to continue with the lab.

1.2: Thread yield
    Test program: run "test_yield" in xv6.
    thread_yield() is implemented as a syscall which wraps the normal process yield() function.  The test program demonstrates this by having two threads, each which iterates 5 times printing either "A" or "B" in a row, which yields after each letter.  This causes the letters to alternate.  If they did not alternate, thread_yield would be incorrectly implemented.

1.3: Synchronization problems
    1: H2O problem
        To run: run "prob1" in xv6.
        The H2O problem is implemented as a series of tests in the program <proj1>, which can be invoked on the xv6 command line. This program runs 3 different test cases: basic, with 2 H and 1 O atoms, which proves the algorithm's basic functionality; many, which runs the program on 20 H and 10 O atoms, to prove it can handle many threads at once; and random, which results in H and O count somewhere between 1 and 10.  The test suite outputs the amount of input atoms, calculates an expected output, and matches the actual output to the expected.  It will also output an "H" or "O" when each respective atom is made available, to give a sense of progress during the program.  Letters may be somewhat out of order due to scheduling quirks.

    2: Monkey problem
        To run: run "prob2" in xv6.
        They monkey problem is in two parts: the first with basic monkeys, and the second with the advanced monkeys along with a dominant instance.  In the first part, 5 equal monkey threads are created and compete to get coconuts.  Each of the three parts takes some work to do - here it's a simple spin block.  As seen in the output, only 3 monkeys maximum may be on the tree at once - those which want to get on, but have no space, are put to sleep by the tree semaphore.
        The second part is more complex.  Here, each monkey's work is separated into three parts - climb, coconut, descend - and there may also be one, the dominant, which takes priority over the others.  The dominant must begin climbing as soon as possible, once it appears.  In this implementation, the dominant monkey skips the tree semaphore - since it already has a queue of many other monkeys waiting to climb - and begins the climb once any other monkey has finished its own climb.  This is seen in the test output, as the monkey marked DOMINANT performs its climb almost right after appearing.  However, there are other monkeys already up in the "tree", and they are able to perform their own jobs before the Dominant gets to where they are.  This is why their actions appear in between the Dominant monkey getting to the top and it getting a coconut.  However, even though the Dominant is one of the last monkeys to appear (21 in the image), its job is finished before a monkey 11 places before it. (monkey 10 in the image).  Verbose output helps to demonstrate the flow of the program. Thread_yield is used to let other monkeys go aside while Dominant does the jobs first.

    3: Missionary problem

2.1: Null pointer dereference
    Test program: run "null" in xv6.  A trap 14 error should appear.
    This was implemented using a single empty page at the beginning of each user program.  When 0 is dereferenced, instead of loading the beginning of user code, an exception occurs due to a pagefault, since the first page is no longer valid.  This is accomplishe by changing the -Ttext entry point flag in the Makefile for user code, changing the entry point in exec() to skip the first PAGESIZE bytes, and modifying the copyuvm() function in vm.c to skip the first page when copying a process's page table.  This leads to the trap 14 error whenever null is dereferenced. (It will also do that for dereferencing any of the first PAGESIZE bytes as well.)
