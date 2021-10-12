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

int fat32::readRDET()
{
	DWORD bytesRead;
	BYTE sector[512];

	int readPont = (sectors_of_boot + numbers_of_fats * sectors_per_fat) * bytes_per_sector;
	SetFilePointer(device, readPont, NULL, FILE_BEGIN);//Set a Point to Read

	if (!ReadFile(device, sector, 512, &bytesRead, NULL))
	{
		printf("ReadFile: %u\n", GetLastError());
		return 0;
	}
	else
	{
		vector<vector<string>> vec = to_vector(sector);
		for (int i = 0; i < vec.size(); i++)
		{
			for (int j = 0; j < vec[i].size(); j++)
				cout << vec[i][j] << " ";
			cout << endl;
		}
		printf("Success!\n\n");
		return 1;
	}
}

