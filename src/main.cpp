#include <iostream>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "CursesProvider.h"

CursesProvider *curses;
std::string TMPDIR;
std::string HOME_PATH = getenv("HOME");

void atExitFunction(void){
    system(std::string("find " + std::string(HOME_PATH) + "/.config/feednix -type f -not -name \'config.json\' -and -not -name \'log.txt\' -delete 2> /dev/null").c_str());
    system(std::string("rm -R " + TMPDIR + " 2> /dev/null").c_str());
    if(curses != NULL)
        curses->cleanup();
    std::cout << "Aborted.\nCheck log for details." << std::endl;
}

void sighandler(int signum){
    system(std::string("find " + std::string(HOME_PATH) + "/.config/feednix -type f -not -name \'config.json\' -and -not -name \'log.txt\' -delete 2> /dev/null").c_str());
    signal(signum, SIG_DFL);
    if(curses != NULL)
        curses->cleanup();
    kill(getpid(), signum);
    std::cerr << "Aborted" << std::endl;
}

void printUsage();

int main(int argc, char **argv){
    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGSEGV, sighandler);
    atexit(atExitFunction);

    bool verboseEnabled = false;
    bool changeTokens = false;

    //Insure that config file and directory are present if not copy default into user home
    if(fopen(std::string(std::string(HOME_PATH) + "/.config/feednix/config.json").c_str(), "r") == NULL){
        system(("mkdir -p " + HOME_PATH + "/config/feednix &> /dev/null").c_str());
        system(("cp /etc/xdg/feednix/config.json " + HOME_PATH + "/.config/feednix").c_str());
        system(("chmod 600 " + HOME_PATH + "/.config/feednix/config.json").c_str());
    }

    char *sys_tmpdir = getenv("TMPDIR");
    if(!sys_tmpdir)
        sys_tmpdir = (char *)"/tmp";

    char * pathTemp = (char *)malloc(sizeof(char) * (strlen(sys_tmpdir) + 16));
    strcpy(pathTemp, sys_tmpdir);
    strcat(pathTemp, "/feednix.XXXXXX");

    TMPDIR = std::string(mkdtemp(pathTemp));
    free(pathTemp);

    if(argc >= 2){
        for(int i = 1; i < argc; ++i){
            if(argv[i][0] == '-' && argv[i][1] == 'h' && strlen(argv[1]) <= 2){
                printUsage();
                return 0;
            }
            else{
                if(argv[i][0] == '-' && argv[i][1] == 'v' && strlen(argv[1]) <= 2)
                    verboseEnabled = true;
                else if(argv[i][0] == '-' && argv[i][1] == 'c' && strlen(argv[1]) <= 2)
                    changeTokens = true;
                else if(argv[i][0] != '-' || ((argv[i][0] == '-' && ((argv[i][1] != 'v' && argv[i][1] != 'c' && argv[i][1] != 'h') || strlen(argv[1]) >= 2)))){
                    std::cerr << "ERROR: Invalid option " << "\'" << argv[i] << "\'\n" << std::endl;
                    printUsage();
                    return 0;
                }
            }
        }
        curses = new CursesProvider(verboseEnabled, changeTokens);
    }
    else{
        curses = new CursesProvider(false, false);
    }


    curses->init();
    curses->eventHandler();

    delete curses;
    return 0;
}

void printUsage(){
    std::cout << "Usage: feednix [OPTIONS]" << std::endl;
    std::cout << "  An ncurses-based console client for Feedly written in C++" << std::endl;
    std::cout << "\n Options:\n  -h        Display this help and exit\n  -v        Set curl to output in verbose mode during login\n  -c        Change tokens with prompt" << std::endl;
    std::cout << "\n Config:\n   Feednix uses a config file to set colors and\n   and the amount of posts to be retrived per\n   request." << std::endl;
    std::cout << "\n   This file can be found and must be placed in:\n     $HOME/.config/feednix\n   A log file can also be found here.\n   A sample config can be found in /etc/feednix" << std::endl;
    std::cout << "\n Author:\n   Copyright Jorge Martinez Hernandez <jorgemartinezhernandez@gmail.com>\n   Licensing information can be found in the source code" << std::endl;
    std::cout << "\n Bugs:\n   Please report any bugs on Github <https://github.com/Jarkore/Feednix>" << std::endl;
}
