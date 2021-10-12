#include"Fat32.h"

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[32])
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
        return 0;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
        return 0;
    }
    else
    {
        printf("Success!\n\n");
        return 1;
    }
}

int main(int argc, char** argv)
{
    BYTE sector[512];
    int flag = ReadSector(L"\\\\.\\F:", 0, sector);
    
    //Dung de luu bang sector duoi dang vector 2 chieu voi gia tri moi element la string
    vector<vector<string>> vec = to_vector(sector);

    // chuyen nhap vi tri vao, so byte de lay tu vec ra chuoi string hex
    string temp = to_hexstr(vec, 2, 5, 5, 0);

    // chuyen chuoi string hex thanh int
    string result = hexstr_tostr(temp);
    
    if (result == "FAT32")
    {
        fat32 drive(L"\\\\.\\F:", vec);
        cout << "Drive info: " << endl;
        drive.readBootsector();
        drive.readRDET();
    }

    
    return 0;
}