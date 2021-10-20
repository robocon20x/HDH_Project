#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_

#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<vector>
#include<windows.h>
#include<iomanip>
#include<sstream>
#include <istream>

using namespace std;

//chuyen bang sector thanh vector 2d string
vector<vector<string>> to_vector(BYTE sector[512]);

//doc string hex tu bang sector
string to_hexstr(vector<vector<string>> sector, int col, int row, int num_byte, bool flag);

//chuyen tu chuoi hexstr sang chuoi string
string hexstr_tostr(string hex);

//chuyen tu hexstr sang int
int hexstr_to_int(string hexstr);

//chuyen tu chuoi hex sang chuoi bin
string to_binstr(int number);

bool is_End(vector<vector<string>> vec);
vector<vector<string>> combine_Table(vector<vector<string>> v1, vector<vector<string>> v2);
vector<vector<string>> find_Table(HANDLE device, int readPont);

void print_Table(vector<vector<string>> vec);

//in tab ra man hinh
void print_Tab(int k);

char* convert_string(string s);
string to_string(char* s);

#endif // !_UTIL_H_
