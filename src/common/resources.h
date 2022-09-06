#ifndef RESOURCES_H
#define RESOURCES_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/times.h"

namespace utility {
double getProcessCPUPercentage();
double getProcessRAMPercentage();
}  // namespace utility

#endif  // RESOURCES_H