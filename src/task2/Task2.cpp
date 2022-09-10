#include "Task2.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Forking
#include <pthread.h>
#include <unistd.h>
#include <wait.h>

#include <algorithm>
#include <cstring>
#include <iterator>

#include "../common/constants.h"
#include "../common/readwrite.h"
#include "../common/sort.h"
#include "../common/time.h"

std::vector<pid_t> children;

void childHandler(int signum) {
    std::cout << utility::timestamp() << "Child with pid " << getpid() << " caught signal "
              << strsignal(signum) << ". Aborting ..." << std::endl;
    exit(signum);
}

Task2::Task2(const std::string &wordlist) : m_wordlist(wordlist) {
    std::ifstream in(wordlist);
    std::string line;
    while (std::getline(in, line)) {
        m_lists[line.length()].push_back(line);
    }
}

void Task2::map2() {
    pid_t pid;
    pid_t waitPid;

    for (auto i = constants::MIN_LENGTH; i <= constants::MAX_LENGTH; ++i) {
        pid = fork();
        if (pid > 0)  // parent
        {
            children.push_back(pid);
            continue;
        } else if (pid == 0)  // child
        {
            alarm(0);                       // disable alarm inherited from parent
            signal(SIGALRM, childHandler);  // override parent's signal handler

            std::cout << utility::timestamp() << "Child (PID " << getpid()
                      << ") created and delegated to " << i << " character strings. Creating ..."
                      << std::endl;

            // in-place sort from second character onwards
            utility::sortFrom(m_lists[i], 2);

            std::string dest = "src/task2/sorted/sorted_" + std::to_string(i) + ".txt";
            utility::writeListToFile(m_lists[i], dest);

            exit(1);
        }
    }

    int status = 0;
    // wait will return > 0 so long as there is at least one child process
    while ((waitPid = wait(&status)) > 0) {
        std::cout << utility::timestamp() << "Child process " << waitPid << " exited with status "
                  << WEXITSTATUS(status) << std::endl;
    }

    std::cout << utility::timestamp() << "All child processes have exited. Parent (PID " << getpid()
              << ") will now begin sorting." << std::endl;
}

void Task2::reduce2() {
    std::vector<std::vector<std::string>> sortedLists;

    // Read files into vectors
    // Index 0 is words of length 3, index 1 is words of length 4, etc.
    for (auto i = constants::MIN_LENGTH; i <= constants::MAX_LENGTH; ++i) {
        std::string source = "src/task2/sorted/sorted_" + std::to_string(i) + ".txt";

        std::vector<std::string> list = utility::readFileIntoVector(source);

        sortedLists.push_back(list);
        std::cout << utility::timestamp() << "Read " << list.size() << " words from " << source
                  << std::endl;
    }

    utility::mergeAndOutput(sortedLists, "src/task2/sorted/sorted.txt");
}