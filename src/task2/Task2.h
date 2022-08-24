#ifndef TASK2_H
#define TASK2_H
#include <string>
#include <vector>

class Task2
{
public:
    static void map2(const std::string &wordlist);
    static void reduce2(const std::vector<std::string> &wordlists);
};

#endif // Task2.h