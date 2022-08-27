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

#include <memory>

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
    std::vector<std::vector<std::string>> sortedLists;

    // Read files into vectors
    // Index 0 is words of length 3, index 1 is words of length 4, etc.
    for (auto i = MIN_LENGTH; i <= MAX_LENGTH; ++i)
    {
        std::string source = "src/task2/sorted/sorted_" + std::to_string(i) + ".txt";
        std::ifstream in(source);

        if (!in)
        {
            std::cerr
                << "Failed to open input stream for file: " << source << ". Aborting ..."
                << std::endl;
            throw std::runtime_error("Instream failed to open");
        }

        std::vector<std::string> list;
        std::string line;
        while (std::getline(in, line))
        {
            list.push_back(line);
        }

        sortedLists.push_back(list);
        std::cout << "Read " << list.size() << " words from " << source << std::endl;
    }

    int sum = 0;

    for (auto &list : sortedLists)
    {
        sum += list.size();
    }

    std::cout << "Total number of words to sort: " << sum << std::endl
              << std::endl;

    std::cout << "Sorting ..." << std::endl;

    std::ofstream out("src/task2/sorted/sorted.txt");

    if (!out)
    {
        std::cerr << "Failed to open output stream." << std::endl;
        throw std::runtime_error("Outstream failed to open");
    }

    std::vector<size_t> indices(sortedLists.size(), 0);
    std::vector<std::string> currentWords;

    // Repeatedly build a vector containing all words to compare. A word is selected based on
    // lexicographical ordering of the 3rd letter and beyond, and the length of the chosen word
    // is used to increment the corresponding indices entry.

    int counter = 0;
    while (counter < sum)
    {
        for (size_t i = 0; i < sortedLists.size(); ++i)
        {
            if (indices[i] < sortedLists[i].size())
            {
                currentWords.push_back(sortedLists[i][indices[i]]);
            }
        }

        if (currentWords.empty())
        {
            break;
        }

        auto min = std::min_element(std::begin(currentWords), std::end(currentWords),
                                    [](const std::string &a, const std::string &b)
                                    { return a.substr(2) < b.substr(2); });
        out << *min << std::endl;

        int length = min->length() - MIN_LENGTH;

        indices[length]++;

        // Note that vector.clear() does not free the memory, so this is still memory efficient
        currentWords.clear();
        counter++;
    }

    if (counter > sum)
    {
        std::cerr << "ERROR: Program has tried to sort more words than there are in the wordlist. "
                  << "Abording Task2::reduce2() ..." << std::endl;
    }
    else if (counter == sum)
    {
        std::cout << "Program has successfully sorted all words." << std::endl;
    }
    else
    {
        std::cerr << "ERROR: Program has tried to sort fewer words than there are in the wordlist."
                  << std::endl;
    }
}