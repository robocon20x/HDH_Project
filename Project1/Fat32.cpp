#include"Fat32.h"

FAT32::FAT32(HANDLE disk, vector<vector<string>> sector)
{
	device = disk;
	BootSector = sector;
};

void FAT32::readBoot_Sector()
{
    cout << "- Type of File System: FAT32" << endl;

    bytes_per_sector = hexstr_to_int(to_hexstr(BootSector, 11, 0, 2, 1));
    cout << "- The number of bytes per sector: " << bytes_per_sector << endl;

    sectors_per_cluster = hexstr_to_int(to_hexstr(BootSector, 13, 0, 1, 1));
    cout << "- The number of sectors per cluster: " << sectors_per_cluster << endl;

    sectors_of_boot = hexstr_to_int(to_hexstr(BootSector, 14, 0, 2, 1));
    cout << "- The number of bytes in Boot Sector: " << sectors_of_boot << endl;

    numbers_of_fats = hexstr_to_int(to_hexstr(BootSector, 0, 1, 1, 1));
    cout << "- The number of FAT table: " << numbers_of_fats << endl;

    cout << "- Size of Volumn: " << hexstr_to_int(to_hexstr(BootSector, 0, 2, 4, 1)) << endl;

    sectors_per_fat = hexstr_to_int(to_hexstr(BootSector, 4, 2, 4, 1));
    cout << "- Size of FAT table: " << sectors_per_fat << endl;

    cout << "- First sector of FAT table: " << sectors_of_boot << endl;

    cout << "- First sector of RDET: " << sectors_of_boot + numbers_of_fats * sectors_per_fat << endl;

    cout << "- First sector of Data: " << sectors_of_boot + numbers_of_fats * sectors_per_fat << endl;
}

int FAT32::first_Sector_Of_Data()
{
    return sectors_of_boot + numbers_of_fats * sectors_per_fat;
}

vector<vector<vector<string>>> FAT32::split_Entry(vector<vector<string>> table)
{
    vector<vector<vector<string>>> res;

    for (int i = 0; i < table.size();)
    {
        string x = to_hexstr(table, 11, i, 1, 1);
        if (x != "0F")
        {
            vector<vector<string>> temp;
            for (int j = i; j < i + 2; j++)
            {
                temp.push_back(table[j]);
            }
            res.push_back(temp);
            i += 2;
        }
        else
        {
            int k = i + 2;
            int count = 1;
            while (to_hexstr(table, 11, k, 1, 1) == "0F")
            {
                count = count + 1;
                k = k + 2;
            }
            vector<vector<string>> temp;
            for (int j = i; j < i + 2 * (count + 1); j++)
            {
                temp.push_back(table[j]);
            }
            res.push_back(temp);
            i += 2 * (count + 1);
        }
    }
    while (res[0][0][0] == "2E")
    {
        res.erase(res.begin());
    }
    return res;
}

int FAT32::check_Entry(vector<vector<string>> entry)
{
    string x;
    if (entry[0][11] != "0F")
    {
        x = to_binstr(hexstr_to_int(entry[0][11]));
    }
    else
    {
        x = to_binstr(hexstr_to_int(entry[entry.size() - 2][11]));
    }

    int cnt = 0;
    for (int i = 0; i < x.length(); i++)
    {
        if (x[i] == '1')
        {
            cnt = cnt + 1;
        }
    }

    if (cnt == 1)
        return 1;
    return 0;
}
int FAT32::check_Character(string x)
{
    if (x[0] >= '2' && x[0] <= '7')
        return 1;
    return 0;
}

int FAT32::get_First_Cluster(vector<vector<string>> entry)
{
    if (entry[0][11] != "0F")
    {
        return hexstr_to_int(to_hexstr(entry, 4, 1, 2, 1) + to_hexstr(entry, 10, 1, 2, 1));
    }
    else
    {
        return hexstr_to_int(to_hexstr(entry, 4, entry.size() - 1, 2, 1) + to_hexstr(entry, 10, entry.size() - 1, 2, 1));
    }
}

int FAT32::get_First_Sector(vector<vector<string>> entry)
{
    return first_Sector_Of_Data() + 8 * (get_First_Cluster(entry) - 2);
}

string FAT32::find_Attribute(vector<vector<string>> entry)
{
    string res_hex, res_bin;
    int i;

    for (i = 0; i < entry.size(); i++)
    {
        if (to_hexstr(entry, 11, i, 1, 1) != "0F" && to_hexstr(entry, 11, i, 1, 1) != "00")
            res_hex = to_hexstr(entry, 11, i, 1, 1);
    }
    res_bin = to_binstr(hexstr_to_int(res_hex));

    i = res_bin.length() - 1;
    while (res_bin[i] != '1')
    {
        i--;
    }

    if (check_Entry(entry))
    {
        switch (i)
        {
        case 7:
            return "Read Only";
            break;
        case 6:
            return "Hidden";
            break;
        case 5:
            return "System";
            break;
        case 4:
            return "Volumn Label";
            break;
        case 3:
            return "Directory";
            break;
        case 2:
            return "Archive";
            break;
        case 1:
            return "Device";
            break;
        case 0:
            return "Unused";
            break;
        }
    }
    else
    {
        return "Error!";
    }
}
string FAT32::find_Name(vector<vector<string>> entry)
{
    string temp = to_hexstr(entry, 11, 0, 1, 1);
    string res = "";
    if (temp != "00" && temp != "0F")
    {
        return hexstr_tostr(to_hexstr(entry, 0, 0, 11, 0));
    }

    if (temp != "00" && temp == "0F")
    {
        int i = entry.size() - 4;
        while (i >= 0)
        {
            for (int j = 1; j < 16; j++)
            {
                if (j != 11 && j != 13 && entry[i][j] != "00" && check_Character(entry[i][j]) == 1)
                    res += entry[i][j];
            }
            for (int j = 0; j < 16; j++)
            {
                if (j != 11 && entry[i + 1][j] != "00" && check_Character(entry[i + 1][j]) == 1)
                    res += entry[i + 1][j];
            }
            i = i - 2;
        }
        return hexstr_tostr(res);
    }
}
vector<string> FAT32::split_File_Name(string file_name, char ch)
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

int FAT32::is_Folder_Empty(vector<vector<string>> entry)
{
    vector<vector<string>> sdet = find_table(device, get_First_Sector(entry) * 512);
    vector<vector<vector<string>>> sdet_entry = split_Entry(sdet);

    if (sdet_entry[0][0][0] == "00")
        return 1;
    return 0;
}

string FAT32::read_Data(int readPont, int level)
{
    string res;
    vector<vector<string>> data = find_table(device, readPont);

    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < 16; j++)
        {
            res = res + data[i][j];
        }
    }

    return res;
}

void FAT32::read_File(vector<vector<string>> entry, int level)
{
    print_Tab(level);
    cout << "File name: " << find_Name(entry) << endl;
    print_Tab(level);
    cout << "Size: " << hexstr_to_int(to_hexstr(entry, 12, entry.size() - 1, 4, 1)) << endl;
    print_Tab(level);
    cout << "First cluster: " << get_First_Cluster(entry);

    cout << "\n ";
    print_Tab(level);
    cout << "--> Cluster used: ";
    vector<int> cluster_used;
    int i = 4 * get_First_Cluster(entry);            //chi so cot trong bang fat
    cluster_used.push_back(get_First_Cluster(entry));
    while (1)
    {
        int k = i;
        int j = 0;                        //chi so dong trong bang fat
        for (j;;)
        {
            if (i >= 16 * (j + 1))
                j++;
            else break;
        }
        while (k >= 16)                   //chi so cot luon < 16
        {
            k -= 16;
        }
        string temp = to_hexstr(FAT, k, j, 1, 1);
        if (temp != "FF" && temp != "0F")
        {
            cluster_used.push_back(hexstr_to_int(temp));
            i += 4;
        }
        else break;
    }
    for (int j = 0; j < cluster_used.size(); j++)
    {
        cout << cluster_used[j];
        if (j < cluster_used.size() - 1)
        {
            cout << ", ";
        }
    }

    cout << "\n ";
    print_Tab(level);
    cout << "--> Sector used: ";
    vector<int> sector_used;
    int cur_sector = get_First_Sector(entry);
    for (int j = 0; j < cluster_used.size(); j++)
    {
        for (int k = 0; k < sectors_per_cluster; k++)
        {
            sector_used.push_back(cur_sector);
            cur_sector = cur_sector + 1;
        }
    }
    for (int j = 0; j < sector_used.size(); j++)
    {
        cout << sector_used[j];
        if (j < sector_used.size() - 1)
        {
            cout << ", ";
        }
    }

    vector<string> name_fea = split_File_Name(find_Name(entry), '.');
    string ext = name_fea[1];
    if (ext == "txt" || ext == "TXT")
    {
        cout << "\n ";
        print_Tab(level);
        cout << "--> Read " << name_fea[1] << " file:\n";
        string res = hexstr_tostr(read_Data(sector_used[0] * 512, level));
        vector<string> temp;
        string fea;
        stringstream line;
        if (res != "")
        {
            line << res;
            while (getline(line, fea, '\n'))
                temp.push_back(fea);
        }
        string tab;
        for (int i = 0; i < level; i++)
            tab += '\t';
        for (int i = 0; i < temp.size(); i++)
            temp[i] = tab + temp[i];
        res.clear();
        for (int i = 0; i < temp.size(); i++)
            res += temp[i];
        cout << res;
    }
    else
    {
        cout << "\n";
        print_Tab(level);
        cout << "Open " << ext << " file by ";
        if (ext == "doc" || ext == "DOC" || ext == "docx" || ext == "DOCX")
            cout << "Microsoft Word" << endl;
        if (ext == "xls" || ext == "XLS" || ext == "xlsx" || ext == "XLSX")
            cout << "Microsoft Excel" << endl;
        if (ext == "pptx" || ext == "PPTX")
            cout << "Microsoft Powerpoint" << endl;
        if (ext == "pdf" || ext == "PDF")
            cout << "Adobe Reader XI" << endl;
        if (ext == "mp3" || ext == "MP3" || ext == "mp4" || ext == "MP4" || ext == "wmv" || ext == "WMV" || ext == "wma" || ext == "WMA")
            cout << "Windows Media Player" << endl;
        if (ext == "jpg" || ext == "JPG" || ext == "jpeg" || ext == "JPEG" || ext == "png" || ext == "PNG")
            cout << "Photos" << endl;
        if (ext == "accdb" || ext == "ACCDB")
            cout << "Microsoft Access" << endl;
        if (ext == "sql" || ext == "SQL")
            cout << "SQL Server or MySQL" << endl;
        if (ext == "pub" || ext == "PUB")
            cout << "Microsoft Publisher" << endl;
        if (ext == "rar" || ext == "RAR" || ext == "rar4" || ext == "RAR4" || ext == "zip" || ext == "ZIP")
            cout << "WinRAR" << endl;
        if (ext == "rtf" || ext == "RTF")
            cout << "WordPad" << endl;
    }
}
void FAT32::read_Folder(vector<vector<string>> entry, int level)
{
    print_Tab(level);
    cout << "Folder name: " << find_Name(entry) << endl;
    print_Tab(level);
    cout << "First cluster: " << get_First_Cluster(entry);

    cout << "\n ";
    print_Tab(level);
    cout << "--> Cluster used: ";
    vector<int> cluster_used;
    int i = 4 * get_First_Cluster(entry);            //chi so cot trong bang fat
    cluster_used.push_back(get_First_Cluster(entry));
    while (1)
    {
        int k = i;
        int j = 0;                        //chi so dong trong bang fat
        for (j;;)
        {
            if (i >= 16 * (j + 1))
                j++;
            else break;
        }
        while (k >= 16)                   //chi so cot luon < 16
        {
            k -= 16;
        }
        string temp = to_hexstr(FAT, k, j, 1, 1);
        if (temp != "FF" && temp != "0F")
        {
            cluster_used.push_back(hexstr_to_int(temp));
            i += 4;
        }
        else break;
    }
    for (int j = 0; j < cluster_used.size(); j++)
    {
        cout << cluster_used[j];
        if (j < cluster_used.size() - 1)
        {
            cout << ", ";
        }
    }

    cout << "\n ";
    print_Tab(level);
    cout << "--> Sector used: ";
    vector<int> sector_used;
    int cur_sector = get_First_Sector(entry);
    for (int j = 0; j < cluster_used.size(); j++)
    {
        for (int k = 0; k < sectors_per_cluster; k++)
        {
            sector_used.push_back(cur_sector);
            cur_sector = cur_sector + 1;
        }
    }
    for (int j = 0; j < sector_used.size(); j++)
    {
        cout << sector_used[j];
        if (j < sector_used.size() - 1)
        {
            cout << ", ";
        }
    }
}
void FAT32::read_Not_File_Or_Folder(vector<vector<string>> entry, int level)
{
    int first_sector_of_data = sectors_of_boot + numbers_of_fats * sectors_per_fat;

    if (find_Attribute(entry) == "Volumn Label")
    {
        print_Tab(level);
        cout << "Volumn name: " << find_Name(entry);
    }
    else if (find_Attribute(entry) == "Error!")
    {
        print_Tab(level);
        cout << "File name: " << find_Name(entry) << endl;
        int first_cluster;
        if (entry[0][11] != "0F")
        {
            first_cluster = hexstr_to_int(to_hexstr(entry, 4, 1, 2, 1) + to_hexstr(entry, 10, 1, 2, 1));
        }
        else
        {
            first_cluster = hexstr_to_int(to_hexstr(entry, 4, entry.size() - 1, 2, 1) + to_hexstr(entry, 10, entry.size() - 1, 2, 1));
        }
        print_Tab(level);
        cout << "First cluster: " << first_cluster;

        cout << "\n ";
        print_Tab(level);
        cout << "--> Cluster used: ";
        vector<int> cluster_used;
        int i = 4 * first_cluster;            //chi so cot trong bang fat
        cluster_used.push_back(first_cluster);
        while (1)
        {
            int k = i;
            int j = 0;                        //chi so dong trong bang fat
            for (j;;)
            {
                if (i >= 16 * (j + 1))
                    j++;
                else break;
            }
            while (k >= 16)                   //chi so cot luon < 16
            {
                k -= 16;
            }
            string temp = to_hexstr(FAT, k, j, 1, 1);
            if (temp != "FF" && temp != "0F")
            {
                cluster_used.push_back(hexstr_to_int(temp));
                i += 4;
            }
            else break;
        }
        for (int j = 0; j < cluster_used.size(); j++)
        {
            cout << cluster_used[j];
            if (j < cluster_used.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << "\n ";
        print_Tab(level);
        cout << "--> Sector used: ";
        vector<int> sector_used;
        int cur_sector = first_sector_of_data + 8 * (cluster_used[0] - 2);
        for (int j = 0; j < cluster_used.size(); j++)
        {
            for (int k = 0; k < sectors_per_cluster; k++)
            {
                sector_used.push_back(cur_sector);
                cur_sector = cur_sector + 1;
            }
        }
        for (int j = 0; j < sector_used.size(); j++)
        {
            cout << sector_used[j];
            if (j < sector_used.size() - 1)
            {
                cout << ", ";
            }
        }
    }
}

void FAT32::read_RDet(vector<vector<string>> table, int level)
{
    FAT = find_table(device, sectors_of_boot * 512);
    vector<vector<vector<string>>> table_entry = split_Entry(table);

    if (table_entry[0][0][0] == "00")
    {
        cout << "\n";
        print_Tab(level);
        cout << "No files or folders are found!\n\n";
        return;
    }

    for (int i = 0; i < table_entry.size(); i++)
    {
        if (table_entry[i][0][0] != "00")
        {
            if (find_Attribute(table_entry[i]) != "Archive" && find_Attribute(table_entry[i]) != "Directory")
            {
                read_Not_File_Or_Folder(table_entry[i], level);
                cout << "\n\n";
            }
            else if (find_Attribute(table_entry[i]) == "Archive")
            {
                read_File(table_entry[i], level);
                cout << "\n\n";
            }
            else if (find_Attribute(table_entry[i]) == "Directory")
            {
                read_Folder(table_entry[i], level);
                if (!is_Folder_Empty(table_entry[i]))
                {
                    cout << "\n\n";
                }
                read_RDet(find_table(device, get_First_Sector(table_entry[i]) * 512), level + 1);
            }
        }
        else return;
    }
}