/* printsyscallsummary.c - printsyscallsummary, syscallsummary_start,
			   syscallsummary_stop, systrace_enter,
			   systrace_exit, systrace_newproc */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
#include <systrace.h>

extern	unsigned long	ctr1000;	/* ms since the system started	*/

/* A process may be inside more than one traced call at a time (sleep
 * calls sleep10, kill calls send, ...), so entries are kept on a small
 * per-process stack.
 */
#define	TRACEDEPTH	8

struct	sysstat	{
	unsigned long	count;		/* times the call was invoked	*/
	unsigned long	total;		/* accumulated execution time	*/
};

struct	sysframe {
	int		callid;
	unsigned long	start;		/* value of ctr1000 on entry	*/
	int		counted;	/* entered while tracing was on	*/
};

/* Statistics live outside of proctab so that processes which have
 * already exited still show up in the summary.
 */
static	struct	sysstat		sysstats[NPROC][NSYSCALLS];
static	struct	sysframe	sysframes[NPROC][TRACEDEPTH];
static	int			sysdepth[NPROC];
static	int			systracing = 0;

static	char	*sysnames[NSYSCALLS] = {
	"sys_chprio",		/* SYS_CHPRIO		*/
	"sys_freemem",		/* SYS_FREEMEM		*/
	"sys_getpid",		/* SYS_GETPID		*/
	"sys_getprio",		/* SYS_GETPRIO		*/
	"sys_gettime",		/* SYS_GETTIME		*/
	"sys_kill",		/* SYS_KILL		*/
	"sys_receive",		/* SYS_RECEIVE		*/
	"sys_recvclr",		/* SYS_RECVCLR		*/
	"sys_recvtim",		/* SYS_RECVTIM		*/
	"sys_resume",		/* SYS_RESUME		*/
	"sys_scount",		/* SYS_SCOUNT		*/
	"sys_screate",		/* SYS_SCREATE		*/
	"sys_sdelete",		/* SYS_SDELETE		*/
	"sys_send",		/* SYS_SEND		*/
	"sys_setdev",		/* SYS_SETDEV		*/
	"sys_setnok",		/* SYS_SETNOK		*/
	"sys_signal",		/* SYS_SIGNAL		*/
	"sys_signaln",		/* SYS_SIGNALN		*/
	"sys_sleep",		/* SYS_SLEEP		*/
	"sys_sleep10",		/* SYS_SLEEP10		*/
	"sys_sleep100",		/* SYS_SLEEP100		*/
	"sys_sleep1000",	/* SYS_SLEEP1000	*/
	"sys_sreset",		/* SYS_SRESET		*/
	"sys_stacktrace",	/* SYS_STACKTRACE	*/
	"sys_suspend",		/* SYS_SUSPEND		*/
	"sys_unsleep",		/* SYS_UNSLEEP		*/
	"sys_wait"		/* SYS_WAIT		*/
};

/*------------------------------------------------------------------------
 * syscallsummary_start - clear the statistics and begin tracing
 *------------------------------------------------------------------------
 */
void syscallsummary_start()
{
	STATWORD	ps;
	int		pid, call;

	disable(ps);
	for (pid = 0; pid < NPROC; pid++) {
		for (call = 0; call < NSYSCALLS; call++) {
			sysstats[pid][call].count = 0;
			sysstats[pid][call].total = 0;
		}
	}
	systracing = 1;
	restore(ps);
}

/*------------------------------------------------------------------------
 * syscallsummary_stop - stop tracing system calls
 *------------------------------------------------------------------------
 */
void syscallsummary_stop()
{
	STATWORD	ps;

	disable(ps);
	systracing = 0;
	restore(ps);
}

/*------------------------------------------------------------------------
 * systrace_newproc - forget tracing state left in a reused process slot
 *------------------------------------------------------------------------
 */
void systrace_newproc(int pid)
{
	if (pid >= 0 && pid < NPROC)
		sysdepth[pid] = 0;
}

/*------------------------------------------------------------------------
 * systrace_enter - record the start of a system call for the caller
 *------------------------------------------------------------------------
 */
void systrace_enter(int callid)
{
	STATWORD	ps;
	struct	sysframe *fptr;
	int		pid, depth;

	if (callid < 0 || callid >= NSYSCALLS)
		return;
	disable(ps);
	pid = currpid;
	if (pid < 0 || pid >= NPROC) {
		restore(ps);
		return;
	}

	/* The depth is bumped even when the frame cannot be stored, so
	 * that systrace_exit stays in step with systrace_enter.
	 */
	depth = sysdepth[pid]++;
	if (depth < TRACEDEPTH) {
		fptr = &sysframes[pid][depth];
		fptr->callid = callid;
		fptr->start = ctr1000;
		fptr->counted = systracing;

		/* Counting on entry rather than on exit means a call that
		 * never returns, such as a process killing itself, still
		 * shows up in the summary.
		 */
		if (systracing)
			sysstats[pid][callid].count++;
	}
	restore(ps);
}

/*------------------------------------------------------------------------
 * systrace_exit - charge the elapsed time of a system call to the caller
 *------------------------------------------------------------------------
 */
void systrace_exit(int callid)
{
	STATWORD	ps;
	struct	sysframe *fptr;
	int		pid, depth;

	if (callid < 0 || callid >= NSYSCALLS)
		return;
	disable(ps);
	pid = currpid;
	if (pid < 0 || pid >= NPROC || sysdepth[pid] <= 0) {
		restore(ps);
		return;
	}

	depth = --sysdepth[pid];
	if (depth < TRACEDEPTH) {
		fptr = &sysframes[pid][depth];
		if (fptr->counted && fptr->callid == callid)
			sysstats[pid][callid].total += ctr1000 - fptr->start;
	}
	restore(ps);
}

/*------------------------------------------------------------------------
 * printsyscallsummary - print the traced system calls, grouped by process
 *------------------------------------------------------------------------
 */
void printsyscallsummary()
{
	struct	sysstat	*sptr;
	int		pid, call, used;

	for (pid = NPROC - 1; pid >= 0; pid--) {
		used = 0;
		for (call = 0; call < NSYSCALLS; call++) {
			if (sysstats[pid][call].count != 0) {
				used = 1;
				break;
			}
		}
		if (!used)
			continue;

		kprintf("Process [pid:%d]\n", pid);
		for (call = 0; call < NSYSCALLS; call++) {
			sptr = &sysstats[pid][call];
			if (sptr->count == 0)
				continue;
			kprintf("\tSyscall: %s, count: %d, average execution time: %d (ms)\n",
				sysnames[call], sptr->count,
				sptr->total / sptr->count);
		}
	}
}
