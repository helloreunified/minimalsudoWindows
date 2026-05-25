#include <windows.h>
#include <stdio.h>
#include <string.h>

BOOL Is64BitWindows(void)
{
#if defined(_WIN64)
    return TRUE;
#else
    BOOL wow64 = FALSE;

    typedef BOOL (WINAPI *FN)(HANDLE, PBOOL);

    FN fn = (FN)GetProcAddress(
        GetModuleHandleA("kernel32.dll"),
        "IsWow64Process");

    if (fn)
        fn(GetCurrentProcess(), &wow64);

    return wow64;
#endif
}

char *GetArguments(void)
{
    char *cmd = GetCommandLineA();
    char *p = cmd;

    if (*p == '"') {
        p++;
        while (*p && *p != '"')
            p++;
        if (*p == '"')
            p++;
    } else {
        while (*p && *p != ' ')
            p++;
    }

    while (*p == ' ')
        p++;

    return p;
}

int main(void)
{
    char self[MAX_PATH];
    char dir[MAX_PATH];
    char target[MAX_PATH];
    char cmd[8192];

    GetModuleFileNameA(NULL, self, sizeof(self));

    strcpy(dir, self);

    char *slash = strrchr(dir, '\\');
    if (slash)
        *slash = '\0';

    snprintf(
        target,
        sizeof(target),
        "%s\\%s",
        dir,
        Is64BitWindows()
            ? "sudo64.exe"
            : "sudo32.exe");

    char *args = GetArguments();

    if (*args)
        snprintf(cmd, sizeof(cmd),
                 "\"%s\" %s",
                 target,
                 args);
    else
        snprintf(cmd, sizeof(cmd),
                 "\"%s\"",
                 target);

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};

    si.cb = sizeof(si);

    if (!CreateProcessA(
            NULL,
            cmd,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        printf("Failed to launch.\n");
        return 1;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}