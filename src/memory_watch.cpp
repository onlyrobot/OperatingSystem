# include<windows.h>
# include<iostream>
# include<cstdio>
# include<psapi.h>
# include<tlhelp32.h>
# include<shlwapi.h>
# include<conio.h>
# include<cstring>
# include<winnt.h>
using namespace std;

HANDLE stdOutput;
#define GB (1024*1024*1024)
#define KB (1024)


void memory(){
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    printf("                   **********SYSTEM MEMORY INFORMATION********                              \n\n");
    printf("                          Total memory             :   %.2fGB                               \n",(float)statex.ullTotalPhys/GB);
    printf("                           Used memory             :   %ld%%                                \n",statex.dwMemoryLoad);
    printf("                         Available memory          :   %.2fGB                               \n",(float)statex.ullAvailPhys/GB);
    printf("                           Total pages             :   %.2fGB                               \n",(float)statex.ullTotalPageFile/GB);
    printf("                         Available pages           :   %.2fGB                               \n",(float)statex.ullAvailPageFile/GB);
    printf("                        Total virtual space        :   %.2fGB                               \n",(float)statex.ullTotalVirtual/GB);
    printf("                       Available virtual space     :   %.2fGB                               \n",(float)statex.ullAvailVirtual/GB);
    printf("                   Available extended virtual space:   %.2fGB                               \n",(float)statex.ullAvailExtendedVirtual/GB);
}

void system(){
    SYSTEM_INFO si;
    ZeroMemory(&si,sizeof(si));
    GetSystemInfo(&si);
    char architecture[100] = {0};
    switch (si.wProcessorArchitecture)
    {
    	case PROCESSOR_ARCHITECTURE_AMD64: strcpy(architecture, "x64 (AMD or Intel)"); break;
	    case PROCESSOR_ARCHITECTURE_ARM:strcpy(architecture, "ARM"); break;
        // case PROCESSOR_ARCHITECTURE_ARM64:strcpy(architecture, "ARM64"); break;
        case PROCESSOR_ARCHITECTURE_IA64:strcpy(architecture, "Intel Itanium-based"); break;
        case PROCESSOR_ARCHITECTURE_INTEL:strcpy(architecture, "x86"); break;
        case PROCESSOR_ARCHITECTURE_UNKNOWN:strcpy(architecture, "Unknown architecture"); break;
    }
    printf("                      **********THE SYSTEM INFORMATION**********                            \n\n");
    printf("                            Process architecture    : %s                                    \n",architecture);
    printf("                                 Page size          : %dKB                                  \n",(int)si.dwPageSize/KB);
    printf("                        Minimal application address : 0x%.8x                                \n",si.lpMinimumApplicationAddress);//%.8x控制长度
    printf("                        Maximum application address : 0x%x                                  \n",si.lpMaximumApplicationAddress);
    printf("                             Number of processors   : %d                                    \n",si.dwNumberOfProcessors);
    printf("                            Allocation granularity  : %dKB                                  \n",si.dwAllocationGranularity/KB);
}

void performance(){
    PERFORMANCE_INFORMATION pi;
    pi.cb=sizeof(pi);
    GetPerformanceInfo(&pi, sizeof(pi));
    printf("                         ***THE PERFORMANCE INFORMATION***                                 \n\n");
    printf("                             Total commit pages  : %d                                      \n",pi.CommitTotal);
    printf("                               Commit limit      : %d                                      \n",pi.CommitLimit);
    printf("                                Commit peak      : %d                                      \n",pi.CommitPeak);
    printf("                           Total physical pages  : %d                                      \n",pi.PhysicalTotal);
    printf("                         Available physical pages: %d                                      \n",pi.PhysicalAvailable);
    printf("                                System cache     : %d                                      \n",pi.SystemCache);
    printf("                            Total kernel pages   : %d                                      \n",pi.KernelTotal);
    printf("                             Kernel non-paged    : %d                                      \n",pi.KernelNonpaged);
    printf("                                 Page size       : %dKB                                    \n",pi.PageSize/KB);
    printf("                              Opened handles     : %d                                      \n",pi.HandleCount);
    printf("                               Process count     : %d                                      \n",pi.ProcessCount);
    printf("                               Thread count      : %d                                      \n",pi.ThreadCount);
}

void process(){
    PROCESSENTRY32 pe;
    pe.dwSize=sizeof(pe);
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bMore = ::Process32First(hProcessSnap, &pe);
    printf("***********************************Process Information*********************************** \n\n");
    printf(" PID\t|  Target Execute File\t| Size(KB)\t\tPID\t|  Target Execute File\t| Size(KB) \n");
    int flag = 0;
    while(bMore){
        HANDLE hP = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
        PROCESS_MEMORY_COUNTERS pmc;
		ZeroMemory(&pmc, sizeof(pmc));
        if(GetProcessMemoryInfo(hP, &pmc, sizeof(pmc))){
            int len=strlen((const char*)pe.szExeFile);
            printf("%d\t| ",pe.th32ProcessID);
            int temp = 0;
            for (int i = 0; i < 18; ++i)
            {
                if (pe.szExeFile[i] != '\0')
                    printf("%c", pe.szExeFile[i]);
                else
                {
                    while (i < 18)
                    {
                        printf(" ");
                        i++;
                    }
                    temp = 1;
                    break;
                }
            }
            if (temp == 0)
                printf("...\t");
            else
                printf("\t");
            printf("| %.4f",(float)pmc.WorkingSetSize/KB);
            if (flag == 0)
            {
                printf("\t||\t");
                flag = 1;
            }
            else
            {
                printf("\n");
                flag = 0;
            }
        }
        bMore = ::Process32Next(hProcessSnap, &pe);
    }
    CloseHandle(hProcessSnap);
}


inline bool TestSet(DWORD dwTarget,DWORD dwMask){
    return ((dwTarget &dwMask)==dwMask);
}

#define SHOWMASK(dwTarget,type) if(TestSet(dwTarget,PAGE_##type)) {cout << "|" << #type;}

void ShowProtection(DWORD dwTarget){
    SHOWMASK(dwTarget,READONLY);
    SHOWMASK(dwTarget,GUARD);
    SHOWMASK(dwTarget,NOCACHE);
    SHOWMASK(dwTarget,READWRITE);
    SHOWMASK(dwTarget,WRITECOPY);
    SHOWMASK(dwTarget,EXECUTE_READ);
    SHOWMASK(dwTarget,EXECUTE);
    SHOWMASK(dwTarget,EXECUTE_READWRITE);
    SHOWMASK(dwTarget,EXECUTE_WRITECOPY);
    SHOWMASK(dwTarget,NOACCESS);
}

void process_virtual_memory(){
    int pid;
    SYSTEM_INFO si;
    ZeroMemory(&si,sizeof(si));
    GetSystemInfo(&si);
    
    STARTUPINFO ssi;
    memset(&ssi, 0, sizeof(STARTUPINFO));
    ssi.dwFlags = STARTF_USESHOWWINDOW;
    ssi.wShowWindow = SW_SHOW;
    PROCESS_INFORMATION pi;
    CreateProcess((TCHAR*)"./timer.exe", (TCHAR*)"10000", NULL, NULL, FALSE, 0, NULL, NULL, &ssi, &pi);

    MEMORY_BASIC_INFORMATION mbi;
    ZeroMemory(&mbi,sizeof(mbi));
    LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
    printf("Address\t\t  | Size\t| State\t\t|    Protect   \t\t|   Type  | Module\n\n");
    while(pBlock < si.lpMaximumApplicationAddress){
        if (VirtualQueryEx(pi.hProcess,pBlock,&mbi,sizeof(mbi))==sizeof(mbi)) {
            LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
            TCHAR szSize[MAX_PATH];
            StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
            printf("%.8x-%.8x ",(DWORD)pBlock,(DWORD)pEnd);
            printf("| %s\t",szSize);
            switch (mbi.State)
            {
                case MEM_COMMIT:
                    printf("| Commited\t");
                    break;
                case MEM_FREE:
                    printf("| Free    \t");
                    break;
                case MEM_RESERVE:
                    printf("| Reserved\t");
                    break;
                default:
                    break;
            }
            if (mbi.Protect == 0 && mbi.State != MEM_FREE) {
                mbi.Protect = PAGE_READONLY;
            }
            ShowProtection(mbi.Protect);
            printf("       \t");
            switch (mbi.Type)
            {
                case MEM_IMAGE:
                    printf("| Image   ");
                    break;
                case MEM_MAPPED:
                    printf("| Mapped  ");
                    break;
                case MEM_PRIVATE:
                    printf("| Private ");
                    break;
                default:
                    printf("|         ");
                    break;
            }
            TCHAR szFilename[MAX_PATH];
            if (GetModuleFileName((HINSTANCE)pBlock,szFilename,MAX_PATH)>0) {
                PathStripPath(szFilename);
                printf("\t| %s                                    ",szFilename);
            }
            printf("\n");
            pBlock=pEnd;
        }
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main(int argc, char const *argv[])
{
    char state = 0;
    stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    // CONSOLE_CURSOR_INFO cci; //隐藏系统光标
	// cci.bVisible = 0;
	// cci.dwSize = 1;
	// SetConsoleCursorInfo(stdOutput, &cci);

    printf("1. Memory  2. System  3. Performance  4. Process  5. Process Virtual Memory  6. Quit\n\n");
    while (true)
    {
        if (_kbhit()) //如果用户有输入
        {
            state = getch();
            system("cls");
            printf("1. Memory  2. System  3. Performance  4. Process  5. Process Virtual Memory  6. Quit\n\n");
        }
        else
            Sleep(500);
        COORD coord = { 0, 2 };
	    SetConsoleCursorPosition(stdOutput, coord);
        void (*display)() = NULL;
        switch (state)
        {
            case '1': display = memory; break;
            case '2': display = system; break;
            case '3': display = performance; break;
            case '4': display = process; break;
            case '5': display = process_virtual_memory; break;
            case '6': return 0;
            default: break;
        }
        if (display != NULL)
            display();
    }
    return 0;
}

