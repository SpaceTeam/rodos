/*
 * syscalls.cpp
 *
 *  Created on: 13.03.2012
 *      Author: Michael Ruffer
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#undef errno
extern int errno;

#include "hal/hal_uart.h"
#include "platform-parameter.h"
//#include "scheduler.h"
#include "debug.h"
#include "hw_specific.h"
#include "yprintf.h"
#include "stm32f4xx_conf.h"
#include "misc-rodos-funcs.h"

namespace RODOS {

#define T2H_SYSCALL_INIT_ARGV           14

extern HAL_UART uart_stdout;

int getCurrentIRQ();
void UART_putc_interrupt(HAL_UART* uart, uint8_t c);

extern "C" {

/* newlib stubs
 * The following function are necessary for newlib. It is very important
 * that there are valid implementations for _sbrk_r and sbrk. These
 * functions are used for memory allocation.
 * For more informations about syscalls visit:
 * http://sourceware.org/newlib/libc.html#Syscalls
 * http://sourceware.org/newlib/libc.html#Reentrant-Syscalls
 *
 */
#if 1
int _close(int file);
int _close(int file) {
	return -1;
}

char *__env[1] = { 0 };
char **environ = __env;

int _execve(char *name, char **argv, char **env);
int _execve(char *name, char **argv, char **env) {
	errno = ENOMEM;
	return -1;
}

int _fork(void);
int _fork(void) {
	errno = EAGAIN;
	return -1;
}

int _fstat(int file, struct stat *st);
int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _getpid(void);
int _getpid(void) {
	return 1;
}

int _isatty(int file);
int _isatty(int file) {
	return 1;
}

int _kill(int pid, int sig);
int _kill(int pid, int sig) {
	errno = EINVAL;
	return -1;
}

int _link(char *old, char *pNew);
int _link(char *old, char *pNew) {
	errno = EMLINK;
	return -1;
}

int _lseek(int file, int ptr, int dir);
int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _open(const char *name, int flags, int mode);
int _open(const char *name, int flags, int mode) {
	return -1;
}

int _read(int file, char *ptr, int len);
int _read(int file, char *ptr, int len) {
	return 0;
}

register char * stack_ptr asm ("sp");
extern char _estack;			// see stm32_flash.ld
extern char _Min_Stack_Size;	// see stm32_flash.ld
caddr_t _sbrk(int incr);
caddr_t _sbrk(int incr) {
	extern char __heap_start__ asm ("__heap_start__"); /* Defined by the linker.  */
	static char * heap_end;
	char * prev_heap_end;

	if (heap_end == NULL)
		heap_end = &__heap_start__;

	prev_heap_end = heap_end;

	// Test, ob stack und heap kollidieren funktioniert in urspruenglicher
	// implementierung nicht, weil
	// RODOS den Stack verwaltet und der Stackpointer adressmaessig
	// irgendwo im XMALLOC-Puffer liegt. Der Puffer liegt z.Z. an einer
	// Adresse die immer kleiner als heap_end ist -> vgl. Linkerscript!!!
	// Der Hauptstack (MSP) wird (nachdem RODOS gestartet ist) nur noch
	// von ISRs & dem Scheduler verwendet.
	//if (heap_end + incr > stack_ptr) {
	if (heap_end + incr > &_estack-_Min_Stack_Size) {
		/* Some of the libstdc++-v3 tests rely upon detecting
		 out of memory errors, so do not abort here.  */
#if 0
		extern void abort (void);

		_write (1, "_sbrk: Heap and stack collision\n", 32);

		abort ();
#else
		asm("bkpt;");
		errno = ENOMEM;
		return (caddr_t) -1;
#endif
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

/*
 int stat(char *file, struct stat *st) {
 st->st_mode = S_IFCHR;
 return 0;
 }
 */
int _times(struct tms *buf);
int _times(struct tms *buf) {
	return -1;
}

int _unlink(char *name);
int _unlink(char *name) {
	errno = ENOENT;
	return -1;
}

int _wait(int *status);
int _wait(int *status) {
	errno = ECHILD;
	return -1;
}

int putchar(int ic);

int _write(int file, char *ptr, int len);
int _write(int file, char *ptr, int len) {
	int todo;

	for (todo = 0; todo < len; todo++) {

		putchar(*ptr);
		ptr++;
	}
	return len;
}

/*
 * reentrant syscalls
 */
long _write_r(void *reent, int fd, const void *buf, size_t cnt);
long _write_r(void *reent, int fd, const void *buf, size_t cnt) {
	return _write(fd, (char*) buf, cnt);
}

caddr_t _sbrk_r(void *reent, size_t incr);
caddr_t _sbrk_r(void *reent, size_t incr) {
	return _sbrk(incr);
}
#endif
/* end newlib stubs */

//puts chars
int putchar(int ic) {
	char c = (char) (ic & 0xff);

	if (c == '\n') {
		putchar('\r');
	}

	//uart_stdout.putcharNoWait(c);

	if(getCurrentIRQ() == -16){//Thread Mode
		while(uart_stdout.putcharNoWait(c) == -1){
			if(isSchedulerRunning()){
				uart_stdout.suspendUntilWriteFinished();
			}else{
				while (!uart_stdout.isWriteFinished()) {}
			}

		}
	}else{// in interrupt, write to UART register directly bypassing HAL_UART buffers
		UART_putc_interrupt(&uart_stdout, c);
	}

	return c;
}

//puts strings
int puts(const char * str);
int puts(const char * str) {
	const char *c;
	c = str;
	while (*c) {
		putchar(*c++);
	}
	return 0;
}

void abort(void);
void abort(void) { while(1); }

int sched_yield();
int sched_yield();
} // end extern "C"

void sp_partition_yield() {}


void FFLUSH() { }

int getCurrentIRQ(){
	return (SCB->ICSR & 0x1FF)-16;
}

extern "C"{
void UndefinedInterrupt_Handler(){
	xprintf("*********ERROR: An interrupt occured, for which no handler was defined. Int. No:%d *************\n",getCurrentIRQ());
	while(1){}
}
}

}

