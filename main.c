#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <winternl.h>

typedef LONG (WINAPI *RtlGetVersionPtr)(RTL_OSVERSIONINFOW *);

void PrintWindowsVersion(void)
{
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (!hMod) return;

    RtlGetVersionPtr fxPtr =
        (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
    if (!fxPtr) return;

    RTL_OSVERSIONINFOW rovi = {0};
    rovi.dwOSVersionInfoSize = sizeof(rovi);

	printf("Microsoft Windows [Version ");

    if (fxPtr(&rovi) == 0) {
        printf("%lu.%lu.%lu",
               rovi.dwMajorVersion,
               rovi.dwMinorVersion,
               rovi.dwBuildNumber);
    }

	printf("]\n");
}

int main()
{
	PrintWindowsVersion();

    char *fullCmd = GetCommandLineA();

    char *args = fullCmd;
    if (*args == '"') {
        args++;
        while (*args && *args != '"') args++;
        if (*args == '"') args++;
    } else {
        while (*args && *args != ' ') args++;
    }
    while (*args == ' ') args++;

    if (*args == '\0') {
        printf("Not specified.");
        return 1;
    }

    char cwd[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, cwd);

    char cmdLine[8192];
    snprintf(cmdLine, sizeof(cmdLine),
    "/k cd /d \"%s\" && "
    "%s & "
    "echo. & "
    "echo The process terminated with exit code %%ERRORLEVEL%%. & "
    "echo Press ENTER to exit... & "
    "pause >nul & exit",
    cwd, args);

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "runas";
    sei.lpFile = "cmd.exe";
    sei.lpParameters = cmdLine;
    sei.nShow = SW_SHOW;

    if (!ShellExecuteExA(&sei)) {
        printf("Elevation failed.\n");
        return 1;
    }

	CloseHandle(sei.hProcess);
    return 0;
}
