#include "syscall.h"

int main()
{
	int c;
    	PrintChar('\n');
    	PrintString("IN BANG MA ASCII\n");
	PrintString("Cac ky tu ASCII in ra duoc tu 32 - 126\n");
    	PrintChar('\n');
	PrintString("____________\n");
	PrintString("| Ma  | KT |\n");
	PrintString("|----------|\n");
    	for (c = 32; c < 127; c++) {
		PrintChar(124);
		PrintChar(32);
        	PrintInt(c);
		PrintChar(32);
		if(c < 100)
		{
			PrintChar(32);
		}
	      	PrintChar('|');
		PrintChar(32);        	
		PrintChar(c);
		PrintChar(32);  
		PrintChar(32); 
		PrintChar(124);
        	PrintChar('\n');
    	}
	PrintString("------------\n");
}
