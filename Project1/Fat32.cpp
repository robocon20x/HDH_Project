#include"Fat32.h"

fat32::fat32(HANDLE disk, vector<vector<string>> sector)
{
	device = disk;
	BootSector = sector;
};

void fat32::readBootsector()
{
	cout << "Loai FAT: FAT32" << endl;

	bytes_per_sector = hexstr_to_int(to_hexstr(BootSector, 11, 0, 2, 1));
	cout << "So byte tren 1 sector: " << bytes_per_sector << endl;

	sectors_per_cluster = hexstr_to_int(to_hexstr(BootSector, 13, 0, 1, 1));
	cout << "So sector tren 1 cluster: " << sectors_per_cluster << endl;

	sectors_of_boot = hexstr_to_int(to_hexstr(BootSector, 14, 0, 2, 1));
	cout << "So sector vung Bootsector: " << sectors_of_boot << endl;

	numbers_of_fats = hexstr_to_int(to_hexstr(BootSector, 0, 1, 1, 1));
	cout << "So bang FAT: " << numbers_of_fats << endl;

	cout << "Tong so sector tren volume: " << hexstr_to_int(to_hexstr(BootSector, 0, 2, 4, 1)) << endl;

	sectors_per_fat = hexstr_to_int(to_hexstr(BootSector, 4, 2, 4, 1));
	cout << "So sector cho 1 bang FAT: " << sectors_per_fat << endl;

	cout << "Sector dau tien cua bang FAT: " << sectors_of_boot << endl;

	cout << "Sector dau tien cua bang RDET: " << sectors_of_boot + numbers_of_fats * sectors_per_fat << endl;

	cout << "Sector dau tien cua vung DATA: " << sectors_of_boot + numbers_of_fats * sectors_per_fat << endl;
};

void fat32::readRDET()
{
	int readPoint = (sectors_of_boot + numbers_of_fats * sectors_per_fat)*bytes_per_sector;
	
	vector<vector<string>> result = find_table(device, readPoint);

	print_table(result);

	return;
}