#include"NTFS.h"

NTFS::NTFS(HANDLE disk, vector<vector<string>> sector)
{
    device = disk;
    BootSector = sector;
}

void NTFS::readBoot_Sector()
{
    cout << "- Type of File System: NTFS" << endl;

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

int NTFS::get_first_sector_MFT()
{
    return begin_MFT * sectors_per_cluster + 78;
}



vector<string> NTFS::split_File_Name(string file_name, char ch)
{
    vector<string> res;
    string fea;
    stringstream line;
    if (file_name != "")
    {
        line << file_name;
        while (getline(line, fea, ch))
            res.push_back(fea);
    }
    return res;
}

string NTFS::get_ext_file(string file_name, NTFS drive)
{
    vector<string> name_fea = drive.split_File_Name(file_name, '.');
    string ext = name_fea[1];
    string ext1;
    for (int i = 0; i < ext.size(); i++)
    {
        if (ext[i] == 'a' or ext[i] == 'b' or ext[i] == 'c' or ext[i] == 'd' or ext[i] == 'e' or ext[i] == 'f' or ext[i] == 'g'
            or ext[i] == 'h' or ext[i] == 'i' or ext[i] == 'j' or ext[i] == 'k' or ext[i] == 'l' or ext[i] == 'm' or ext[i] == 'n'
            or ext[i] == 'o' or ext[i] == 'p' or ext[i] == 'q' or ext[i] == 'r' or ext[i] == 's' or ext[i] == 't' or ext[i] == 'u'
            or ext[i] == 'v' or ext[i] == 'w' or ext[i] == 'x' or ext[i] == 'y' or ext[i] == 'z'
            or ext[i] == 'A' or ext[i] == 'B' or ext[i] == 'C' or ext[i] == 'D' or ext[i] == 'E' or ext[i] == 'F' or ext[i] == 'G'
            or ext[i] == 'H' or ext[i] == 'I' or ext[i] == 'J' or ext[i] == 'K' or ext[i] == 'L' or ext[i] == 'M' or ext[i] == 'N'
            or ext[i] == 'O' or ext[i] == 'P' or ext[i] == 'Q' or ext[i] == 'R' or ext[i] == 'S' or ext[i] == 'T' or ext[i] == 'U'
            or ext[i] == 'V' or ext[i] == 'W' or ext[i] == 'X' or ext[i] == 'Y' or ext[i] == 'Z')
        {
            ext1 += ext[i];
        }
    }
    return ext1;
}

string NTFS::get_type_file(string flag_type)
{
    string type_file;
    if (flag_type == "10000000")
    {
        type_file = "Thu muc";
    }
    if (flag_type == "00000020")
    {
        type_file = "Tap tin";
    }
    return type_file;
}

void NTFS::read_MFT(vector<vector<string>> k, NTFS drive)
{
    //vector<vector<string>> k = find_table(disk, drive.get_first_sector_MFT() * 512);

    vector<vector<string>> sector;
    int total_sector = k.size() / 64;
    for (int j = 0; j < total_sector; j++)
    {
        for (int i = 0 + 64 * j; i < 64 + 64 * j; i++)
        {
            sector.push_back(k[i]);
        }
        int id_record = hexstr_to_int(to_hexstr(sector, 12, 2, 4, 1));
        cout << "ID record: " << id_record << endl;

        int start_offset_$standard_info = hexstr_to_int(to_hexstr(sector, 4, 1, 2, 1));

        int size_$standard_info = hexstr_to_int(to_hexstr(sector, (start_offset_$standard_info + 4) % 16, (start_offset_$standard_info + 4) / 16, 4, 1));

        int start_offset_data_$standard_info = hexstr_to_int(to_hexstr(sector, (start_offset_$standard_info + 20) % 16, (start_offset_$standard_info + 20) / 16, 2, 1))
            + start_offset_$standard_info;

        int start_offset_$file_name = start_offset_$standard_info + size_$standard_info;

        int size_$file_name = hexstr_to_int(to_hexstr(sector, (start_offset_$file_name + 4) % 16, (start_offset_$file_name + 4) / 16, 4, 1));

        int start_offset_data_$file_name = hexstr_to_int(to_hexstr(sector, (start_offset_$file_name + 20) % 16, (start_offset_$file_name + 20) / 16, 2, 1))
            + start_offset_$file_name;

        int parent_id = hexstr_to_int(to_hexstr(sector, start_offset_data_$file_name % 16, start_offset_data_$file_name / 16, 6, 1));
        cout << "Parent ID: " << parent_id << endl;

        string flag_type = to_hexstr(sector, (start_offset_data_$file_name + 56) % 16, (start_offset_data_$file_name + 56) / 16, 4, 1);
        string type = drive.get_type_file(flag_type);
        cout << "Type of File:" << type << endl;

        int len_file_name_attribute = hexstr_to_int(to_hexstr(sector, (start_offset_data_$file_name + 64) % 16, (start_offset_data_$file_name + 64) / 16, 1, 1));

        int start_offset_file_name_attribute = start_offset_data_$file_name + 66;

        int modun = start_offset_file_name_attribute % 16;

        string file_name;

        //doc ten file
        if ((modun + len_file_name_attribute * 2) > 16) {
            file_name = hexstr_tostr(to_hexstr(sector, start_offset_file_name_attribute % 16, start_offset_file_name_attribute / 16,
                16 - modun, 1));
            reverse(file_name.begin(), file_name.end());
            int remain_size_file_name = len_file_name_attribute * 2 - (16 - modun);
            int now_offset = start_offset_file_name_attribute + (16 - modun);
            int q = remain_size_file_name / 16;
            int p = remain_size_file_name % 16;
            string name;
            for (int i = 0; i < q; i++)
            {
                name = hexstr_tostr(to_hexstr(sector, (now_offset + i * 16) % 16, (now_offset + i * 16) / 16,
                    16, 1));
                reverse(name.begin(), name.end());
                file_name += name;
            }
            now_offset += q * 16;
            name = hexstr_tostr(to_hexstr(sector, now_offset % 16, now_offset / 16,
                p, 1));
            reverse(name.begin(), name.end());
            file_name += name;
        }
        else {
            file_name = hexstr_tostr(to_hexstr(sector, start_offset_file_name_attribute % 16, start_offset_file_name_attribute / 16,
                len_file_name_attribute * 2, 1));
            reverse(file_name.begin(), file_name.end());
        }
        cout << "File name: " << file_name << endl;
        int start_offset_next_attribute = start_offset_$file_name + size_$file_name;
        int next_attribute, size_next_attribute;
        int start_offset_$data;
        int size_$data, size_of_file, start_offset_data_of_txt;
        if (flag_type == "00000020")
        {

            string ext = drive.get_ext_file(file_name, drive);

            if (ext == "txt" or ext == "TXT")
            {
                next_attribute = hexstr_to_int(to_hexstr(sector, start_offset_next_attribute % 16, start_offset_next_attribute / 16, 4, 1));

                if (next_attribute != 128)
                {
                    size_next_attribute = hexstr_to_int(to_hexstr(sector, (start_offset_next_attribute + 4) % 16,
                        (start_offset_next_attribute + 4) / 16, 4, 1));
                    start_offset_$data = start_offset_next_attribute + size_next_attribute;
                }
                else
                {
                    start_offset_$data = start_offset_next_attribute;
                }
                size_of_file = hexstr_to_int(to_hexstr(sector, (start_offset_next_attribute + 4) % 16,
                    (start_offset_next_attribute + 4) / 16, 4, 1));
                size_$data = hexstr_to_int(to_hexstr(sector, (start_offset_next_attribute + 16) % 16,
                    (start_offset_next_attribute + 16) / 16, 4, 1));
                start_offset_data_of_txt = hexstr_to_int(to_hexstr(sector, (start_offset_next_attribute + 20) % 16,
                    (start_offset_next_attribute + 20) / 16, 2, 1)) + start_offset_$data;
                cout << "Size File:" << size_$data << " bytes" << endl;

                //xu li doc du lieu data file txt
                string data;
                int data_row = start_offset_data_of_txt / 16;
                int data_col = start_offset_data_of_txt % 16;
                int count = 0;
                while (count < size_$data)
                {
                    data += sector[data_row][data_col];
                    data_col++;
                    if (data_col == 16)
                    {
                        data_col = 0;
                        data_row += 1;
                    }
                    count++;
                }

                string datafile = hexstr_tostr(data);
                cout << "Data: " << datafile << endl;
            }
            else
            {
                int size_file = hexstr_to_int(to_hexstr(sector, (start_offset_$file_name + 64) % 16,
                    (start_offset_$file_name + 64) / 16, 4, 1));
                cout << "Size File: " << size_file << " bytes" << endl;
            }
        }
        sector.clear();
        cout << endl;
    }
}