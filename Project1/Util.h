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
using namespace std;

//chuyen bang sector thanh vector 2d string
vector<vector<string>> to_vector(BYTE sector[512]);

//doc string hex tu bang sector
string to_hexstr(vector<vector<string>> sector, int col, int row, int num_byte, bool flag);

//chuyen tu chuyen hexstr sang chuoi string
string hexstr_tostr(string hex);

//chuyen tu hexstr sang int
int hexstr_to_int(string hexstr);


//ket hop 2 vector
vector<vector<string>> combine_table(vector<vector<string>> v1, vector<vector<string>> v2);

//kiem tra vector nay co chua gia tri khong
bool is_end(vector<vector<string>> vec);

//in bang
void print_table(vector<vector<string>> vec);
#endif // !_UTIL_H_
