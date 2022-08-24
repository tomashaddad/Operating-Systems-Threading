#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>

class Profiler {
public:
    void start();
    void stop();
    int getDuration();

private:
    std::chrono::steady_clock::time_point begin;
    signed long m_duration;
};

#endif  // Profiler.h