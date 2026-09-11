/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL setdev_impl(int pid, int dev1, int dev2);

SYSCALL	setdev(int pid, int dev1, int dev2)
{
	SYSCALL	rv;

	systrace_enter(SYS_SETDEV);
	rv = setdev_impl(pid, dev1, dev2);
	systrace_exit(SYS_SETDEV);
	return rv;
}

LOCAL SYSCALL setdev_impl(int pid, int dev1, int dev2)
{
	short	*nxtdev;

	if (isbadpid(pid))
		return(SYSERR);
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	return(OK);
}
