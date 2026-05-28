#include <iostream>
#include <string>
#include <windows.h>

int main(int argc, char* argv[])
{
    if (argc==1) {
        std::cout << "su binary for Windows v0.2" << '\n';
        std::cout << "Usage: su [-][Option...]" << '\n';
        std::cout << ">HELP GUIDE<" << '\n';
        std::cout << "    - : Creates a terminal under SYSTEM account. When no desktop session is present," << '\n'
        << "        the shell will run inline under the current terminal.";
        std::cout << "    [...] : Runs the following command under Administrator previleges." << '\n';
        std::cout << ">TIPS<" << '\n';
        std::cout << "    It is strongly recommended that you enable password-enabled User Account Control and ensure it shall runs under Session 0, in case of a present security vulnerability.";
        std::cout << '\n' << ">CHANGELOGS<" << '\n';
        std::cout << "     v0.1 : Added initial binary." << '\n';
        std::cout << "     v0.2 : Added help text.";
    }
    else if (argc==2 && (std::string)argv[1]=="-")
        system("E:\\wmsudo\\bin\\sudo64.exe E:\\sysinternals\\PSTools\\psexec.exe -s cmd.exe");
    else {
        std::string _command_ = "E:\\wmsudo\\bin\\sudo.exe";
        for (int i=1; i<argc; i++) {
            _command_ += " ";
            _command_ += "\"";
            _command_ += argv[i];
            _command_ += "\"";
        }
        system(_command_.c_str());
    }

    return 0;
}