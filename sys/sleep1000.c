/* sleep1000.c - sleep1000 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL sleep1000_impl(int n);

SYSCALL sleep1000(int n)
{
	SYSCALL	rv;

	systrace_enter(SYS_SLEEP1000);
	rv = sleep1000_impl(n);
	systrace_exit(SYS_SLEEP1000);
	return rv;
}

LOCAL SYSCALL sleep1000_impl(int n)
{
	STATWORD ps;    

	if (n < 0  || clkruns==0)
	         return(SYSERR);
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	return(OK);
}
