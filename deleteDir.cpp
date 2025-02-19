#include <stdio.h>
#include <iostream>
#include <unistd.h>     // для getcwd
#include <fcntl.h>      // для open
#include <dirent.h>      // для dirent
#include <string.h>

#define STR_LEN 100
char nowDir_char[STR_LEN];
char mainDir[STR_LEN];


void deleteDirRecurs() {
    static int countRecurs = 0;
    int thisRecurs = countRecurs;
    // при первой итерации запоминаем папку, в которой мы были
    if (!countRecurs)
    {
        getcwd(mainDir, STR_LEN);
    }
    
    std::cout << "countRecurs: " << ++countRecurs << "\n";
    dirent** nameList;
    int countElems = scandir(".", &nameList, NULL, alphasort);
    std::cout << "countElems: " << countElems << "\n";
    if (countElems == -1){
        std::cout << "I'm in the if. I shouldn't be here." << "\n";
        exit(-1);
    }
    std::cout << "after if" << "\n";
    for (size_t i = 1; i <= countElems; i++)
    {
        std::cout << "nameList[countElems - "<< i << "]->d_name: " << nameList[countElems - i]->d_name << "\n";
        int fd = open(nameList[countElems - i]->d_name, O_DIRECTORY);
        if (-1 == fd 
            || nameList[countElems - i]->d_name[0] == '.')
            continue;
        else{
            std::cout << nameList[countElems - i]->d_name << "\n";
            close(fd);
            chdir(nameList[countElems - i]->d_name);
            deleteDirRecurs();

            std::cout << "rmdir(nameList[countElems - " << i << "]->d_name)" << "\n";
            std::cout << "nameList[countElems - i]->d_name = " << nameList[countElems - i]->d_name << "\n";
            if (-1 == rmdir(nameList[countElems - i]->d_name)) {
                throw std::runtime_error("cant't remove dir");
            }
            --i;
        }
    }
    
    for (size_t i = 1; i <= countElems; i++)
    {
        if (!thisRecurs || !strcmp("..", nameList[countElems - i]->d_name)
            || !strcmp(".", nameList[countElems - i]->d_name))
            continue;
        std::cout << "unlink(nameList[countElems - "<< i <<"]->d_name);" << "\n";
        std::cout << "nameList[countElems - "<< i <<"]->d_name = " << nameList[countElems - i]->d_name << "\n";
        unlink(nameList[countElems - i]->d_name);
    }

    getcwd(nowDir_char, STR_LEN);
    std::cout << "mainDir = " << mainDir << "\n";
    std::cout << "nowDir_char = " << nowDir_char << "\n";
    if (strcmp(mainDir, nowDir_char))
    {
        std::cout << "chdir(\"..\");" << "\n";
        chdir("..");
    }
    else
        return; 
}

int main() {
    std::cout << "Start delete" << "\n";
    deleteDirRecurs();
    return 0;
}