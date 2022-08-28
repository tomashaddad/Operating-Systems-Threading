#include <filesystem>
#include <iostream>

#include "../common/Profiler.h"
#include "Task1.h"

void print_arguments(int argc, char *argv[]) {
    std::cout << "Arguments: [ ";

    for (auto i = 1; i < argc; ++i) {
        std::cout << argv[i] << (i == argc - 1 ? "" : ", ");
    }

    std::cout << " ]" << std::endl;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Two arguments are required, but " << argc - 1
                  << " were provided." << std::endl;
        print_arguments(argc, argv);
        return EXIT_FAILURE;
    }

    std::filesystem::path cwd = std::filesystem::current_path();

    std::cout << "The current working directory is: " << cwd << std::endl;
    std::cout << "Running code with arguments: ";
    print_arguments(argc, argv);

    Profiler profiler;
    profiler.start();

    Task1::TaskFilter(argv[1], argv[2]);

    profiler.stop();

    std::cout << profiler.getDuration() << std::endl;

    return EXIT_SUCCESS;
}