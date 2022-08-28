#include "Task1.h"

#include <cctype>
#include <fstream>
#include <iostream>

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

    std::string line;
    while (std::getline(in, line)) {
        if (line.length() >= 3 && line.length() <= 15) {
            bool is_valid = true;
            int counter = 0;
            char reference = line[0];

            for (auto& current : line) {
                if (std::isalpha(current)) {
                    if (current == reference) {
                        counter++;
                        if (counter == 3) {
                            is_valid = false;
                            break;
                        }
                    } else {
                        reference = current;
                        counter = 1;
                    }
                } else {
                    is_valid = false;
                    break;
                }
            }
            if (is_valid) {
                out << line << std::endl;
            }
        }
    }
}