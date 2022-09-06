#include "sort.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "constants.h"

void utility::mergeAndOutput(std::vector<std::vector<std::string>>& sortedLists,
                             const std::string& output) {
    int sum = 0;

    for (auto& list : sortedLists) {
        sum += list.size();
    }

    std::cout << "Total number of words to sort: " << sum << std::endl << std::endl;

    std::cout << "Sorting ..." << std::endl;

    std::ofstream out(output);

    if (!out) {
        std::cerr << "Failed to open output stream." << std::endl;
        throw std::runtime_error("Outstream failed to open");
    }

    std::vector<size_t> indices(sortedLists.size(), 0);
    std::vector<std::string> currentWords;

    // Repeatedly build a vector containing all words to compare. A word is
    // selected based on lexicographical ordering of the 3rd letter and beyond,
    // and the length of the chosen word is used to increment the corresponding
    // indices entry.

    int counter = 0;
    while (counter < sum) {
        for (size_t i = 0; i < sortedLists.size(); ++i) {
            if (indices[i] < sortedLists[i].size()) {
                currentWords.push_back(sortedLists[i][indices[i]]);
            }
        }

        if (currentWords.empty()) {
            break;
        }

        auto min = std::min_element(
            std::begin(currentWords), std::end(currentWords),
            [](const std::string& a, const std::string& b) { return a.substr(2) < b.substr(2); });
        out << *min << std::endl;

        int length = min->length() - constants::MIN_LENGTH;

        indices[length]++;

        // Note that vector.clear() does not free the memory, so this is still
        // memory efficient
        currentWords.clear();
        counter++;
    }

    if (counter > sum) {
        std::cerr << "ERROR: Program has tried to sort more words than there "
                     "are in the wordlist. "
                  << "Abording Task2::reduce2() ..." << std::endl;
    } else if (counter == sum) {
        std::cout << "Program has successfully sorted all words." << std::endl;
    } else {
        std::cerr << "ERROR: Program has tried to sort fewer words than there "
                     "are in the wordlist."
                  << std::endl;
    }
}

void utility::sortFrom(std::vector<std::string>& list, int charPos) {
    auto start = std::begin(list);
    auto end = std::end(list);
    std::sort(start, end, [&charPos](const std::string& a, const std::string& b) {
        return a.substr(charPos) < b.substr(charPos);
    });
}