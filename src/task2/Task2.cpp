#include "Task2.h"
#include <map>
#include <fstream>
#include <vector>
#include <string>

#include <iostream>

// Forking
#include <pthread.h>
#include <unistd.h>
#include <wait.h>

#define MIN_LENGTH 3
#define MAX_LENGTH 15

void Task2::map2(const std::string &wordlist)
{
    // std::map<int, std::vector<std::string>> lists;
    // std::ifstream in(wordlist);

    // int i = 0;
    // std::string line;
    // while (std::getline(in, line))
    // {
    //     lists[line.length()].push_back(line);
    // }

    pid_t pid;
    pid_t waitPid;

    int id = 0;

    for (auto i = MIN_LENGTH; i <= MAX_LENGTH; ++i)
    {
        pid = fork();
        if (pid > 0) // parent
        {
            continue;
        }
        else if (pid == 0) // child
        {
            id = i;
            break;
        }
    }

    std::cout << "Child process " << getpid() << ": " << id << std::endl;

    int status = 0;
    while ((waitPid = wait(&status)) > 0)
    {
        std::cout << "Child process " << waitPid << " exited with status " << WEXITSTATUS(status) << std::endl;
    }

    if (id == 0)
    {
        std::cout << "I am the parent! " << getpid() << std::endl;
    }
    else
    {
        exit(1);
    }
}