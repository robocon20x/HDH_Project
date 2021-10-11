#include <windows.h>
#include<iostream>
#include<vector>
#include <stdio.h>
#include<sstream>
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
        printf("\nSuccess!\n");
        return 1;
    }
}
//chuyen bang sector thanh vector 2d string
vector<vector<string>> to_vector(BYTE sector[512]){
    vector<vector<string>> result;
    int i = 0;
   
    for (int i = 0; i < 32; i++) {
        vector<string> temp;
        for (int j = 0; j < 16; j++) {
            stringstream ss;
            ss << setfill('0') << setw(2) << uppercase << hex << static_cast<unsigned>(sector[i * 16 + j]);
            temp.push_back(ss.str());
        }
        result.push_back(temp);
    }       
    return result;
}

//doc string hex tu bang sector
string to_hexstr(vector<vector<string>> sector, int col,int row, int num_byte) {
    string result;
    for (int i = num_byte-1; i >= 0; i--) {
        result += sector[row][col + i];
    }
    return result;
}

//chuyen tu chuyen hexstr sang chuoi string
string hexstr_tostr(string hex) {
    string ascii = "";
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        string part = hex.substr(i, 2);
        char ch = stoul(part, nullptr, 16);
        ascii += ch;
    }
    return ascii;
}
//chuyen tu hexstr sang int
int hexstr_to_int(string hexstr) {
    /*stringstream ss;
    unsigned int x;
    ss << hex << hexstr;
    ss >> x;
    return x;*/
    int result;
    char* offset;
    string real_hex = "0x" + hexstr;
    result = strtol(real_hex.c_str(), &offset, 0);
    return result;
}


int main(int argc, char** argv)
{

    BYTE sector[512];
    int flag = ReadSector(L"\\\\.\\D:", 0, sector);
    vector<vector<string>> vec;

    //Dung de luu bang sector duoi dang vector 2 chieu voi gia tri moi element la string
    vec = to_vector(sector);

    // chuyen nhap vi tri vao, so byte de lay tu vec ra chuoi string hex
    string temp = to_hexstr(vec, 11, 0, 2);
    // chuyen chuoi string hex thanh int
    int result = hexstr_to_int(temp);
    cout  << result;

    
    return 0;
}