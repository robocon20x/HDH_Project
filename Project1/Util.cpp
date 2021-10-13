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

vector<vector<string>> combine_table(vector<vector<string>> v1, vector<vector<string>> v2) {
    /*int begin = v1.size();*/
    for (int i = 0; i < v2.size(); i++) {
        v1.push_back(v2[i]);
    }
    v2.clear();
    return v1;
}

bool is_end(vector<vector<string>> vec) {
    for (int i = 0; i < vec[0].size(); i++) {
        if (vec[vec.size()-1][i] != "00") return false;
    }
    return true;
}

vector<vector<string>> find_table(HANDLE device, int readPont)
{
    DWORD bytesRead;
    BYTE sector[512];

    vector<vector<string>> result;
    SetFilePointer(device, readPont, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("Find sector false at: %u\n", GetLastError());
        return result;
    }

    result = to_vector(sector);

    if (is_end(result)) {
        return result;
    }

    vector<vector<string>> next = find_table(device, readPont + 512);
    return combine_table(result, next);
};

void print_table(vector<vector<string>> vec) {
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[i].size(); j++) {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
    return;
}