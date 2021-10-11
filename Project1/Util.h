#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_

#include<iostream>
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

#endif // !_UTIL_H_
