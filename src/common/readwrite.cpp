#include "readwrite.h"

#include <fstream>
#include <iostream>

void utility::writeListToFile(const std::vector<std::string>& list, std::string& output) {
    std::ofstream out(output);

    if (!out) {
        std::cerr << "Failed to open output stream." << std::endl;
        throw std::runtime_error("Outstream failed to open");
    }

    for (const auto& word : list) {
        out << word << std::endl;
    }
}

std::vector<std::string> utility::readFileIntoVector(const std::string& file) {
    std::ifstream in(file);

    if (!in) {
        std::cerr << "Failed to open input stream for file: " << file << ". Aborting ..."
                  << std::endl;
        throw std::runtime_error("Instream failed to open");
    }

    std::vector<std::string> list;
    std::string line;
    while (std::getline(in, line)) {
        list.push_back(line);
    }
    return list;
}
