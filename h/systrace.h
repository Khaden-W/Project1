/* systrace.h - system call tracing hooks used by printsyscallsummary */

#ifndef _SYSTRACE_H_
#define _SYSTRACE_H_

/* Identifiers for the 27 traced system calls.  They are kept in
 * alphabetical order so the summary comes out sorted by call name.
 */

#define	SYS_CHPRIO	 0
#define	SYS_FREEMEM	 1
#define	SYS_GETPID	 2
#define	SYS_GETPRIO	 3
#define	SYS_GETTIME	 4
#define	SYS_KILL	 5
#define	SYS_RECEIVE	 6
#define	SYS_RECVCLR	 7
#define	SYS_RECVTIM	 8
#define	SYS_RESUME	 9
#define	SYS_SCOUNT	10
#define	SYS_SCREATE	11
#define	SYS_SDELETE	12
#define	SYS_SEND	13
#define	SYS_SETDEV	14
#define	SYS_SETNOK	15
#define	SYS_SIGNAL	16
#define	SYS_SIGNALN	17
#define	SYS_SLEEP	18
#define	SYS_SLEEP10	19
#define	SYS_SLEEP100	20
#define	SYS_SLEEP1000	21
#define	SYS_SRESET	22
#define	SYS_STACKTRACE	23
#define	SYS_SUSPEND	24
#define	SYS_UNSLEEP	25
#define	SYS_WAIT	26

#define	NSYSCALLS	27

/* Called on entry to and exit from each traced system call */

extern	void	systrace_enter(int callid);
extern	void	systrace_exit(int callid);

/* Drops tracing state left behind in a reused process slot; called
 * from create().
 */
extern	void	systrace_newproc(int pid);

#endif
