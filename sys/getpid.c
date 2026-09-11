/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL getpid_impl();

SYSCALL getpid()
{
	SYSCALL	rv;

	systrace_enter(SYS_GETPID);
	rv = getpid_impl();
	systrace_exit(SYS_GETPID);
	return rv;
}

LOCAL SYSCALL getpid_impl()
{
	return(currpid);
}
