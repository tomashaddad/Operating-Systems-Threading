#ifndef SORT_H
#define SORT_H

#include <memory>
#include <string>
#include <vector>

namespace utility {
void mergeAndOutput(std::vector<std::vector<std::string>>& sortedLists, const std::string& output);
void sortFrom(std::vector<std::string>& list, int charPos);
}  // namespace utility

#endif  // SORT_H