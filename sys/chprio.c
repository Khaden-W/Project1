/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL chprio_impl(int pid, int newprio);

SYSCALL chprio(int pid, int newprio)
{
	SYSCALL	rv;

	systrace_enter(SYS_CHPRIO);
	rv = chprio_impl(pid, newprio);
	systrace_exit(SYS_CHPRIO);
	return rv;
}

LOCAL SYSCALL chprio_impl(int pid, int newprio)
{
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
	return(newprio);
}
