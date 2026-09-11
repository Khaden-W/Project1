/* printprocstks.c - printprocstks */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * currstkptr - read the stack pointer of the running process
 *------------------------------------------------------------------------
 */
static unsigned long currstkptr()
{
	unsigned long	esp;

	__asm__ __volatile__("movl %%esp, %0" : "=r"(esp));
	return esp;
}

/*------------------------------------------------------------------------
 * printprocstks - print stack information for every existing process
 *		   whose priority is greater than the given one
 *------------------------------------------------------------------------
 */
void printprocstks(int priority)
{
	struct	pentry	*pptr;
	int		pid;
	unsigned long	esp;

	/* proctab holds the saved stack pointer, which is stale for the
	 * process we are running on, so read %esp directly for that one.
	 */
	esp = currstkptr();

	kprintf("# Filter: %d\n", priority);
	for (pid = 0; pid < NPROC; pid++) {
		pptr = &proctab[pid];
		if (pptr->pstate == PRFREE || pptr->pprio <= priority)
			continue;
		kprintf("Process [%s]\n", pptr->pname);
		kprintf("\tpid: %d\n", pid);
		kprintf("\tpriority: %d\n", pptr->pprio);
		kprintf("\tbase: 0x%08x\n", (unsigned long)pptr->pbase);
		kprintf("\tlimit: 0x%08x\n", (unsigned long)pptr->plimit);
		kprintf("\tlen: %d\n", pptr->pstklen);
		kprintf("\tpointer: 0x%08x\n",
			pid == currpid ? esp : (unsigned long)pptr->pesp);
	}
}
