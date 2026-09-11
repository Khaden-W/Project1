/* receive.c - receive */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  receive  -  wait for a message and return it
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL receive_impl();

SYSCALL	receive()
{
	SYSCALL	rv;

	systrace_enter(SYS_RECEIVE);
	rv = receive_impl();
	systrace_exit(SYS_RECEIVE);
	return rv;
}

LOCAL SYSCALL receive_impl()
{
	STATWORD ps;    
	struct	pentry	*pptr;
	WORD	msg;

	disable(ps);
	pptr = &proctab[currpid];
	if ( !pptr->phasmsg ) {		/* if no message, wait for one	*/
		pptr->pstate = PRRECV;
		resched();
	}
	msg = pptr->pmsg;		/* retrieve message		*/
	pptr->phasmsg = FALSE;
	restore(ps);
	return(msg);
}
