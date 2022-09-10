#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../common/Profiler.h"
#include "../common/constants.h"
#include "../common/print.h"
#include "../common/readwrite.h"
#include "../common/sort.h"
#include "../common/time.h"
#include "../task1/Task1.h"

std::vector<std::string> globalWords;

pthread_mutex_t g_mutexDescriptors;
pthread_cond_t g_condFifosReady;
int g_counter = 0;

struct ThreadData {
    int length;
    std::vector<int>& indices;
};

struct ReduceData {
    int length;
    std::vector<std::string>& words;
};

void* sort(void* arg) {
    Profiler profiler;
    profiler.start();

    ThreadData* data = (ThreadData*)arg;

    auto start = std::begin(data->indices);
    auto end = std::end(data->indices);

    // Indices in the ThreadData struct is a reference, so this is done in-place
    std::sort(start, end, [](const int& a, const int& b) {
        return globalWords[a].substr(2) < globalWords[b].substr(2);
    });

    std::string fifo = "src/task3/sorted/sorted_" + std::to_string(data->length);
    const char* fifo_cstr = fifo.c_str();

    mkfifo(fifo_cstr, 0666);
    pthread_mutex_lock(&g_mutexDescriptors);
    ++g_counter;
    pthread_cond_signal(&g_condFifosReady);
    pthread_mutex_unlock(&g_mutexDescriptors);

    auto descriptor = open(fifo_cstr, O_WRONLY);
    int counter = 0;
    for (auto i : data->indices) {
        write(descriptor, globalWords[i].c_str(), globalWords[i].size());
        counter++;
    }

    close(descriptor);

    profiler.stop();

    std::cout << utility::timestamp() << "Map3 sort for words of length " << data->length
              << " took " << profiler.getDuration() << " ms" << std::endl;

    return data;
}

void* map3(void* arg) {
    std::cout << utility::timestamp() << "map3 thread started!" << std::endl;
    // A mapping of word lengths to a vector of indices in the globalWords vector
    std::map<int, std::vector<int>> indices;
    pthread_t threads[constants::NUM_LENGTHS];

    for (std::size_t i = 0; i < globalWords.size(); ++i) {
        indices[globalWords[i].length()].push_back(i);
    }

    for (int length = constants::MIN_LENGTH; length <= constants::MAX_LENGTH; ++length) {
        ThreadData* data = new ThreadData{length, indices[length]};
        pthread_create(&threads[length - constants::MIN_LENGTH], NULL, &sort, data);
    }

    ThreadData* mapData;

    for (int length = constants::MIN_LENGTH; length <= constants::MAX_LENGTH; ++length) {
        pthread_join(threads[length - constants::MIN_LENGTH], (void**)&mapData);
        delete mapData;
    }

    std::cout << utility::timestamp() << "Successfully joined all sort threads!" << std::endl;

    return 0;
}

void* readFifo(void* arg) {
    ReduceData* data = (ReduceData*)arg;

    std::string fifo = "src/task3/sorted/sorted_" + std::to_string(data->length);
    std::cout << utility::timestamp() << "Reading from " << fifo << std::endl;

    auto descriptor = open(fifo.c_str(), O_RDONLY);

    if (descriptor == -1) {
        std::cout << utility::timestamp() << "Failed to create read only descriptor for FIFO "
                  << fifo << std::endl;
        return data;
    }

    char* word = new char[data->length];

    while (true) {
        if (read(descriptor, word, sizeof(char) * data->length)) {
            data->words.push_back(std::string(word));
        } else {
            // Finished reading from FIFO
            break;
        }
    }

    close(descriptor);
    delete word;

    std::cout << utility::timestamp() << "Successfully read " << data->words.size()
              << " words from " << fifo << std::endl;

    return data;
}

void* reduce3(void* arg) {
    std::cout << utility::timestamp() << "reduce3 thread started!" << std::endl;

    pthread_mutex_lock(&g_mutexDescriptors);
    while (g_counter < constants::NUM_LENGTHS) {
        pthread_cond_wait(&g_condFifosReady, &g_mutexDescriptors);
        std::cout << utility::timestamp() << "Total FIFOs created: " << g_counter << std::endl;
    }
    pthread_mutex_unlock(&g_mutexDescriptors);

    // All FIFOS are ready, so we can start reading them

    pthread_t threads[constants::NUM_LENGTHS];
    std::vector<std::vector<std::string>> sortedLists(constants::NUM_LENGTHS);

    for (int length = 3; length <= constants::MAX_LENGTH; ++length) {
        ReduceData* data = new ReduceData{length, sortedLists[length - constants::MIN_LENGTH]};
        pthread_create(&threads[length - constants::MIN_LENGTH], NULL, &readFifo, data);
    }

    ReduceData* reduceData;

    for (int length = constants::MIN_LENGTH; length <= constants::MAX_LENGTH; ++length) {
        pthread_join(threads[length - constants::MIN_LENGTH], (void**)&reduceData);
        delete reduceData;
    }

    utility::mergeAndOutput(sortedLists, "src/task3/sorted/sorted.txt");

    return 0;
}

int main(int argc, char** argv) {
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

    std::string inputDir(argv[1]);
    std::string outputDir(argv[2]);

    Task1::TaskFilter(inputDir, outputDir);

    globalWords = utility::readFileIntoVector(outputDir);

    pthread_t map, reduce;

    if (pthread_create(&map, NULL, &map3, NULL)) {
        std::cout << utility::timestamp() << "Error creating map3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&reduce, NULL, &reduce3, NULL)) {
        std::cout << utility::timestamp() << "Error creating reduce3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_join(map, NULL)) {
        std::cout << utility::timestamp() << "Error joining map3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_join(reduce, NULL)) {
        std::cout << utility::timestamp() << "Error joining reduce3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    profiler.stop();

    std::cout << utility::timestamp() << "Program executed in " << profiler.getDuration() << " ms."
              << std::endl;

    return EXIT_SUCCESS;
}
