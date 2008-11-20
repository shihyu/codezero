/*
 * Clone test.
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sched.h>
#include <errno.h>

int clone_global = 0;

int my_thread_func(void *arg)
{
	printf("Cloned child %d running...\n", getpid());
	for (int i = 0; i < 25; i++)
		clone_global++;
	printf("Cloned child exiting with global increased to: %d. (Should be just about 100 at final)\n", clone_global);
	_exit(0);
}

int clonetest(void)
{
	pid_t childid;
	void *child_stack;

	/* Parent loops and calls clone() to clone new threads. Children don't come back from the clone() call */
	for (int i = 0; i < 4; i++) {
		if ((child_stack = mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_GROWSDOWN, 0, 0)) == MAP_FAILED) {
			printf("MMAP failed.\n");
			_exit(1);
		} else {
			printf("Mapped area starting at %p\n", child_stack);
		}
		((int *)child_stack)[-1] = 5; /* Test mapped area */

		printf("Cloning...\n");

		if ((childid = clone(my_thread_func, child_stack,
		     CLONE_PARENT | CLONE_FS | CLONE_VM | CLONE_THREAD | CLONE_SIGHAND, 0)) < 0) {
			perror("CLONE failed.\n");
		} else {
			printf("Cloned a new thread with child pid %d\n", childid);
		}
	}
	_exit(0);
	return 0;
}




