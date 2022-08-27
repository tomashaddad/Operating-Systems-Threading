#ifndef TASK2_H
#define TASK2_H
#include <string>
#include <vector>
#include <map>

class Task2
{
public:
    Task2(const std::string &wordlist);
    void map2();
    void reduce2();

private:
    std::vector<std::string> sortFrom(int charPos);
    void writeToFile(int charPos, const std::vector<std::string> &list);

    std::map<int, std::vector<std::string>> m_lists;
    std::string m_wordlist;
};

#endif // Task2.h