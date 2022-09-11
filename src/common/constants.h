#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
int constexpr MIN_LENGTH = 3;
int constexpr MAX_LENGTH = 15;
int constexpr NUM_LENGTHS = MAX_LENGTH - MIN_LENGTH + 1;
int constexpr TIME_LIMIT = 20;
int constexpr BUFFER_SIZE = 20;  // Just has to be slightly larger than the longest word
}  // namespace constants

#endif  // CONSTANTS_H