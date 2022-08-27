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

#include <algorithm>
#include <iterator>

#define MIN_LENGTH 3
#define MAX_LENGTH 15

Task2::Task2(const std::string &wordlist) : m_wordlist(wordlist)
{
    std::ifstream in(wordlist);
    std::string line;
    while (std::getline(in, line))
    {
        m_lists[line.length()].push_back(line);
    }
}

std::vector<std::string> Task2::sortFrom(int charPos)
{
    auto start = std::begin(m_lists[charPos]);
    auto end = std::end(m_lists[charPos]);
    std::sort(start, end,
              [](const std::string &a, const std::string &b)
              { return a.substr(2) < b.substr(2); });
    return m_lists[charPos];
}

void Task2::writeToFile(int charPos, const std::vector<std::string> &list)
{
    std::string dest = "src/task2/sorted/sorted_" + std::to_string(charPos) + ".txt";
    std::ofstream out(dest);

    if (!out)
    {
        std::cerr << "Failed to open output stream." << std::endl;
        throw std::runtime_error("Outstream failed to open");
    }

    for (const auto &word : list)
    {
        out << word << std::endl;
    }
}

void Task2::map2()
{
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

            std::cout << "Child (PID " << getpid() << ") created and delegated to "
                      << id << " character strings. Creating ..." << std::endl;

            auto sorted = sortFrom(i);
            writeToFile(i, sorted);

            exit(1);
        }
    }

    int status = 0;
    // wait will return > 0 so long as there is at least one child process
    while ((waitPid = wait(&status)) > 0)
    {
        std::cout << "Child process " << waitPid << " exited with status "
                  << WEXITSTATUS(status) << std::endl;
    }

    std::cout << "All child processes have exited. Parent (PID " << getpid()
              << ") will now begin sorting." << std::endl;
}

void Task2::reduce2()
{
}