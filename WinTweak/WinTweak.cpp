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
	system("cscript //nologo %windir%/system32/slmgr.vbs /upk");
	system("cscript //nologo %windir%/system32/slmgr.vbs /cpky");
	{
		string PATH_temp = "cscript //nologo %windir%/system32/slmgr.vbs /ipk " + key;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
	}
	{
		string PATH_temp = "cscript //nologo %windir%/system32/slmgr.vbs /skms " + kms;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
		bool active = system("cscript //nologo %windir%/system32/slmgr.vbs /ato");
		return active;
	}
}

int activate_windows(string win) {
	{
		clear();
		active_windows("W269N-WFGWX-YVC9B-4J6C9-T83GX", "kms9.MSGuides.com:1688");
		pause_on_exit();
	}
}

int download_and_install(bool isclear, bool iscontinue, string projectname, string link, string filename, string exec) {
	if (check_exist(PATH_str + "\\tools\\wget.exe") != 0) {
		cout << "wget not found";
		pause_on_exit();
	}
	else {
		if (isclear == 0) {
			clear();
			clear_tmp();
		}
		cout << "\n Downloading " << projectname << "...\n";
		download_file(link, filename);
		cout << "\n Installing " << projectname << "... ";
		exec_file(exec);
		printf("Done\n");
		if (iscontinue != 0) {
			pause_on_exit();
		}
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
				download_and_install(0, 1, "DirectX", "https://tinyurl.com/55824f4a", "directx.exe", "directx.exe");
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
		activate_windows("win7");
		goto activate_win;
	case 2:
		activate_windows("win8");
		goto activate_win;
	case 3:
		activate_windows("win81");
		goto activate_win;
	case 4:
		activate_windows("win10");
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
			download_and_install(0, 1, "Framework 4.8", "https://tinyurl.com/2vb2fudw", "Framework48.exe", "Framework48.exe /q /norestart");
			goto framework;
		case 0:
			goto main_menu;
			break;
	}
	goto framework;
office:
	clear();
	cout << "Menu by KhanhNguyen9872\n";
	cout << "\n 1. Office ProPlus LTSC 2021\n 2. Office 365\n 0. Exit\n\n Your choose: ";
	cin >> khanh;
	switch (khanh) {
		case 1:
			download_and_install(0, 1, "Office ProPlus LTSC 2021", "https://tinyurl.com/22kbfheb", "OfficePPLTSC2021.exe", "OfficePPLTSC2021.exe");
			goto office;
		case 2:
			download_and_install(0, 1, "Office 365", "https://tinyurl.com/38w39nj4", "Office365.exe", "Office365.exe");
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

void install_vcredist() {
	download_and_install(0, 0, "vcredist x86", "https://tinyurl.com/4b75en7n", "vcredist_x86.exe", "vcredist_x86.exe /install /quiet /norestart");
	if (arch == "x86_64" || arch == "ARM64") {
		printf ("============");
		download_and_install(1, 0, "vcredist x64", "https://tinyurl.com/bdzea7wf", "vcredist_x64.exe", "vcredist_x64.exe /install /quiet /norestart");
	}
	if (arch == "ARM64") {
		printf ("============");
		download_and_install(1, 0, "vcredist ARM", "https://tinyurl.com/wbzj2sc7", "vcredist_ARM.exe", "vcredist_ARM.exe /install /quiet /norestart");
		printf ("============");
		download_and_install(1, 0, "vcredist ARM64", "https://tinyurl.com/ys649mw9", "vcredist_ARM64.exe", "vcredist_ARM64.exe /install /quiet /norestart");
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
