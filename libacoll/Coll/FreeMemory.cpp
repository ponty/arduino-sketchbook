
#include "WProgram.h"
#include "FreeMemory.h"

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;
extern unsigned int __data_start;

/// return available RAM
int freeMemory1()
{
	/// copy from http://arduino.cc/playground/Code/AvailableMemory
	int free_memory;

	if ((int) __brkval == 0)
		free_memory = ((int) &free_memory) - ((int) &__bss_end);
	else
		free_memory = ((int) &free_memory) - ((int) __brkval);

	return free_memory;
}

/// return available RAM
/*! This function places the current value of the heap and stack pointers in the
 * variables. You can call it from any place in your code and save the data for
 * outputting or displaying later. This allows you to check at different parts of
 * your program flow.
 * The stack pointer starts at the top of RAM and grows downwards. The heap pointer
 * starts just above the static variables etc. and grows upwards. SP should always
 * be larger than HP or you'll be in big trouble! The smaller the gap, the more
 * careful you need to be. Julian Gall 6-Feb-2009.
 */
int freeMemory2()
{
	/// copy from http://arduino.cc/playground/Code/AvailableMemory
	uint8_t * heapptr, *stackptr;

	stackptr = (uint8_t *) malloc(4); // use stackptr temporarily
	heapptr = stackptr; // save value of heap pointer
	free(stackptr); // free up the memory again (sets stackptr to 0)
	stackptr = (uint8_t *) (SP); // save value of stack pointer
	return (int) (stackptr - heapptr);
}

/// return available RAM

/// this function will return the number of bytes currently free in RAM
/// written by David A. Mellis
/// based on code by Rob Faludi http://www.faludi.com
int freeMemory3()
{
	/// copy from http://arduino.cc/playground/Code/AvailableMemory
	int size = (int) RAMEND - (int) &__data_start;
	byte *buf;

	while ((buf = (byte *) malloc(--size)) == NULL)
		;

	free(buf);

	return size;
}

