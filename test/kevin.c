#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int main()
{
	char*	send="!Hola bujito! ?Que Tal?";
	char	recv[100];
	TtyReceive(recv, 99);
	n_printf(recv);
	TtySend(send);
	
	n_printf(send);

	return 0;
}
