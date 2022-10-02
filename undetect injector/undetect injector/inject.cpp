#define injectorundetect
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <tchar.h>
using namespace std;

DWORD get_proc_id(const char* proc_name)
{
    DWORD proc_id = 0;
    auto* const h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (h_snap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 proc_entry;
        proc_entry.dwSize = sizeof(proc_entry);

        if (Process32First(h_snap, &proc_entry))
        {
            do
            {
                if (!_stricmp(proc_entry.szExeFile, proc_name))
                {
                    proc_id = proc_entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(h_snap, &proc_entry));
        }
    }

    CloseHandle(h_snap);
    return proc_id;
}
static const char rnum[] = "0123456789" "qwertyuiopasdfghjklzxcvbnm" "QWERTYUIOPASDFGHJKLZXCVBNM";
int staticLength = sizeof(rnum) - 1;
std::string RandomTitle;

char GetRandom() {
    return rnum[rand() % staticLength];
}

void SetRandomTitle() {

    srand(time(0));
    for (unsigned int i = 0; i < 40; ++i) {

        RandomTitle += GetRandom();
    }

    SetConsoleTitle(RandomTitle.c_str());
}

int main() {
    SetRandomTitle(); // Устанавливает рандомное название приложения

    _getch();
    return 0;
}
int main1()
{
    const char* dll_path = R"(C:\1.dll)";//Сюда имя dll
    const char* proc_name = "javaw.exe"; // Процесс игры
    DWORD proc_id = 0;

    while (!proc_id)
    {
        proc_id = get_proc_id(proc_name);
        Sleep(30);
    }

    auto* const h_proc = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);

    if (h_proc && h_proc != INVALID_HANDLE_VALUE)
    {
        const LPVOID nt_open_file = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");//ggez
        if (nt_open_file)
        {
            char original_bytes[5];
            memcpy(original_bytes, nt_open_file, 5);
            WriteProcessMemory(h_proc, nt_open_file, original_bytes, 5, nullptr);
        }

        auto* loc = VirtualAllocEx(h_proc, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(h_proc, loc, dll_path, strlen(dll_path) + 1, nullptr);
        auto* const h_thread = CreateRemoteThread(h_proc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), loc, 0, nullptr);

        if (h_thread) CloseHandle(h_thread);
    }

    if (h_proc) CloseHandle(h_proc);

    return 0;
}
