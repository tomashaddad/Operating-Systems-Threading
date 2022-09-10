#include <unistd.h>

#include <csignal>
#include <filesystem>
#include <iostream>

#include "../common/Profiler.h"
#include "../common/constants.h"
#include "../common/print.h"
#include "../common/time.h"
#include "Task1.h"

void signalHandler(int signum) {
    std::cerr << utility::timestamp() << "The program has exceeded the maximum time limit of "
              << constants::TIME_LIMIT << " seconds." << std::endl;
    std::cerr << utility::timestamp() << "Terminating ..." << std::endl;
    exit(signum);
}

int main(int argc, char **argv) {
    std::cout << utility::timestamp() << "Task 1 has begun executing." << std::endl;

    alarm(constants::TIME_LIMIT);
    signal(SIGALRM, signalHandler);

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

    std::cout << utility::timestamp() << "Program executed in " << profiler.getDuration() << " ms."
              << std::endl;

    return EXIT_SUCCESS;
}