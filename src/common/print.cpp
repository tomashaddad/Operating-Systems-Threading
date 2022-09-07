#include "print.h"

#include <iostream>

#include "time.h"

void utility::print_arguments(int argc, char *argv[]) {
    std::cout << utility::timestamp() << "Arguments: [ ";

    for (auto i = 1; i < argc; ++i) {
        std::cout << argv[i] << (i == argc - 1 ? "" : ", ");
    }

    std::cout << " ]" << std::endl;
}