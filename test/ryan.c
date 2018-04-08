#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int main()
{
	char*	send="!Hola, soy burritos!";
	char	recv[100];
	TtySend(send);
	n_printf(send);
	TtyReceive(recv, 99);
	n_printf(recv);

	return 0;
}
