#include <windows.h>
#include<iostream>
#include<vector>
#include <stdio.h>
#include<iomanip>
using namespace std;
int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("\nSuccess!\n");
        return 2;
    }
}
int main(int argc, char** argv)
{

    BYTE sector[512];
    int flag = ReadSector(L"\\\\.\\D:", 0, sector);

    vector<vector<BYTE>> v;
    if (2 == flag) {
        for (int i = 0; i < 512; i++) {
            if (0 == i % 16) {
                cout << "\n";
            }
            cout  <<setfill('0') << setw(2)<< uppercase << hex << static_cast<unsigned>(sector[i]) <<" ";
        }
    }

    
    return 0;
}