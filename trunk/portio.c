
#include <conio.h>

unsigned int getPortB(int port)
{
	return inp(port); // Open Watcom support the function
}

void setPortB(int port, int data)
{
	outp(port, data); // Open Watcom support the function
}
