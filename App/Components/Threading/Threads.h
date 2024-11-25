#pragma once

#include "../Includes/includes.h"
#include "../Structures/Structures.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class Threading{
    public:
        Threading(size_t number_of_threads);
        ~Threading();

        void enqueueTask(std::function<void()> task);
    private:
        std::vector<std::thread> workers;              // Worker threads
        std::queue<std::function<void()>> tasks;       // Task queue
        std::mutex mutex;                              // Mutex to protect shared data
        std::condition_variable condition;            // Condition variable for task notification
        bool stop = false;                             // Stop flag
};