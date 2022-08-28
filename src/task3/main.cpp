#include <pthread.h>

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

void* sort(void* arg) {
    int* wordLength = (int*)arg;
    std::cout << "I am joining word length " << *wordLength << std::endl;
    delete wordLength;
    wordLength = nullptr;
    return 0;
}

void* map3(void* arg) {
    std::map<int, std::vector<std::string>> lists;
    pthread_t threads[MAX_LENGTH - MIN_LENGTH + 1];

    for (const auto& word : globalWords) {
        lists[word.length()].push_back(word);
    }

    for (int length = MIN_LENGTH; length <= MAX_LENGTH; ++length) {
        int* lengthPtr = new int(length);
        pthread_create(&threads[length - MIN_LENGTH], NULL, &sort, lengthPtr);
    }

    for (int length = MIN_LENGTH; length <= MAX_LENGTH; ++length) {
        pthread_join(threads[length - MIN_LENGTH], NULL);
    }

    return 0;
}

void* reduce3(void* arg) { return 0; }

void populateValidWords(std::string wordlist,
                        std::vector<std::string>& validWords) {
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
        std::cout << "Two arguments are required, but " << argc - 1
                  << " were provided." << std::endl;
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

    std::cout << "Program executed in " << profiler.getDuration() << " ms."
              << std::endl;

    return EXIT_SUCCESS;
}
