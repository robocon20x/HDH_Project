// exception.cc 
/////////////////////////////////////////////////
// 	DH KHTN - DHQG TPHCM			/
// 	19120497 Bui Truong Giang		/
// 	19120489 Luu Truong Duong		/
// 	19120485 Nguyen Pham Quang Dung		/
//	19120661 Le Mai Ngueyn Thao		/
//	18120306 Le Tho Dat			/
/////////////////////////////////////////////////


//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// Change the system's Program counter register back 4 bytes to continue loading instruction
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input:  User space address(int) - limit of buffer(int)
// Output: Buffer(char*)
// Purpose: Copy buffer from User memory space to System memory space
char* User2System(int virtAddr, int limit)
{
	int i; //index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
		
	memset(kernelBuf, 0, limit + 1);
	
	//printf("\n Filename u2s:");
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}


// Input: User space address(int) - Limit of buffer(int) - Buffer(char*)
// Output: Number of bytes copied(int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

//Function to handle runtime Exception and system call
void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	switch (which)
	{
		case NoException: //Exception runtime in system.h
			return;

		case PageFaultException: 
			DEBUG('a', "\n No valid translation found"); //Notification in debug
			printf("\n\n No valid translation found"); //Notification in runtime
			interrupt->Halt();
			break;

		case ReadOnlyException: 
			DEBUG('a', "\n Write attempted to page marked read-only");
			printf("\n\n Write attempted to page marked read-only");
			interrupt->Halt();
			break;

		case BusErrorException: 
			DEBUG('a', "\n Translation resulted invalid physical address");
			printf("\n\n Translation resulted invalid physical address");
			interrupt->Halt();
			break;

		case AddressErrorException: 
			DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
			printf("\n\n Unaligned reference or one that was beyond the end of the address space");
			interrupt->Halt();
			break;

		case OverflowException: 
			DEBUG('a', "\nInteger overflow in add or sub.");
			printf("\n\n Integer overflow in add or sub.");
			interrupt->Halt();
			break;

		case IllegalInstrException: 
			DEBUG('a', "\n Unimplemented or reserved instr.");
			printf("\n\n Unimplemented or reserved instr.");
			interrupt->Halt();
			break;

		case NumExceptionTypes:
			DEBUG('a', "\n Number exception types");
			printf("\n\n Number exception types");
			interrupt->Halt();
			break;

		case SyscallException: //Exception systemcall in syscall.h
			switch (type)
			{
				case SC_Halt:
					DEBUG('a', "\nShutdown, initiated by user program.");
					printf("\nShutdown, initiated by user program. ");
					interrupt->Halt();

				case SC_ReadInt:
				{
					// Input: None
		    			// Output: The integer read from console.
		    			// Purpose: Read a integer from console.
		    			char* buffer;
		    			int MAX_BUFFER = 255;
		    			buffer = new char[MAX_BUFFER + 1];

					//read buffer up to MAX_BUFFER characters, return the number of digits read
		    			int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER);
		    			int number = 0; // store a result

		    			/* Conversion from buffer to integer int */

		    			// Determine negative or positive number                       
		    			bool isNegative = false; // Hypothesis is positive
		    			int firstNumIndex = 0;
		    			int lastNumIndex = 0;
				    	if(buffer[0] == '-')
				    	{
						isNegative = true;
						firstNumIndex = 1;
						lastNumIndex = 1;                        			   		
				    	}
				    
				    	// Check the validity of integer buffer
					for(int i = firstNumIndex; i < numbytes; i++)					
				    	{
						if(buffer[i] == '.') /// 125.0000000 is a integer
						{
					    		int j = i + 1;
					    		for(; j < numbytes; j++)
					    		{
								// Not valid
								if(buffer[j] != '0')
								{
						    			printf("\n\n The integer number is not valid");
						    			DEBUG('a', "\n The integer number is not valid");
						    			machine->WriteRegister(2, 0);
						    			IncreasePC();
						    			delete buffer;
						    			return;
								}
					    		}
					    		// If valid them update lastNumIndex
					    		lastNumIndex = i - 1;				
					    		break;                           
						}
						else if(buffer[i] < '0' || buffer[i] > '9')
						{
					    		printf("\n\n The integer number is not valid");
					    		DEBUG('a', "\n The integer number is not valid");
					    		machine->WriteRegister(2, 0);
					    		IncreasePC();
					    		delete buffer;
					    		return;
						}
						lastNumIndex = i;    
				    	}			
				    
				    	// If is integer, convert string to int
				    	for(int i = firstNumIndex; i<= lastNumIndex; i++)
				    	{
						number = number * 10 + (int)(buffer[i] - 48); 
				    	}
				    
				    	// If negative so * -1;
				    	if(isNegative)
				    	{
						number = number * -1;
				    	}
				    	machine->WriteRegister(2, number);
				    	IncreasePC();
				    	delete buffer;
				    	return;		
				}

				case SC_PrintInt:
				{	
				    // Input: an integer
				    // Output: none
				    // Purpose: Show the integer in console
				    int number = machine->ReadRegister(4);
				    if(number == 0)
				    {
				        gSynchConsole->Write("0", 1); // Show integer 0
				        IncreasePC();
				        return;    
				    }
				    
				    /*Process convert integer to string to show in console*/
				    bool isNegative = false; // what if is possitive
				    int numberOfNum = 0; // variable to store number of digits
				    int firstNumIndex = 0; 
			
				    if(number < 0)
				    {
				        isNegative = true;
				        number = number * -1; // To convert negative to possitive to count number of digits
				        firstNumIndex = 1; 
				    } 	
				    
				    int t_number = number; // temporary variable for number
				    while(t_number)
				    {
				        numberOfNum++;
				        t_number /= 10;
				    }
		    
				    //Create string buffer to write in console
				    char* buffer;
				    int MAX_BUFFER = 255;
				    buffer = new char[MAX_BUFFER + 1];
				    for(int i = firstNumIndex + numberOfNum - 1; i >= firstNumIndex; i--)
				    {
				        buffer[i] = (char)((number % 10) + 48);
				        number /= 10;
				    }
				    if(isNegative)
				    {
				        buffer[0] = '-';
					buffer[numberOfNum + 1] = 0;
				        gSynchConsole->Write(buffer, numberOfNum + 1);
				        delete buffer;
				        IncreasePC();
				        return;
				    }
				    buffer[numberOfNum] = 0;	
				    gSynchConsole->Write(buffer, numberOfNum);
				    delete buffer;
				    IncreasePC();
				    return;        			
					
				}

				case SC_ReadChar:
				{

					//Input: None
					//Output: Only 1 character (char)
					//Purpose: Read 1 character input by user
					int maxBytes = 255;
					char* buffer = new char[255];
					int numBytes = gSynchConsole->Read(buffer, maxBytes);

					if(numBytes > 1) //If more than 1 character is input so invalid
					{
						printf("Chi duoc nhap duy nhat 1 ky tu!");
						DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
						machine->WriteRegister(2, 0);
					}
					else if(numBytes == 0) //Hollow
					{
						printf("Ky tu rong!");
						DEBUG('a', "\nERROR: Ky tu rong!");
						machine->WriteRegister(2, 0);
					}
					else
					{
						//Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
						char c = buffer[0];
						machine->WriteRegister(2, c);
					}

					delete buffer;
					IncreasePC();
					return;
					break;
				}

				case SC_PrintChar:
				{
					// Input: Character(char)
					// Output: Character(char)
					// Purpose: Output a character in console
					char c = (char)machine->ReadRegister(4); // Write a character to register r4
					gSynchConsole->Write(&c, 1); // Write c, 1 byte
					IncreasePC();
					return;
					break;

				}

				case SC_ReadString:
				{
					// Input: Buffer(char*), maximum length of input string(int)
					// Output: None
					// Purpose: read a string with two arguments are buffer and maximum length
					int virtAddr, length;
					char* buffer;
					virtAddr = machine->ReadRegister(4); // Take address of argument buffer from register 4
					length = machine->ReadRegister(5); // Take maximum length of input string from register 5
					buffer = User2System(virtAddr, length); // Copy string from User Space to System Space
					gSynchConsole->Read(buffer, length); // Call function Read of SynchConsole to read string
					System2User(virtAddr, length, buffer); // Copy string from System Space to User Space
					delete buffer; 
					IncreasePC(); // Increase Program Counter 
					return;
					break;
				}

				case SC_PrintString:
				{
					// Input: Buffer(char*)
					// Output: String read in buffer(char*)
					// Purpose: Print a string from argument buffer to console
					int virtAddr;
					char* buffer;
					virtAddr = machine->ReadRegister(4); // Take address of argument buffer from register 4
					buffer = User2System(virtAddr, 255); // Copy string from User Space to System Space with buffer langth 255 character
					int length = 0;
					while (buffer[length] != 0) length++; // Count length of buffer
					gSynchConsole->Write(buffer, length + 1); // Call function Write of SynchConsole to write string
					delete buffer; 
					IncreasePC(); // Increase Program Counter 
					return;
					break;
				}


				default:
					break;
			}
			IncreasePC();
	}
}
