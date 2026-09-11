/* lab0.h - zfunction, printprocstks, printsyscallsummary */

#ifndef _LAB0_H_
#define _LAB0_H_

/* Task 1 - implemented in sys/zfunction.S */

extern	long	zfunction(long param);

/* Task 2 - implemented in sys/printprocstks.c */

extern	void	printprocstks(int priority);

/* Task 3 - implemented in sys/printsyscallsummary.c */

extern	void	syscallsummary_start();
extern	void	syscallsummary_stop();
extern	void	printsyscallsummary();

#endif
