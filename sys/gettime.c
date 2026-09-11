/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>

extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
LOCAL SYSCALL gettime_impl(long *timvar);

SYSCALL	gettime(long *timvar)
{
	SYSCALL	rv;

	systrace_enter(SYS_GETTIME);
	rv = gettime_impl(timvar);
	systrace_exit(SYS_GETTIME);
	return rv;
}

LOCAL SYSCALL gettime_impl(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */

    return OK;
}
