/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL scount_impl(int sem);

SYSCALL scount(int sem)
{
	SYSCALL	rv;

	systrace_enter(SYS_SCOUNT);
	rv = scount_impl(sem);
	systrace_exit(SYS_SCOUNT);
	return rv;
}

LOCAL SYSCALL scount_impl(int sem)
{
extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE)
		return(SYSERR);
	return(semaph[sem].semcnt);
}
