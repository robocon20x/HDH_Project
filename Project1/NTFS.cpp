#include"NTFS.h"

NTFS::NTFS(HANDLE disk, vector<vector<string>> sector)
{
	device = disk;
	BootSector = sector;
}

void NTFS::readBoot_Sector()
{
	cout << "- Type of File System: FAT32" << endl;

	bytes_per_sector = hexstr_to_int(to_hexstr(BootSector, 11, 0, 2, 1));
	cout << "- The number of bytes per sector: " << bytes_per_sector << endl;

	sectors_per_cluster = hexstr_to_int(to_hexstr(BootSector, 13, 0, 1, 1));
	cout << "- The number of sectors per cluster: " << sectors_per_cluster << endl;

	cout << "- Media descriptor: " << to_hexstr(BootSector, 5, 1, 1, 1) << " hard disk" << endl;

	cout << "- Total number sectors NTFS: " << hexstr_to_int(to_hexstr(BootSector, 8, 2, 8, 1)) << endl;

	begin_MFT = hexstr_to_int(to_hexstr(BootSector, 0, 3, 8, 1));
	cout << "- The MFT begin at cluster: " << begin_MFT << endl;

	cout << "- MFT mirror begin at: " << hexstr_to_int(to_hexstr(BootSector, 8, 3, 8, 1)) << endl;
}

void NTFS::read_MFT()
{
	vector<vector<string>> MFT = find_table(device, begin_MFT * 512);

}
