#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	char name[MAX_LENGTH];
	ReadString(name,MAX_LENGTH);
	PrintString(name);
	return 1;
}
