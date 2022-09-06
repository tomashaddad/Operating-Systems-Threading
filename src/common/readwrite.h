#ifndef READWRITE_H
#define READWRITE_H

#include <string>
#include <vector>

namespace utility {
void writeListToFile(const std::vector<std::string>& list, std::string& output);
std::vector<std::string> readFileIntoVector(const std::string& file);
}  // namespace utility

#endif  // READWRITE_H