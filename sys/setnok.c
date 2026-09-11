/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL setnok_impl(int nok, int pid);

SYSCALL	setnok(int nok, int pid)
{
	SYSCALL	rv;

	systrace_enter(SYS_SETNOK);
	rv = setnok_impl(nok, pid);
	systrace_exit(SYS_SETNOK);
	return rv;
}

LOCAL SYSCALL setnok_impl(int nok, int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
	return(OK);
}
