#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <locale>
#include <VersionHelpers.h>
#include <filesystem>
#include <io.h>
#include <map>

using namespace std;

void clear();
void pause_on_exit();
void pause_on_continue();
void system_info();
void clearTrash();
void install_vcredist();
void clear_tmp();
void set_console_size();
void khanh_main();

std::wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

map<string, string> windows;
map<int, string> kms7;
map<int, string> kms8;
map<int, string> kms10;

struct stat info;

wstring PATH_a = ExePath();
const std::string PATH_str(PATH_a.begin(), PATH_a.end());
char PATH[255];
char PATH_EXEC[255];
string arch;
string test1;
bool is64 = stat("C:\\Windows\\SysWOW64", &info);
bool isarm64 = stat("C:\\Program Files (Arm)", &info);
bool network;

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
	cout << "\n Activating Windows...\n\n";
	system("cscript //nologo %windir%/system32/slmgr.vbs /upk > NUL 2>&1");
	system("cscript //nologo %windir%/system32/slmgr.vbs /cpky > NUL 2>&1");
	{
		string PATH_temp = "cscript //nologo %windir%/system32/slmgr.vbs /ipk " + key + " > NUL 2>&1";
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
	}
	{
		string PATH_temp = "cscript //nologo %windir%/system32/slmgr.vbs /skms " + kms + " > NUL 2>&1";
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		system(PATH_EXEC);
		bool active = system("cscript //nologo %windir%/system32/slmgr.vbs /ato");
		return active;
	}
}

int test_ping(string link) {
	{
		bool return_code = exec("ping -n 1 " + link + " > NUL 2>&1");
		return return_code;
	}
}

int activate_windows(int id, string win, int count) {
	{
		clear();
		cout << "Test KMS Server...\n";
		for (int i = 1; i < 5; i++) {
			string a;
			if (id == 7) {
				a = kms7[i];
			}
			else if (id == 8) {
				a = kms8[i];
			}
			else if (id == 10) {
				a = kms10[i];
			}
			if (test_ping(a) == 0) {
				count = i;
				break;
			}
		}
		if (count == 0) {
			cout << "All server KMS is die!\n";
			pause_on_exit();
			return 1;
		}
		else {
			clear();
			bool return_code;
			if (id == 7) {
				return_code = active_windows(windows[win], kms7[count]);
			}
			else if (id == 8) {
				return_code = active_windows(windows[win], kms8[count]);
			}
			else if (id == 10) {
				return_code = active_windows(windows[win], kms10[count]);
			}
			pause_on_exit();
			return return_code;
		}
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
		bool return_code = exec_file(exec);
		printf("Done\n");
		if (iscontinue != 0) {
			pause_on_exit();
		}
		return return_code;
	}
}

int main(){
first:
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
	}
	else {
		strcpy_s(PATH, PATH_str.c_str());
	}
windows:
	windows["win7pro"] = "FJ82H-XT6CR-J8D7P-XQJJ2-GPDD4";
	windows["win7proN"] = "MRPKT-YTG23-K7D7T-X2JMM-QY7MG";
	windows["win7proE"] = "W82YF-2Q76Y-63HXB-FGJG9-GF7QX";
	windows["win7enter"] = "33PXH-7Y6KF-2VJC9-XBBR8-HVTHH";
	windows["win7enterN"] = "YDRBP-3D83W-TY26F-D46B2-XCKRJ";
	windows["win7enterE"] = "C29WB-22CC8-VJ326-GHFJW-H9DH4";
	windows["win8pro"] = "NG4HW-VH26C-733KW-K6F98-J8CK4";
	windows["win8proN"] = "XCVCF-2NXM9-723PB-MHCB7-2RYQQ";
	windows["win8enter"] = "32JNW-9KQ84-P47T8-D8GGY-CWCK7";
	windows["win8enterN"] = "JMNMF-RHW7P-DMY6X-RF3DR-X2BQT";
	windows["win81pro"] = "GCRJD-8NW9H-F2CDX-CCM8D-9D6T9";
	windows["win81proN"] = "HMCNV-VVBFX-7HMBH-CTY9B-B4FXY";
	windows["win81enter"] = "MHF9N-XY6XB-WVXMC-BTDCT-MKKG7";
	windows["win81enterN"] = "TT4HM-HN7YT-62K67-RGRQJ-JFFXW";
	windows["win10pro"] = "W269N-WFGWX-YVC9B-4J6C9-T83GX";
	windows["win10proN"] = "MH37W-N47XK-V7XM9-C7227-GCQG9";
	windows["win10enter"] = "NPPR9-FWDCX-D2C8J-H872K-2YT43";
	windows["win10enterN"] = "DPH2V-TTNVB-4X9Q3-TJR4H-KHJW4";
	windows["win10edu"] = "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
	windows["win10eduN"] = "2WH4N-8QGBV-H22JP-CT43Q-MDWWJ";
	windows["win10edu"] = "WNMTR-4C88C-JK8YV-HQ7T2-76DF9";
	windows["win10edu"] = "2F77B-TNFGY-69QQF-B8YKP-D69TJ";
	windows["win10edu"] = "DCPHK-NFMTC-H88MJ-PFHPY-QJ4BJ";
	windows["win10edu"] = "QFFDN-GRT3P-VKWWX-X7T3R-8B639";
	windows["win10enter2015ltsc"] = "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
	windows["win10enter2015ltscN"] = "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
	windows["win10enter2016ltsc"] = "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
	windows["win10enter2016ltscN"] = "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
	windows["win10prowork"] = "NRG8B-VKK3Q-CXVCJ-9G2XF-6Q84J";
	windows["win10proworkN"] = "9FNHH-K3HBT-3W4TD-6383H-6XYWF";
kms7:
	kms7[1] = "s8.now.im";
	kms7[2] = "s9.now.im";
kms8:
	kms8[1] = "kms8.MSGuides.com";
	kms8[2] = "kms9.MSGuides.com";
	kms8[3] = "kms.digiboy.ir";
kms10:
	kms10[1] = "kms8.MSGuides.com";
	kms10[2] = "kms9.MSGuides.com";
	kms10[3] = "kms.digiboy.ir";
check:
	cout << "Testing internet....\n";
	if (test_ping("google.com") == 0) {
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
	khanh_main();
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
	khanh_main();
	cout << "\n 1. Windows 7\n 2. Windows 8\n 3. Windows 8.1\n 4. Windows 10/11\n 0. Exit\n\n Your choose: ";
	cin >> khanh;
	switch (khanh) {
	case 1:
		clear();
		khanh_main();
		cout << "\n 1. Windows 7 Professional\n 2. Windows 7 Professional N\n 3. Windows 7 Professional E\n 4. Windows 7 Enterprise\n 5. Windows 7 Enterprise N\n 6. Windows 7 Enterprise E\n 0. Exit\n\n Your choose: ";
		cin >> khanh;
		switch (khanh) {
		case 1:
			activate_windows(7, "win7pro", 0);
			goto activate_win;
		case 2:
			activate_windows(7, "win7proN", 0);
			goto activate_win;
		case 3:
			activate_windows(7, "win7proE", 0);
			goto activate_win;
		case 4:
			activate_windows(7, "win7enter", 0);
			goto activate_win;
		case 5:
			activate_windows(7, "win7enterN", 0);
			goto activate_win;
		case 6:
			activate_windows(7, "win7enterE", 0);
			goto activate_win;
		}
		goto activate_win;
	case 2:
		activate_windows(8, "win8", 0);
		goto activate_win;
	case 3:
		activate_windows(8, "win81", 0);
		goto activate_win;
	case 4:
		activate_windows(8, "win10", 0);
		goto activate_win;
	case 0:
		goto main_menu;
		break;
	}
	goto activate_win;
framework:
	clear();
	khanh_main();
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
	khanh_main();
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

void khanh_main() {
	cout << "Menu by KhanhNguyen9872\n";
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
