/* ----------------------------------------

         Test of all Nachos system calls
	 and library functions. New process
	 lauched by the exec system call.

	 Simply writes a message ...

  ---------------------------------------- */

// For Nachos system calls
#include "userlib/syscall.h"

// For nachos library functions
#include "userlib/libnachos.h"


void main(void)
{
  n_printf("   New process launched normally, trying to exit\n");
  
  Exit(7);	

}
