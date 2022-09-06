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

    return data;
}

void* map3(void* arg) {
    std::cout << "map3 thread started!" << std::endl;
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

    std::cout << "Successfully joined all sort threads!" << std::endl;

    return 0;
}

void* readFifo(void* arg) {
    ReduceData* data = (ReduceData*)arg;

    std::string fifo = "src/task3/sorted/sorted_" + std::to_string(data->length);
    std::cout << "Reading from " << fifo << std::endl;

    auto descriptor = open(fifo.c_str(), O_RDONLY);

    if (descriptor == -1) {
        std::cout << "Failed to create read only descriptor for FIFO " << fifo << std::endl;
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

    std::cout << "Successfully read " << data->words.size() << " words from " << fifo << std::endl;

    return data;
}

void* reduce3(void* arg) {
    std::cout << "reduce3 thread started!" << std::endl;

    pthread_mutex_lock(&g_mutexDescriptors);
    while (g_counter < constants::NUM_LENGTHS) {
        pthread_cond_wait(&g_condFifosReady, &g_mutexDescriptors);
        std::cout << "Total FIFOs created: " << g_counter << std::endl;
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

    int sum = 0;
    for (auto& list : sortedLists) {
        sum += list.size();
    }

    std::cout << "Successfully joined all read threads! Total words: " << sum << std::endl;
    std::cout << "Merging to output file..." << std::endl;

    std::ofstream out("src/task3/sorted/sorted.txt");

    if (!out) {
        std::cerr << "Failed to open output stream." << std::endl;
        throw std::runtime_error("Outstream failed to open");
    }

    std::vector<size_t> indices(sortedLists.size(), 0);
    std::vector<std::string> currentWords;

    // Repeatedly build a vector containing all words to compare. A word is
    // selected based on lexicographical ordering of the 3rd letter and beyond,
    // and the length of the chosen word is used to increment the corresponding
    // indices entry.

    int counter = 0;
    while (counter < sum) {
        for (size_t i = 0; i < sortedLists.size(); ++i) {
            if (indices[i] < sortedLists[i].size()) {
                currentWords.push_back(sortedLists[i][indices[i]]);
            }
        }

        if (currentWords.empty()) {
            break;
        }

        auto min = std::min_element(
            std::begin(currentWords), std::end(currentWords),
            [](const std::string& a, const std::string& b) { return a.substr(2) < b.substr(2); });
        out << *min << std::endl;

        int length = min->length() - constants::MIN_LENGTH;

        indices[length]++;

        // Note that vector.clear() does not free the memory, so this is still
        // memory efficient
        currentWords.clear();
        counter++;
    }

    if (counter > sum) {
        std::cerr << "ERROR: Program has tried to sort more words than there "
                     "are in the wordlist. "
                  << "Abording Task2::reduce2() ..." << std::endl;
    } else if (counter == sum) {
        std::cout << "Program has successfully sorted all words." << std::endl;
    } else {
        std::cerr << "ERROR: Program has tried to sort fewer words than there "
                     "are in the wordlist."
                  << std::endl;
    }

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

    if (pthread_create(&reduce, NULL, &reduce3, NULL)) {
        std::cout << "Error creating reduce3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_join(map, NULL)) {
        std::cout << "Error joining map3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_join(reduce, NULL)) {
        std::cout << "Error joining reduce3 thread" << std::endl;
        return EXIT_FAILURE;
    }

    profiler.stop();

    std::cout << "Program executed in " << profiler.getDuration() << " ms." << std::endl;

    return EXIT_SUCCESS;
}
