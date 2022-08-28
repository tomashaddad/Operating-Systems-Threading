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
#include "../task1/Task1.h"

#define MIN_LENGTH 3
#define MAX_LENGTH 15

std::vector<std::string> globalWords;

struct ThreadData {
    int length;
    std::vector<int>& indices;
};

void* sort(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    std::cout << "I am joining word length " << data->length << std::endl;

    auto start = std::begin(data->indices);
    auto end = std::end(data->indices);

    // Indices in the ThreadData struct is a reference, so this is done in-place
    std::sort(start, end, [](const int& a, const int& b) {
        return globalWords[a].substr(2) < globalWords[b].substr(2);
    });

    std::string fifo = "src/task3/sorted/sorted_" + std::to_string(data->length);
    const char* fifo_cstr = fifo.c_str();

    mkfifo(fifo_cstr, 0666);
    auto descriptor = open(fifo_cstr, O_WRONLY);

    for (auto i : data->indices) {
        std::cout << "Writing " << globalWords[i].c_str() << " to " << fifo << std::endl;
        write(descriptor, globalWords[i].c_str(), globalWords[i].size());
    }

    close(descriptor);

    delete data;
    data = nullptr;
    return 0;
}

void* map3(void* arg) {
    // A mapping of word lengths to a vector of indices in the globalWords vector
    std::map<int, std::vector<int>> indices;
    pthread_t threads[MAX_LENGTH - MIN_LENGTH + 1];

    for (std::size_t i = 0; i < globalWords.size(); ++i) {
        indices[globalWords[i].length()].push_back(i);
    }

    for (int length = MIN_LENGTH; length <= MAX_LENGTH; ++length) {
        ThreadData* data = new ThreadData{length, indices[length]};
        pthread_create(&threads[length - MIN_LENGTH], NULL, &sort, data);
    }

    for (int length = MIN_LENGTH; length <= MAX_LENGTH; ++length) {
        pthread_join(threads[length - MIN_LENGTH], NULL);
    }

    return 0;
}

void* reduce3(void* arg) {
    std::cout << "I am in the reduce function!" << std::endl;

    auto descriptor = open("src/task3/sorted/sorted_3", O_RDONLY);
    char word[3];

    for (int i = 0; i < 10; ++i) {
        read(descriptor, &word, sizeof(char) * 3);
        std::cout << "Read word " << word << std::endl;
    }

    close(descriptor);

    return 0;
}

void populateValidWords(std::string wordlist, std::vector<std::string>& validWords) {
    std::ifstream in(wordlist);

    if (!in) {
        std::cerr << "Failed to open input stream " << wordlist << std::endl;
        throw std::runtime_error("Instream failed to open");
    }

    std::string word;
    while (std::getline(in, word)) {
        validWords.push_back(word);
    }
}

void print_arguments(int argc, char* argv[]) {
    std::cout << "Arguments: [ ";

    for (auto i = 1; i < argc; ++i) {
        std::cout << argv[i] << (i == argc - 1 ? "" : ", ");
    }

    std::cout << " ]" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Two arguments are required, but " << argc - 1 << " were provided."
                  << std::endl;
        print_arguments(argc, argv);
        return EXIT_FAILURE;
    }

    std::filesystem::path cwd = std::filesystem::current_path();

    std::cout << "The current working directory is: " << cwd << std::endl;
    std::cout << "Running code with arguments: ";
    print_arguments(argc, argv);

    Profiler profiler;
    profiler.start();

    std::string inputDir(argv[1]);
    std::string outputDir(argv[2]);

    Task1::TaskFilter(inputDir, outputDir);

    populateValidWords(outputDir, globalWords);

    pthread_t map, reduce;

    if (pthread_create(&map, NULL, &map3, NULL)) {
        std::cout << "Error creating map3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Map thread created" << std::endl;

    if (pthread_create(&reduce, NULL, &reduce3, NULL)) {
        std::cout << "Error creating reduce3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Reduce thread created" << std::endl;

    if (pthread_join(map, NULL)) {
        std::cout << "Error joining map3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Map thread joined" << std::endl;

    if (pthread_join(reduce, NULL)) {
        std::cout << "Error joining reduce3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Reduce thread joined" << std::endl;

    profiler.stop();

    std::cout << "Program executed in " << profiler.getDuration() << " ms." << std::endl;

    return EXIT_SUCCESS;
}
