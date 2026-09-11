/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL signaln_impl(int sem, int count);

SYSCALL signaln(int sem, int count)
{
	SYSCALL	rv;

	systrace_enter(SYS_SIGNALN);
	rv = signaln_impl(sem, count);
	systrace_exit(SYS_SIGNALN);
	return rv;
}

LOCAL SYSCALL signaln_impl(int sem, int count)
{
	STATWORD ps;    
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);
	return(OK);
}
