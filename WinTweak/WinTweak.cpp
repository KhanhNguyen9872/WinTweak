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
void check_activate();

std::wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

map<short, string> lib;
map<string, string> windows;
map<short, string> kms7;
map<short, string> kms8;
map<short, string> kms10;

struct stat info;

wstring PATH_a = ExePath();
const std::string PATH_str(PATH_a.begin(), PATH_a.end());
char PATH[255];
char PATH_EXEC[255];
string arch;
string test1;
string slmgr = "cscript //nologo %windir%/system32/slmgr.vbs";
bool is64 = stat("C:\\Windows\\SysWOW64", &info);
bool isarm64 = stat("C:\\Program Files (Arm)", &info);
bool network;

string read_file(string filename)
{
	stringstream buffer;
	buffer << ifstream(filename).rdbuf();
	return buffer.str();
}

bool exec(string command) {
	{
		string PATH_temp = command;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		return system(PATH_EXEC);
	}
}

string exec_output(string command, short output_type) {
	{
		string output;
		// 0 = all output; 1 = output normal; 2 = output error;
		if (output_type == 0) {
			output = " > " + PATH_str + "\\tmp\\tmp.output 2>&1";
		} else if (output_type == 1) {
			output = " > " + PATH_str + "\\tmp\\tmp.output 2> NUL";
		}
		else if (output_type == 2) {
			output = " 2> " + PATH_str + "\\tmp\\tmp.output > NUL";
		}
		exec(command + output);
		string output_str = read_file(PATH_str + "\\tmp\\tmp.output");
		exec("del /f " + PATH_str + "\\tmp\\tmp.output > NUL 2>&1");
		return output_str;
	}
}


bool exec_file(string filename) {
    {
		string PATH_temp = "cd " + PATH_str + "\\tmp && " + filename;
		return exec(PATH_temp);
	}
}

bool fileExists(const std::string& file) {
	struct stat buf;
	return (stat(file.c_str(), &buf) == 0);
}

bool check_exist(string pathf) {
	{
		string PATH_temp = pathf;
		strcpy_s(PATH_EXEC, PATH_temp.c_str());
		return stat(PATH_EXEC, &info);
	}
}

bool check_library() {
library:
	lib[1] = "\\tools";
	lib[2] = "\\tools\\awk.exe";
	lib[3] = "\\tools\\curl.exe";
	lib[4] = "\\tools\\curl-ca-bundle.crt";
	lib[5] = "\\tools\\grep.exe";
	lib[6] = "\\tools\\libcurl.dll";
	lib[7] = "\\tools\\libeay32.dll";
	lib[8] = "\\tools\\libiconv2.dll";
	lib[9] = "\\tools\\libintl3.dll";
	lib[10] = "\\tools\\libssl32.dll";
	lib[11] = "\\tools\\pcre3.dll";
	lib[12] = "\\tools\\regex2.dll";
	lib[13] = "\\tools\\sed.exe";
	lib[14] = "\\tools\\wget.exe";
	{
		short b = lib.size() + 1;
		for (int i = 1; i < b; i++) {
			if (check_exist(PATH_str + lib[i]) != 0) {
				cout << "\nsome library is missing! [" << lib[i] << "]\n";
				pause_on_exit();
				exit(1);
			}
		}
	}
}

bool download_file(string link, string filename) {
	{
		check_library();
		return exec("cd " + PATH_str + "\\tmp && .\\..\\tools\\wget.exe --no-check-certificate -q --show-progress -O " + filename + " " + link);
	}
}

bool active_windows(string key, string kms) {
	cout << "\n Activating Windows...\n\n";
	exec(slmgr + " /upk > NUL 2>&1");
	exec(slmgr + " /cpky > NUL 2>&1");
	exec(slmgr + " /ipk " + key + " > NUL 2>&1");
	exec(slmgr + " /skms " + kms + " > NUL 2>&1");
	{
		return exec(slmgr + " /ato");
	}
}

bool test_ping(string link) {
	{
		return exec("ping -n 1 " + link + " > NUL 2>&1");
	}
}

bool activate_windows(int id, string win, int count) {
windows:
	//win7
	windows["win7pro"] = "FJ82H-XT6CR-J8D7P-XQJJ2-GPDD4";
	windows["win7proN"] = "MRPKT-YTG23-K7D7T-X2JMM-QY7MG";
	windows["win7proE"] = "W82YF-2Q76Y-63HXB-FGJG9-GF7QX";
	windows["win7ul"] = "RHTBY-VWY6D-QJRJ9-JGQ3X-Q2289";
	windows["win7embeddedpos7"] = "YBYF6-BHCR3-JPKRB-CDW7B-F9BK4";
	windows["win7embeddedstd"] = "XGY72-BRBBT-FF8MH-2GG8H-W7KCW";
	windows["win7embeddedthinpc"] = "73KQT-CD9G6-K7TQG-66MRP-CQ22C";
	windows["win7enter"] = "33PXH-7Y6KF-2VJC9-XBBR8-HVTHH";
	windows["win7enterN"] = "YDRBP-3D83W-TY26F-D46B2-XCKRJ";
	windows["win7enterE"] = "C29WB-22CC8-VJ326-GHFJW-H9DH4";
	//win8
	windows["win8core"] = "BN3D2-R7TKB-3YPBD-8DRP2-27GG4";
	windows["win8coreN"] = "8N2M2-HWPGY-7PGT9-HGDD8-GVGGY";
	windows["win8coresingle"] = "2WN2H-YGCQR-KFX6K-CD6TF-84YXQ";
	windows["win8corechina"] = "4K36P-JN4VD-GDC6V-KDT89-DYFKP";
	windows["win8corearm"] = "DXHJF-N9KQX-MFPVR-GHGQK-Y7RKV";
	windows["win8promedia"] = "GNBB8-YVD74-QJHX6-27H4K-8QHDG";
	windows["win8embeddedpro"] = "RYXVT-BNQG7-VD29F-DBMRY-HT73M";
	windows["win8embeddedenter"] = "NKB3R-R2F8T-3XCDP-7Q2KW-XWYQ2";
	windows["win8pro"] = "NG4HW-VH26C-733KW-K6F98-J8CK4";
	windows["win8proN"] = "XCVCF-2NXM9-723PB-MHCB7-2RYQQ";
	windows["win8enter"] = "32JNW-9KQ84-P47T8-D8GGY-CWCK7";
	windows["win8enterN"] = "JMNMF-RHW7P-DMY6X-RF3DR-X2BQT";
	//win8.1
	windows["win81core"] = "M9Q9P-WNJJT-6PXPY-DWX8H-6XWKK";
	windows["win81coreN"] = "7B9N3-D94CG-YTVHR-QBPX3-RJP64";
	windows["win81coresingle"] = "BB6NG-PQ82V-VRDPW-8XVD2-V8P66";
	windows["win81corechina"] = "NCTT7-2RGK8-WMHRF-RY7YQ-JTXG3";
	windows["win81corearm"] = "XYTND-K6QKT-K2MRH-66RTM-43JKP";
	windows["win81promedia"] = "789NJ-TQK6T-6XTH8-J39CJ-J8D3P";
	windows["win81embeddedpro"] = "NMMPB-38DD4-R2823-62W8D-VXKJB";
	windows["win81embeddedenter"] = "FNFKF-PWTVT-9RC8H-32HB2-JB34X";
	windows["win81embeddedauto"] = "VHXM3-NR6FT-RY6RT-CK882-KW2CJ";
	windows["win81bing"] = "3PY8R-QHNP9-W7XQD-G6DPH-3J2C9";
	windows["win81bingN"] = "Q6HTR-N24GM-PMJFP-69CD8-2GXKR";
	windows["win81bingsingle"] = "KF37N-VDV38-GRRTV-XH8X6-6F3BB";
	windows["win81bingchina"] = "R962J-37N87-9VVK2-WJ74P-XTMHR";
	windows["win81prostudent"] = "MX3RK-9HNGX-K3QKC-6PJ3F-W8D7B";
	windows["win81prostudentN"] = "TNFGH-2R6PB-8XM3K-QYHX2-J4296";
	windows["win81pro"] = "GCRJD-8NW9H-F2CDX-CCM8D-9D6T9";
	windows["win81proN"] = "HMCNV-VVBFX-7HMBH-CTY9B-B4FXY";
	windows["win81enter"] = "MHF9N-XY6XB-WVXMC-BTDCT-MKKG7";
	windows["win81enterN"] = "TT4HM-HN7YT-62K67-RGRQJ-JFFXW";
	//win10
	windows["win10home"] = "TX9XD-98N7V-6WMQ6-BX7FG-H8Q99";
	windows["win10homeN"] = "3KHY7-WNT83-DGQKR-F7HPR-844BM";
	windows["win10homesingle"] = "7HNRX-D7KGG-3K4RQ-4WPJ4-YTDFH";
	windows["win10homechina"] = "PVMJN-6DFY6-9CCP6-7BKTT-D3WVR";
	windows["win10proedu"] = "6TP4R-GNPTD-KYYHQ-7B7DP-J447Y";
	windows["win10proeduN"] = "YVWGF-BXNMC-HTQYQ-CPQ99-66QFC";
	windows["win10enterltsc2019"] = "M7XTQ-FN8P6-TTKYV-9D4CC-J462D";
	windows["win10enterltsc2019N"] = "92NFX-8DJQP-P6BBQ-THF9C-7CG2H";
	windows["win10enterremoteserver"] = "7NBT4-WGBQX-MP4H7-QXFF8-YP3KX";
	windows["win10enterremotesession"] = "CPWHC-NT2C7-VYW78-DHDB2-PG3GK";
	windows["win10lean"] = "NBTWJ-3DR69-3C4V8-C26MC-GQ9M6";
	windows["win10pro"] = "W269N-WFGWX-YVC9B-4J6C9-T83GX";
	windows["win10proN"] = "MH37W-N47XK-V7XM9-C7227-GCQG9";
	windows["win10enter"] = "NPPR9-FWDCX-D2C8J-H872K-2YT43";
	windows["win10enterN"] = "DPH2V-TTNVB-4X9Q3-TJR4H-KHJW4";
	windows["win10enterG"] = "YYVX9-NTFWV-6MDM3-9PT4T-4M68B";
	windows["win10enterGN"] = "44RPN-FTY23-9VTTB-MP9BX-T84FV";
	windows["win10edu"] = "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
	windows["win10eduN"] = "2WH4N-8QGBV-H22JP-CT43Q-MDWWJ";
	windows["win10enter2015ltsb"] = "WNMTR-4C88C-JK8YV-HQ7T2-76DF9";
	windows["win10enter2015ltsbN"] = "2F77B-TNFGY-69QQF-B8YKP-D69TJ";
	windows["win10enter2016ltsb"] = "DCPHK-NFMTC-H88MJ-PFHPY-QJ4BJ";
	windows["win10enter2016ltsbN"] = "QFFDN-GRT3P-VKWWX-X7T3R-8B639";
	windows["win10prowork"] = "NRG8B-VKK3Q-CXVCJ-9G2XF-6Q84J";
	windows["win10proworkN"] = "9FNHH-K3HBT-3W4TD-6383H-6XYWF";
kms7:
	kms7[1] = "tinhve.vn";
	kms7[2] = "s8.now.im";
	kms7[3] = "s9.now.im";
kms8:
	kms8[1] = "kms8.MSGuides.com";
	kms8[2] = "kms9.MSGuides.com";
	kms8[3] = "kms.digiboy.ir";
kms10:
	kms10[1] = "kms8.MSGuides.com";
	kms10[2] = "kms9.MSGuides.com";
	kms10[3] = "kms.digiboy.ir";
	{
		string khanh;
		cout << "\n\nDo you want to activate it? [Y/N]: ";
		cin >> khanh;
		if (khanh == "Y" || khanh == "y") {
			clear();
			cout << "Test KMS Server...\n";
			string a; int b;
			if (id == 7) {
				b = kms7.size() + 1;
			}
			else if (id == 8) {
				b = kms8.size() + 1;
			}
			else if (id == 10) {
				b = kms10.size() + 1;
			}
			for (int i = 1; i < b; i++) {
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
}

bool download_and_install(bool isclear, bool iscontinue, string projectname, string link, string filename, string exec) {
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

BOOL if_admin() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}

short main(){
first:
	set_console_size();
	if (!if_admin()) {
		cout << "\nYou need run as administrator!\n";
		pause_on_exit();
		exit(1);
	} else if (!IsWindows7OrGreater())
	{
		cout << "\nYou need at least Windows 7 or later\n";
		pause_on_exit();
		exit(1);
	} else if (IsWindowsServer())
	{
		printf("\nYou are running this application on Windows Server!\nRecommend not to run this app\n");
		pause_on_continue();
	}

	check_library();

	if (PATH_str.length() > 128) {
		cout << "PATH directory to long [" << PATH_str.length() << "]!\nPlease change to another short PATH!\n Required: 128 or below!\n";
		pause_on_exit();
		exit(1);
	}
	else {
		strcpy_s(PATH, PATH_str.c_str());
	}

check:
	cout << "\nTesting internet....\n";
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
			goto main_menu;
		case 2:
			clearTrash();
			goto main_menu;
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
	cout << "\n 1. Check Activate\n 2. Windows 7\n 3. Windows 8\n 4. Windows 8.1\n 5. Windows 10/11\n 0. Back\n\n Your choose: ";
	cin >> khanh;
	switch (khanh) {
	case 1:
		check_activate();
		goto activate_win;
	case 2:
		clear();
		khanh_main();
		cout << "\n 1. Windows 7 Professional";
		cout << "\n 2. Windows 7 Professional N";
		cout << "\n 3. Windows 7 Professional E";
		cout << "\n 4. Windows 7 Enterprise";
		cout << "\n 5. Windows 7 Enterprise N";
		cout << "\n 6. Windows 7 Enterprise E";
		cout << "\n 7. Windows 7 Embedded POSReady 7";
		cout << "\n 8. Windows 7 Embedded Standard";
		cout << "\n 9. Windows 7 Embedded ThinPC";
		cout << "\n 0. Back\n\n Your choose : ";
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
		case 7:
			activate_windows(7, "win7embeddedpos7", 0);
			goto activate_win;
		case 8:
			activate_windows(7, "win7embeddedstd", 0);
			goto activate_win;
		case 9:
			activate_windows(7, "win7embeddedthinpc", 0);
			goto activate_win;
		}
		goto activate_win;
	case 3:
		clear();
		khanh_main();
		cout << "\n 1. Windows 8 Core";
		cout << "\n 2. Windows 8 Core N";
		cout << "\n 3. Windows 8 Core Single Language";
		cout << "\n 4. Windows 8 Core China";
		cout << "\n 5. Windows 8 Core ARM";
		cout << "\n 6. Windows 8 Pro";
		cout << "\n 7. Windows 8 Pro N";
		cout << "\n 8. Windows 8 Pro with Media Center";
		cout << "\n 9. Windows 8 Enterprise";
		cout << "\n10. Windows 8 Enterprise N";
		cout << "\n11. Windows 8 Embedded Industry Pro";
		cout << "\n12. Windows 8 Embedded Industry Enterprise";
		cout << "\n 0. Back\n\n Your choose : ";
		cin >> khanh;
		switch (khanh) {
		case 1:
			activate_windows(8, "win8core", 0);
			goto activate_win;
		case 2:
			activate_windows(8, "win8coreN", 0);
			goto activate_win;
		case 3:
			activate_windows(8, "win8coresingle", 0);
			goto activate_win;
		case 4:
			activate_windows(8, "win8corechina", 0);
			goto activate_win;
		case 5:
			activate_windows(8, "win8corearm", 0);
			goto activate_win;
		case 6:
			activate_windows(8, "win8pro", 0);
			goto activate_win;
		case 7:
			activate_windows(8, "win8proN", 0);
			goto activate_win;
		case 8:
			activate_windows(8, "win8promedia", 0);
			goto activate_win;
		case 9:
			activate_windows(8, "win8enter", 0);
			goto activate_win;
		case 10:
			activate_windows(8, "win8enterN", 0);
			goto activate_win;
		case 11:
			activate_windows(8, "win8embeddedpro", 0);
			goto activate_win;
		case 12:
			activate_windows(8, "win8embeddedpro", 0);
			goto activate_win;
		}
		goto activate_win;
	case 4:
		clear();
		khanh_main();
		cout << "\n 1. Windows 8.1 Core";
		cout << "\n 2. Windows 8.1 Core N";
		cout << "\n 3. Windows 8.1 Core Single Language";
		cout << "\n 4. Windows 8.1 Core China";
		cout << "\n 5. Windows 8.1 Core ARM";
		cout << "\n 6. Windows 8.1 Pro";
		cout << "\n 7. Windows 8.1 Pro N";
		cout << "\n 8. Windows 8.1 Pro with Media Center";
		cout << "\n 9. Windows 8.1 Enterprise";
		cout << "\n10. Windows 8.1 Enterprise N";
		cout << "\n11. Windows 8.1 Embedded Industry Pro";
		cout << "\n12. Windows 8.1 Embedded Industry Enterprise";
		cout << "\n13. Windows 8.1 Embedded Industry Automotive";
		cout << "\n14. Windows 8.1 with Bing";
		cout << "\n15. Windows 8.1 with Bing N";
		cout << "\n16. Windows 8.1 with Bing Single Language";
		cout << "\n17. Windows 8.1 with Bing China";
		cout << "\n18. Windows 8.1 Pro for Students";
		cout << "\n19. Windows 8.1 Pro for Students N";
		cout << "\n 0. Back\n\n Your choose : ";
		cin >> khanh;
		switch (khanh) {
		case 1:
			activate_windows(8, "win81core", 0);
			goto activate_win;
		case 2:
			activate_windows(8, "win81coreN", 0);
			goto activate_win;
		case 3:
			activate_windows(8, "win81coresingle", 0);
			goto activate_win;
		case 4:
			activate_windows(8, "win81corechina", 0);
			goto activate_win;
		case 5:
			activate_windows(8, "win81corearm", 0);
			goto activate_win;
		case 6:
			activate_windows(8, "win81pro", 0);
			goto activate_win;
		case 7:
			activate_windows(8, "win81proN", 0);
			goto activate_win;
		case 8:
			activate_windows(8, "win81promedia", 0);
			goto activate_win;
		case 9:
			activate_windows(8, "win81enter", 0);
			goto activate_win;
		case 10:
			activate_windows(8, "win81enterN", 0);
			goto activate_win;
		case 11:
			activate_windows(8, "win81embeddedpro", 0);
			goto activate_win;
		case 12:
			activate_windows(8, "win81embeddedenter", 0);
			goto activate_win;
		case 13:
			activate_windows(8, "win81embeddedauto", 0);
			goto activate_win;
		case 14:
			activate_windows(8, "win81bing", 0);
			goto activate_win;
		case 15:
			activate_windows(8, "win81bingN", 0);
			goto activate_win;
		case 16:
			activate_windows(8, "win81bingsingle", 0);
			goto activate_win;
		case 17:
			activate_windows(8, "win81bingchina", 0);
			goto activate_win;
		case 18:
			activate_windows(8, "win81prostudent", 0);
			goto activate_win;
		case 19:
			activate_windows(8, "win81prostudentN", 0);
			goto activate_win;
		}
		goto activate_win;
	case 5:
		clear();
		khanh_main();
		cout << "\n 1. Windows 10 Home";
		cout << "\n 2. Windows 10 Home N";
		cout << "\n 3. Windows 10 Home Single Language";
		cout << "\n 4. Windows 10 Home China";
		cout << "\n 5. Windows 10 Pro";
		cout << "\n 6. Windows 10 Pro N";
		cout << "\n 7. Windows 10 Pro Education";
		cout << "\n 8. Windows 10 Pro Education N";
		cout << "\n 9. Windows 10 Pro Workstation";
		cout << "\n10. Windows 10 Pro Workstation N";
		cout << "\n11. Windows 10 Education";
		cout << "\n12. Windows 10 Education N";
		cout << "\n13. Windows 10 Enterprise";
		cout << "\n14. Windows 10 Enterprise N";
		cout << "\n15. Windows 10 Enterprise G";
		cout << "\n16. Windows 10 Enterprise G N";
		cout << "\n17. Windows 10 Enterprise 2015 LTSB";
		cout << "\n18. Windows 10 Enterprise 2015 LTSB N";
		cout << "\n19. Windows 10 Enterprise 2016 LTSB";
		cout << "\n20. Windows 10 Enterprise 2016 LTSB N";
		cout << "\n21. Windows 10 Enterprise LTSC 2019";
		cout << "\n22. Windows 10 Enterprise LTSC 2019 N";
		cout << "\n23. Windows 10 Enterprise Remote Server";
		cout << "\n24. Windows 10 Enterprise Remote Sessions";
		cout << "\n25. Windows 10 Lean";
		cout << "\n 0. Back\n\n Your choose : ";
		cin >> khanh;
		switch (khanh) {
		case 1:
			activate_windows(10, "win10home", 0);
		case 2:
			activate_windows(10, "win10homeN", 0);
			goto activate_win;
		case 3:
			activate_windows(10, "win10homesingle", 0);
			goto activate_win;
		case 4:
			activate_windows(10, "win10homechina", 0);
			goto activate_win;
		case 5:
			activate_windows(10, "win10pro", 0);
			goto activate_win;
		case 6:
			activate_windows(10, "win10proN", 0);
			goto activate_win;
		case 7:
			activate_windows(10, "win10proedu", 0);
			goto activate_win;
		case 8:
			activate_windows(10, "win10proeduN", 0);
			goto activate_win;
		case 9:
			activate_windows(10, "win10prowork", 0);
			goto activate_win;
		case 10:
			activate_windows(10, "win10proworkN", 0);
			goto activate_win;
		case 11:
			activate_windows(10, "win10edu", 0);
			goto activate_win;
		case 12:
			activate_windows(10, "win10eduN", 0);
			goto activate_win;
		case 13:
			activate_windows(10, "win10enter", 0);
			goto activate_win;
		case 14:
			activate_windows(10, "win10enterN", 0);
			goto activate_win;
		case 15:
			activate_windows(10, "win10enterG", 0);
			goto activate_win;
		case 16:
			activate_windows(10, "win10enterGN", 0);
			goto activate_win;
		case 17:
			activate_windows(10, "win10enter2015ltsb", 0);
			goto activate_win;
		case 18:
			activate_windows(10, "win10enter2015ltsbN", 0);
			goto activate_win;
		case 19:
			activate_windows(10, "win10enter2016ltsb", 0);
			goto activate_win;
		case 20:
			activate_windows(10, "win10enter2016ltsbN", 0);
			goto activate_win;
		case 21:
			activate_windows(10, "win10enterltsc2019", 0);
			goto activate_win;
		case 22:
			activate_windows(10, "win10enterltsc2019N", 0);
			goto activate_win;
		case 23:
			activate_windows(10, "win10enterremoteserver", 0);
			goto activate_win;
		case 24:
			activate_windows(10, "win10enterremotesessions", 0);
			goto activate_win;
		case 25:
			activate_windows(10, "win10lean", 0);
			goto activate_win;
		}
		goto activate_win;
	case 0:
		goto main_menu;
		break;
	}
	goto activate_win;
framework:
	clear();
	khanh_main();
	cout << "\n 1. Framework 4.8\n 0. Back\n\n Your choose: ";
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
	cout << "\n 1. Office ProPlus LTSC 2021\n 2. Office 365\n 0. Back\n\n Your choose: ";
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
	exec("rmdir /q /s " + PATH_str + "\\tmp > NUL 2>&1");
	exec("mkdir " + PATH_str + "\\tmp > NUL 2>&1");
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

void check_activate()
{
	{
		clear();
		cout << "\n Checking activate...\n\n";
		if (exec(slmgr + " /xpr |findstr \"permanently\" >nul") == 0) {
			cout << " Windows da duoc kich hoat ban quyen Vinh Vien!\n";
		}
		else {
			exec(slmgr + " /xpr");
		}
		pause_on_exit();
	}
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
    cout << "\nThis process will clear all temp folder!\nDo you want to continue? [Y/N]: ";
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
