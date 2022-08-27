#include "../task1/Task1.h"
#include "../common/Profiler.h"
#include "Task2.h"

#include <filesystem>
#include <iostream>

void print_arguments(int argc, char *argv[])
{
    std::cout << "Arguments: [ ";

    for (auto i = 1; i < argc; ++i)
    {
        std::cout << argv[i] << (i == argc - 1 ? "" : ", ");
    }

    std::cout << " ]" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Two arguments are required, but " << argc - 1 << " were provided." << std::endl;
        print_arguments(argc, argv);
        return EXIT_FAILURE;
    }

    std::filesystem::path cwd = std::filesystem::current_path();

    std::cout << "The current working directory is: " << cwd << std::endl;
    std::cout << "Running code with arguments: ";
    print_arguments(argc, argv);

    Profiler profiler;
    profiler.start();

    std::string output(argv[2]);

    Task1::TaskFilter(argv[1], argv[2]);

    Task2 task2(argv[2]);
    task2.map2();
    // task2.reduce2();

    profiler.stop();

    std::cout << profiler.getDuration() << std::endl;

    return EXIT_SUCCESS;
}
