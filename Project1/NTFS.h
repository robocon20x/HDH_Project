#pragma once
#ifndef _NTFS_H_
#define _NTFS_H_

#include"Util.h"

class NTFS
{
private:
	HANDLE device;
	vector<vector<string>> BootSector;
	int bytes_per_sector; // Số bytes trên mỗi sector
	int sectors_per_cluster; // Sc : Số bytes trên mỗi cluster
	int begin_MFT; // cluster bắt đầu MFT
public:
	NTFS(HANDLE disk, vector<vector<string>> sector);

	void readBoot_Sector();

	void read_MFT();
};

#endif