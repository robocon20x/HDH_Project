/////////////////////////////////////////////////
// 	DH KHTN - DHQG TPHCM			/
// 	19120497 Bui Truong Giang		/
// 	19120489 Luu Truong Duong		/
// 	19120485 Nguyen Pham Quang Dung		/
//	19120661 Le Mai Nguyen Thao		/
//	18120306 Le Tho Dat			/
/////////////////////////////////////////////////
#include "syscall.h"


void main()
{
	int pingPID, pongPID;
	PrintString("Ping-Pong test starting...\n\n");
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");
	Join(pingPID);
	Join(pongPID);	
}
