/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL getprio_impl(int pid);

SYSCALL getprio(int pid)
{
	SYSCALL	rv;

	systrace_enter(SYS_GETPRIO);
	rv = getprio_impl(pid);
	systrace_exit(SYS_GETPRIO);
	return rv;
}

LOCAL SYSCALL getprio_impl(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	restore(ps);
	return(pptr->pprio);
}
