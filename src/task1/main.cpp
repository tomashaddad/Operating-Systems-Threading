#include <filesystem>
#include <iostream>

#include "../common/Profiler.h"
#include "../common/print.h"
#include "../common/time.h"
#include "Task1.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << utility::timestamp() << "Two arguments are required, but " << argc - 1
                  << " were provided." << std::endl;
        utility::print_arguments(argc, argv);
        return EXIT_FAILURE;
    }

    std::filesystem::path cwd = std::filesystem::current_path();

    std::cout << utility::timestamp() << "The current working directory is: " << cwd << std::endl;

    utility::print_arguments(argc, argv);

    Profiler profiler;
    profiler.start();

    Task1::TaskFilter(argv[1], argv[2]);

    profiler.stop();

    std::cout << profiler.getDuration() << std::endl;

    return EXIT_SUCCESS;
}