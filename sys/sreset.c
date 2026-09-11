/* sreset.c - sreset */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  sreset  --  reset the count and queue of a semaphore
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL sreset_impl(int sem, int count);

SYSCALL sreset(int sem, int count)
{
	SYSCALL	rv;

	systrace_enter(SYS_SRESET);
	rv = sreset_impl(sem, count);
	systrace_exit(SYS_SRESET);
	return rv;
}

LOCAL SYSCALL sreset_impl(int sem, int count)
{
	STATWORD ps;    
	struct	sentry	*sptr;
	int	pid;
	int	slist;

	disable(ps);
	if (isbadsem(sem) || count<0 || semaph[sem].sstate==SFREE) {
		restore(ps);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	slist = sptr->sqhead;
	while ((pid=getfirst(slist)) != EMPTY)
		ready(pid,RESCHNO);
	sptr->semcnt = count;
	resched();
	restore(ps);
	return(OK);
}
