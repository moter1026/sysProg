#include <stdio.h>
#include <iostream>
#include <unistd.h>     // для getcwd
#include <fcntl.h>      // для open
#include <sys/stat.h>   // для mkdir
#include <string>

#define STR_LEN 100
#define BUF_LEN 500
#define ONE "one.bin"
#define RAND "rand.bin"
#define ZERO "zero.bin"
#define DEV_RAND "/dev/urandom"
#define DEV_ZERO "/dev/zero"

char nowDir_char[STR_LEN];

void createNewDir(const char* name) {
    if (mkdir(name, 0755) == -1) {
        throw std::runtime_error("didn't create a directory.");
    }
    chdir(name);
    getcwd(nowDir_char, STR_LEN);
}

std::string getNextTxtFile() {
    static int numTxtFile = 0;
    std::string res(std::to_string(++numTxtFile)+".txt");
    return res;
}

int main() {
    getcwd(nowDir_char, STR_LEN);
    std::string nowDir(nowDir_char);

    char BUF[BUF_LEN];

    char Dir[] = "A";
    // создаю директорию А 
    createNewDir(Dir);
    
    std::string first = getNextTxtFile();
    int fileDescript = open(first.c_str() ,O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    link(first.c_str(), getNextTxtFile().c_str());
    close(fileDescript);

    // создаю директорию B 
    Dir[0] += 1;
    createNewDir(Dir);

    fileDescript = open(ONE ,O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    symlink(ONE, getNextTxtFile().c_str());

    for (size_t i = 0; i < BUF_LEN; i++)
    {
        BUF[i] = 0xff;
    }
    write(fileDescript, BUF, BUF_LEN);
    close(fileDescript);

    // создаю директорию C 
    Dir[0] += 1;
    createNewDir(Dir);

    // создаю директорию D 
    Dir[0] += 1;
    createNewDir(Dir);

    int rand = open(RAND ,O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    int urandom = open(DEV_RAND ,O_RDONLY, S_IRWXU);
    if(read(urandom, BUF, BUF_LEN) == -1) {
        throw std::runtime_error("Can't read");
    }
    write(rand, BUF, BUF_LEN);
    close(rand);
    close(urandom);

    chdir("../");
    getcwd(nowDir_char, STR_LEN);
    
    // создаю директорию E 
    Dir[0] += 1;
    createNewDir(Dir);
    
    for (size_t i = 0; i < 2; i++)
    {
        fileDescript = open(getNextTxtFile().c_str() ,O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
        close(fileDescript);
    }
    
    int zero = open(ZERO ,O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    int devZero = open(DEV_ZERO ,O_RDONLY, S_IRWXU);
    if(read(devZero, BUF, BUF_LEN) == -1) {
        throw std::runtime_error("Can't read");
    }
    write(zero, BUF, BUF_LEN);
    close(zero);
    close(devZero);
    return 0;
}