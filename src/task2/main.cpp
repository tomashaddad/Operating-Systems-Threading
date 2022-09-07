#include <filesystem>
#include <iostream>

#include "../common/Profiler.h"
#include "../common/print.h"
#include "../common/time.h"
#include "../task1/Task1.h"
#include "Task2.h"

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

    std::string output(argv[2]);

    Task1::TaskFilter(argv[1], argv[2]);

    Task2 task2(argv[2]);
    task2.map2();
    task2.reduce2();

    profiler.stop();

    std::cout << utility::timestamp() << "Program executed in " << profiler.getDuration() << " ms."
              << std::endl;

    return EXIT_SUCCESS;
}
