#pragma once

#include "../Includes/includes.h"
#include "../Structures/Structures.h"
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class Threading{
    public:
        Threading(size_t number_of_threads);
        ~Threading();

        void enqueueTask(std::function<void()> task);

        static void await_file_change(std::string path_to_monitor){
            std::chrono::milliseconds timespan(5);
            //Read content of the file
            std::string content = Entites::Convert::text_file_to_string(path_to_monitor.c_str());
            while (1)
            {
                std::this_thread::sleep_for(timespan);
                if(Entites::Convert::text_file_to_string(path_to_monitor.c_str()) != content)
                    return;
            }
        }
    private:
        std::vector<std::thread> workers;              // Worker threads
        std::queue<std::function<void()>> tasks;       // Task queue
        std::mutex mutex;                              // Mutex to protect shared data
        std::condition_variable condition;            // Condition variable for task notification
        bool stop = false;                             // Stop flag
};