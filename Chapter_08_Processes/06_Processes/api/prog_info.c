/*! Program info */

#include <api/prog_info.h>

#include <api/pthread.h>
#include <api/malloc.h>

/* symbols from user.ld */
extern char user_code, user_end;

extern int PROG_START_FUNC ( char *args[] );
extern char PROG_HELP[];

prog_info_t pi =
{
	.zero =		{ 0, 0, 0, 0 },
	.init = 	prog_init,
	.entry =	PROG_START_FUNC,
	.param =	NULL,
	.exit =		pthread_exit,
	.prio =		THR_DEFAULT_PRIO,

	.heap_size =	HEAP_SIZE,
	.stack_size =	STACK_SIZE,
	.thread_stack =	THREAD_STACK_SIZE,

	.help_msg =	PROG_HELP,

	.start_adr =	&user_code,
	.heap =		NULL,
	.stack =	NULL,
	.end_adr =	&user_end,

	.mpool =	NULL,
};

int stdio_init (); /* implemented in stdio.c */

/*! Initialize process environment */
void prog_init ( void *args )
{
	/* open stdin & stdout */
	stdio_init ();

	/* initialize dynamic memory */
	pi.mpool = mem_init ( pi.heap, (size_t) pi.stack - (size_t) pi.heap );

	/* call starting function */
	( (void (*) ( void * ) ) pi.entry ) ( args );

	pthread_exit ( NULL );
}
