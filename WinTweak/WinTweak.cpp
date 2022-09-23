#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <cstring>
#include <sstream>
#include <locale>
#include <VersionHelpers.h>
#include <filesystem>
#include <io.h>

using namespace std;

void clear();
void pause_on_exit();
void pause_on_continue();
void system_info();
void clearTrash();
void install_vcredist();
void clear_tmp();
void set_console_size();
void install_directx();
void install_framework48();
void install_office_proplus_ltsc_2021();
void activate_windows();

std::wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

wstring PATH_a = ExePath();
const std::string PATH_str(PATH_a.begin(), PATH_a.end());
char PATH[255];
char PATH_EXEC[255];
string arch;
string test1;
bool network;

struct stat info;
bool is64 = stat( "C:\\Windows\\SysWOW64", &info );
bool isarm64 = stat( "C:\\Program Files (Arm)", &info );

int exec(string command) {
	{
		string PATH_temp = command;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		bool return_code = system(PATH_EXEC);
		return return_code;
	}
}

int exec_file(string filename) {
    {
		string PATH_temp = "cd " + PATH_str + "\\tmp && " + filename;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		bool return_code = system(PATH_EXEC);
		return return_code;
	}
}

bool fileExists(const std::string& file) {
	struct stat buf;
	return (stat(file.c_str(), &buf) == 0);
}

int check_exist(string pathf) {
	{
		string PATH_temp = pathf;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		bool isfile = stat(PATH_EXEC, &info);
		return isfile;
	}
}

int download_file(string link, string filename) {
	{
		string PATH_temp = "cd " + PATH_str + "\\tmp && .\\..\\tools\\wget.exe --no-check-certificate -q --show-progress -O " + filename + " " + link;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		bool return_code = system(PATH_EXEC);
		return return_code;
	}
}

int active_windows(string key, string kms) {
	system("cscript //nologo slmgr.vbs /upk");
	system("cscript //nologo slmgr.vbs /cpky");
	{
		string PATH_temp = "cscript //nologo slmgr.vbs /ipk " + key;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
	}
	{
		string PATH_temp = "cscript //nologo slmgr.vbs /skms " + kms;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
		bool active = system("cscript //nologo slmgr.vbs /ato");
		return active;
	}
}


int main(){
check:
	set_console_size();
	if (!IsWindows7OrGreater())
	{
		cout << "You need at least Windows 7 or later\n";
		pause_on_exit();
		exit(1);
	}
	if (IsWindowsServer())
	{
		printf("You are running this application on Windows Server!\nRecommend not to run this app\n");
		pause_on_continue();
	}

	if (check_exist(PATH_str + "\\tools") != 0) {
		cout << "Tools folder is missing!\n";
		pause_on_exit();
		exit(1);
	}
	
	if (PATH_str.length() > 128) {
    	cout << "PATH directory to long [" << PATH_str.length() << "]!\nPlease change to another short PATH!\n Required: 128 or below!\n";
    	pause_on_exit();
    	exit(1);
	} else {
		strcpy_s(PATH, PATH_str.c_str());
	}
	cout << "Testing internet....\n";
	if (exec("ping -n 1 google.com > NUL 2>&1") == 0) {
		network = 1;
		test1 = "Yes";
	} else {
		network = 0;
		test1 = "No";
	}
	if (is64 == 0) {
		if (isarm64 == 0) {
			arch = "ARM64";
		} else {
	    	arch = "x86_64";
		}
	} else {
	    arch = "x86";
	}
    clear_tmp();
main_menu:
	clear();
	set_console_size();
	short int khanh;
	cout << "Menu by KhanhNguyen9872\n";
	cout << "CPU: " << arch << "\n\n Internet: " << test1 << "\n DIR: " << PATH_str << "\n\n 1. System info\n 2. Clear Trash\n 3. Activate Windows\n 4. Install vcredist\n 5. Install DirectX\n 6. Install Framework\n 7. Install Office\n 0. Exit\n\n Your choose: ";
	cin >> khanh;
	switch (khanh) {
		case 1:
			system_info();
		case 2:
			clearTrash();
		case 3:
			if (network == 1) {
				goto activate_win;
			}
			goto main_menu;
		case 4:
			if (network == 1) {
				install_vcredist();
			}
			goto main_menu;
		case 5:
			if (network == 1) {
				install_directx();
			}
			goto main_menu;
		case 6:
			if (network == 1) {
				goto framework;
			}
			goto main_menu;
		case 7:
			if (network == 1) {
				goto office;
			}
			goto main_menu;
		case 0:
			clear();
			exit(0);
	}
	goto main_menu;
activate_win:
	clear();
	cout << "Menu by KhanhNguyen9872\n";
	cout << "\n 1. Windows 7\n 2. Windows 8\n 3. Windows 8.1\n 4. Windows 10/11\n 0. Exit\n\n Your choose: ";
	cin >> khanh;
	switch (khanh) {
	case 1:
		activate_windows();
		goto activate_win;
	case 2:
		activate_windows();
		goto activate_win;
	case 3:
		activate_windows();
		goto activate_win;
	case 4:
		activate_windows();
		goto activate_win;
	case 0:
		goto main_menu;
		break;
	}
	goto activate_win;
framework:
	clear();
	cout << "Menu by KhanhNguyen9872\n";
	cout << "\n 1. Framework 4.8\n 0. Exit\n\n Your choose: ";
	cin >> khanh;
	switch (khanh) {
		case 1:
			install_framework48();
			goto framework;
		case 0:
			goto main_menu;
			break;
	}
	goto framework;
office:
	clear();
	cout << "Menu by KhanhNguyen9872\n";
	cout << "\n 1. Office ProPlus LTSC 2021\n 0. Exit\n\n Your choose: ";
	cin >> khanh;
	switch (khanh) {
		case 1:
			install_office_proplus_ltsc_2021();
			goto office;
		case 0:
			goto main_menu;
			break;
	}
	goto office;
}

void set_console_size() {
    {
		system("mode con: cols=65 lines=9001 && color 71");
	}
}

void activate_windows() {
    {
		active_windows("W269N-WFGWX-YVC9B-4J6C9-T83GX", "");
	}
}

void pause_on_exit()
{
	cout << "\n============\n Press any key to Exit!\n";
    system("pause > NUL");
}

void clear_tmp() {
	{
	    string PATH_temp = "rmdir /q /s " + PATH_str + "\\tmp > NUL 2>&1";
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
	}
	{
		string PATH_temp = "mkdir " + PATH_str + "\\tmp > NUL 2>&1";
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
	}
}

void install_office_proplus_ltsc_2021() {
	if (check_exist(PATH_str + "\\tools\\wget.exe") != 0) {
		cout << "wget not found";
		pause_on_exit();
	}
	else {
		clear();
		clear_tmp();
		printf("\n Downloading Office ProPlus LTSC 2021...\n");
		download_file("https://tinyurl.com/bdr9pbaa", "OfficeProPlusLTSC2021.exe");
		printf("\n Installing Office ProPlus LTSC 2021... ");
		exec_file("OfficeProPlusLTSC2021.exe");
		printf("Done\n");
		pause_on_exit();
	}
}

void install_framework48() {
	clear();
	clear_tmp();
	printf ("\n Downloading Framework 4.8...\n");
	download_file("https://tinyurl.com/2vb2fudw", "Framework48.exe");
	printf ("\n Installing Framework 4.8... ");
	exec_file("Framework48.exe /q /norestart");
	printf ("Done\n");
	pause_on_exit();
}

void install_directx() {
	clear();
	clear_tmp();
	printf ("\n Downloading DirectX...\n");
	download_file("https://tinyurl.com/55824f4a", "DirectX.exe");
	printf ("\n Installing DirectX... ");
	exec_file("DirectX.exe");
	printf ("\nInstallation is running in the background\n");
	pause_on_exit();
}

void install_vcredist() {
	clear();
	clear_tmp();
	printf ("\n Downloading vcredist x86...\n");
	download_file("https://tinyurl.com/4b75en7n", "vcredist_x86.exe");
	printf ("\n Installing vcredist x86... ");
	exec_file("vcredist_x86.exe /install /quiet /norestart");
	printf ("Done\n");
	if (arch == "x86_64" || arch == "ARM64") {
		printf ("============");
		printf ("\n Downloading vcredist x64...\n");
		download_file("https://tinyurl.com/bdzea7wf", "vcredist_x64.exe");
		printf ("\n Installing vcredist x64... ");
		exec_file("vcredist_x64.exe /install /quiet /norestart");
		printf ("Done\n");
	}
	if (arch == "ARM64") {
		printf ("============");
		printf ("\n Downloading vcredist ARM...\n");
		download_file("https://tinyurl.com/wbzj2sc7", "vcredist_ARM.exe");
		printf ("\n Installing vcredist ARM... ");
		exec_file("vcredist_ARM.exe /install /quiet /norestart");
		printf ("Done\n");
		printf ("============");
		printf ("\n Downloading vcredist ARM64...\n");
		download_file("https://tinyurl.com/ys649mw9", "vcredist_ARM64.exe");
		printf ("\n Installing vcredist ARM64... ");
		exec_file("vcredist_ARM64.exe /install /quiet /norestart");
		printf ("Done\n");
	}
	cout << "============\n Install completed!";
	clear_tmp();
	pause_on_exit();
}

void pause_on_continue()
{
	cout << "\n\n Press any key to continue!\n";
    system("pause > NUL");
}

void clear()
{
    system("cls");
}

void clearTrash()
{
	clear();
    string khanh;
    cout << "This process will clear all temp folder!\nDo you want to continue? [Y/N]: ";
    cin >> khanh;
    if (khanh == "Y" || khanh == "y") {
    	system("rmdir /q /s %temp% > NUl 2>&1");
		system("mkdir %temp% > NUL 2>&1");
    	system("rmdir /s /q C:\\windows\\prefetch > NUL 2>&1");
		system("mkdir C:\\windows\\prefetch > NUL 2>&1");
	    system("rmdir /s /q C:\\Windows\\SoftwareDistribution\\Download > NUL 2>&1");
	    cout << "Done";
	    pause_on_exit();
	}
}

void system_info()
{
   clear();
   system("mode con: cols=120 lines=9001 && systeminfo");
   pause_on_exit();
}
