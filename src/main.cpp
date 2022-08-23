#include "Profiler.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    Profiler profiler;
    profiler.start();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    profiler.stop();

    std::cout << profiler.getDuration() << std::endl;

    return EXIT_SUCCESS;
}