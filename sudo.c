#include <windows.h>
#include <stdio.h>

BOOL Is64BitWindows(void)
{
    BOOL wow64 = FALSE;

    typedef BOOL (WINAPI *FN)(HANDLE, PBOOL);

    FN fn = (FN)GetProcAddress(
        GetModuleHandleA("kernel32.dll"),
        "IsWow64Process");

    if (fn)
        fn(GetCurrentProcess(), &wow64);

    return wow64;
}

int main(void)
{
    char full[MAX_PATH + 8192];

    const char *exe =
        Is64BitWindows()
        ? "sudo64.exe"
        : "sudo32.exe";

    snprintf(
        full,
        sizeof(full),
        "\"%s\" %s",
        exe,
        GetCommandLineA());

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};

    si.cb = sizeof(si);

    if (!CreateProcessA(
            NULL,
            full,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        printf("Launch failed.\n");
        return 1;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}