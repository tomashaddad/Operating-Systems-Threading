#include "Profiler.h"
#include <iostream>

void Profiler::start() {
    begin = std::chrono::steady_clock::now();
}

void Profiler::stop() {
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    m_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

int Profiler::getDuration() {
    return m_duration;
}
