/*
 * syscalls template
 * Sergio Montenegro
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#undef errno
extern int errno;

#include "rodos.h"
#include "vendor-headers.h"


namespace RODOS {

void sp_partition_yield() { }
void FFLUSH()             { }
void enterSleepMode()     { }

extern HAL_UART uart_stdout;

int getCurrentIRQ();
void UART_putchar(HAL_UART* uart, uint8_t c);

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

void abort(void);
void abort(void) { while(1); }
	
int _close(int file);
int _close([[gnu::unused]] int file) {
	return -1;
}

char *__env[1] = { 0 };
char **environ = __env;

int _execve(char *name, char **argv, char **env);
int _execve([[gnu::unused]] char *name,[[gnu::unused]] char **argv,[[gnu::unused]] char **env) {
	errno = ENOMEM;
	return -1;
}

int _fork(void);
int _fork(void) {
	errno = EAGAIN;
	return -1;
}

int _fstat(int file, struct stat *st);
int _fstat([[gnu::unused]] int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _getpid(void);
int _getpid(void) {
	return 1;
}

int _isatty(int file);
int _isatty([[gnu::unused]] int file) {
	return 1;
}

int _kill(int pid, int sig);
int _kill([[gnu::unused]] int pid,[[gnu::unused]] int sig) {
	errno = EINVAL;
	return -1;
}

int _link([[gnu::unused]] char *old,[[gnu::unused]] char *pNew);
int _link([[gnu::unused]] char *old,[[gnu::unused]] char *pNew) {
	errno = EMLINK;
	return -1;
}

int _lseek([[gnu::unused]] int file, int ptr, int dir);
int _lseek([[gnu::unused]] int file, [[gnu::unused]] int ptr, [[gnu::unused]] int dir) {
	return 0;
}

int _open(const char *name, int flags, int mode);
int _open([[gnu::unused]] const char *name, [[gnu::unused]] int flags, [[gnu::unused]] int mode) {
	return -1;
}

int _read([[gnu::unused]] int file, char *ptr, int len);
int _read([[gnu::unused]] int file, [[gnu::unused]] char *ptr, [[gnu::unused]] int len) {
	return 0;
}

int _write([[gnu::unused]] int file, const char *ptr, int len);
int _write([[gnu::unused]] int file, const char *ptr, int len) {
	int todo;

	for (todo = 0; todo < len; todo++) 
	{
		putchar(*ptr);
		ptr++;
	}
	return len;
}

register char * stack_ptr asm ("sp");
extern char _estack;			// see efr32fg1p.ld
extern char _Min_Stack_Size;	// see efr32fg1p.ld
caddr_t _sbrk(int incr);
caddr_t _sbrk(int incr) {
	extern char __heap_start__ asm ("__HeapBase"); /* Defined by the linker.  */
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


int _stat(char *file, struct stat *st) ;
int _stat([[gnu::unused]] char *file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
 
int _times(struct tms *buf);
int _times([[gnu::unused]] struct tms *buf) {
	return -1;
}

int _unlink(char *name);
int _unlink([[gnu::unused]] char *name) {
	errno = ENOENT;
	return -1;
}

int _wait(int *status);
int _wait([[gnu::unused]] int *status) {
	errno = ECHILD;
	return -1;
}


/*
 * reentrant syscalls
 */
long _write_r(void *reent, int fd, const void *buf, size_t cnt);
long _write_r([[gnu::unused]] void *reent, int fd, const void *buf, size_t cnt) {
	return _write(fd, (const char*) buf, (int)cnt);
}

caddr_t _sbrk_r(void *reent, size_t incr);
caddr_t _sbrk_r([[gnu::unused]] void *reent, size_t incr) {
	return _sbrk((int)incr);
}
#endif
/* end newlib stubs */

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
		UART_putchar(&uart_stdout, c);
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



}// end extern "C"

int getCurrentIRQ(){
	return (int)(SCB->ICSR & 0x1FF)-16;
}

extern "C"{
void UndefinedInterrupt_Handler(){
	xprintf("*********ERROR: An interrupt occured, for which no handler was defined. Int. No:%d *************\n",getCurrentIRQ());
	while(1){}
}
}

}
