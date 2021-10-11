#pragma once
#include"Util.h"

//chuyen bang sector thanh vector 2d string
vector<vector<string>> to_vector(BYTE sector[512]) {
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
string to_hexstr(vector<vector<string>> sector, int col, int row, int num_byte, bool flag) {
    string result;
    if (flag)
        for (int i = num_byte - 1; i >= 0; i--)
            result += sector[row][col + i];
    else
        for (int i = 0; i < num_byte; i++)
            result += sector[row][col + i];
    
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
int hexstr_to_int(string hexstr) 
{
    int result;
    char* offset;
    string real_hex = "0x" + hexstr;
    result = strtol(real_hex.c_str(), &offset, 0);
    return result;
}
