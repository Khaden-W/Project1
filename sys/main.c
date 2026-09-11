/* main.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

int prX;
void halt();

/*------------------------------------------------------------------------
 *  prch  --  child process used by the syscall summary test
 *------------------------------------------------------------------------
 */
void prch(char c)
{
	int i;
	sleep(5);
}

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
int main()
{
	kprintf("Beginning provided tests:\n\n");
	kprintf("Task 1 (zfunction)\n");
	kprintf("0xaabbccdd => 0x%08x\n", zfunction(0xaabbccdd));
	kprintf("\n");

	kprintf("Task 2 (printprocstks)\n");
	printprocstks(10);
	kprintf("\n");

	kprintf("Task 3 (printsyscallsummary)\n");
	syscallsummary_start();
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();

	/* The following shutdown call stops Xinu. If you want to keep it
	 * running, comment it out.
	 */
	shutdown();
	return 0;
}
