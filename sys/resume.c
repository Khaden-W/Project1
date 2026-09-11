/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL resume_impl(int pid);

SYSCALL resume(int pid)
{
	SYSCALL	rv;

	systrace_enter(SYS_RESUME);
	rv = resume_impl(pid);
	systrace_exit(SYS_RESUME);
	return rv;
}

LOCAL SYSCALL resume_impl(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate!=PRSUSP) {
		restore(ps);
		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid, RESCHYES);
	restore(ps);
	return(prio);
}
