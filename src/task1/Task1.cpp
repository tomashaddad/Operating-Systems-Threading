#include "Task1.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <set>

void Task1::TaskFilter(const std::string& input, const std::string& output) {
    std::ifstream in(input);

    if (!in) {
        std::cerr << "Input file not found" << std::endl;
        throw std::runtime_error("File not found");
    }

    std::ofstream out(output);

    if (!out) {
        std::cerr << "Failed to open output stream." << std::endl;
        throw std::runtime_error("Outstream failed to open");
    }

    // Adding words to a set means that filter duplicate words
    std::set<std::string> unique_words;

    std::string line;
    while (std::getline(in, line)) {
        if (line.length() >= 3 && line.length() <= 15) {
            bool is_valid = true;
            int counter = 0;
            char reference = line[0];

            for (auto& character : line) {
                if (std::isalpha(character)) {
                    // As long as we're still looking at the same character, if we read it three
                    // times in a row, we mark it as invalid, and stop checking the rest of the word
                    // Otherwise, we reset the counter and the reference character
                    if (character == reference) {
                        counter++;
                        if (counter == 3) {
                            is_valid = false;
                            break;
                        }
                    } else {
                        reference = character;
                        counter = 1;
                    }
                } else {
                    is_valid = false;
                    break;
                }
            }
            if (is_valid) {
                unique_words.insert(line);
            }
        }
    }

    for (auto& word : unique_words) {
        out << word << std::endl;
    }
}