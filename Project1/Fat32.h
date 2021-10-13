#ifndef _FAT_32_H_
#define _FAT_32_H_

#include"Util.h"

class fat32
{
private:
	HANDLE device;
	vector<vector<string>> BootSector;
	int bytes_per_sector; // Số bytes trên mỗi sector
	int sectors_per_cluster; // Sc : Số bytes trên mỗi cluster
	int sectors_of_boot; // SB : Số sector trước vùng FAT
	int numbers_of_fats; // NF : Số bảng FAT
	int sectors_per_fat; //SF : Số sector trên 1 bảng FAT
public:
	fat32(HANDLE disk, vector<vector<string>> sector);
	void readBootsector();
	void readRDET();
};

#endif // !_FAT_32_H_
