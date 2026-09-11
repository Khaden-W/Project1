/* recvclr.c - recvclr */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  recvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL recvclr_impl();

SYSCALL	recvclr()
{
	SYSCALL	rv;

	systrace_enter(SYS_RECVCLR);
	rv = recvclr_impl();
	systrace_exit(SYS_RECVCLR);
	return rv;
}

LOCAL SYSCALL recvclr_impl()
{
	STATWORD ps;    
	WORD	msg;

	disable(ps);
	if (proctab[currpid].phasmsg) {
		proctab[currpid].phasmsg = 0;
		msg = proctab[currpid].pmsg;
	} else
		msg = OK;
	restore(ps);
	return(msg);
}
