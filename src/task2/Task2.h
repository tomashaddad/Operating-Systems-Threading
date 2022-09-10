#ifndef TASK2_H
#define TASK2_H
#include <map>
#include <string>
#include <vector>

class Task2 {
   public:
    Task2(const std::string &wordlist);
    void map2();
    void reduce2();

   private:
    std::map<int, std::vector<std::string>> m_lists;
    std::string m_wordlist;
};

extern std::vector<pid_t> children;

#endif  // Task2.h