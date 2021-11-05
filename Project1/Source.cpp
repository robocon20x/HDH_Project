#include"Fat32.h"
#include"NTFS.h"

int ReadSector(HANDLE& device, vector<vector<string>>& sector)//(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    DWORD bytesRead;
    BYTE boot[512];

    wstring disk_name;
    cout << "Nhap ten o dia: ";
    wcin >> disk_name;
    disk_name = L"\\\\.\\" + disk_name + L":";

    LPCWSTR drive = disk_name.c_str();

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("Open drive false at: %u\n", GetLastError());
        return 0;
    }

    SetFilePointer(device, 0, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, boot, 512, &bytesRead, NULL))
    {
        printf("Read bootsector false at: %u\n", GetLastError());
        return 0;
    }
    else
    {
        //Dung de luu bang sector duoi dang vector 2 chieu voi gia tri moi element la string
        sector = to_vector(boot);
        printf("Read bootsector success!\n\n");
        return 1;
    }
}

int main(int argc, char** argv)
{
    vector<vector<string>> sector;
    HANDLE disk = NULL;
    int flag = ReadSector(disk, sector);

    if (!flag)
        return 0;

    // chuyen nhap vi tri vao, so byte de lay tu vec ra chuoi string hex
    string temp = to_hexstr(sector, 2, 5, 5, 0);

    // chuyen chuoi string hex thanh int
    string result = hexstr_tostr(temp);
    
    if (result == "FAT32")
    {
        FAT32 drive(disk, sector);
        cout << "--------------------|DRIVE INFO|--------------------\n";
        drive.readBoot_Sector();
        cout << "\n-----------------------|RDET|-----------------------\n";

        drive.read_RDet(find_table(disk, drive.first_Sector_Of_Data() * 512), 0);
    }
    else
    {
        NTFS drive(disk, sector);
        cout << "--------------------|DRIVE INFO|--------------------\n";
        drive.readBoot_Sector();

        vector<vector<string>> k = find_table(disk, 6291456 * 512);

        print_table(k);
    }

    return 1;
}