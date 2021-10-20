#ifndef _FAT_32_H_
#define _FAT_32_H_

#include"Util.h"

class FAT32
{
private:
	HANDLE device;
	vector<vector<string>> BootSector;
	int bytes_per_sector; // Số bytes trên mỗi sector
	int sectors_per_cluster; // Sc : Số bytes trên mỗi cluster
	int sectors_of_boot; // SB : Số sector trước vùng FAT
	int numbers_of_fats; // NF : Số bảng FAT
	int sectors_per_fat; //SF : Số sector trên 1 bảng FAT
	vector<vector<string>> FAT;
public:
	FAT32(HANDLE disk, vector<vector<string>> sector);

	void readBoot_Sector();

	int first_Sector_Of_Data();

	vector<vector<vector<string>>> split_Entry(vector<vector<string>> table);

	int check_Entry(vector<vector<string>> entry);
	int check_Character(string x);

	int get_First_Cluster(vector<vector<string>> entry);
	int get_First_Sector(vector<vector<string>> entry);
	string find_Attribute(vector<vector<string>> entry);
	string find_Name(vector<vector<string>> entry);
	vector<string> split_File_Name(string file_name, char ch);

	int is_Folder_Empty(vector<vector<string>> entry);

	string read_Data(int readPont, int level);

	void read_File(vector<vector<string>> entry, int level);
	void read_Folder(vector<vector<string>> entry, int level);
	void read_Not_File_Or_Folder(vector<vector<string>> entry, int level);

	void read_RDet(vector<vector<string>> table, int level);
};

#endif // !_FAT_32_H_
