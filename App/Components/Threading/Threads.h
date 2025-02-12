/**
 * File for handling multi threading in application, used in Count_Cells and Preprocessing (Norm_*)
 * Define here is also a method "await file change" which is used to communicate with model which runs in python
 * */

#pragma once

#include "../Includes/includes.h"
#include "../Structures/Structures.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class Threading{
    public:
        //Try to start threading with set number of threads, 
        //Need to check it before with hardware_concurrency(), and provide it as "number_of_threads"
        Threading(size_t number_of_threads);
        ~Threading();

        //Add a task to the queue. When a thread is free, it will automatically start doing this task
        void enqueueTask(std::function<void()> task);

        /**
        * Function waits until the file changes. If it iterates for more than 5000 times (10 sec), the wait is abandoned
        * @param path_to_monitor a path to the file, which changes are monitored 
        * **/
        static void await_file_change(std::string path_to_monitor, int limit=5000, int span=2){
            std::chrono::milliseconds timespan(span);
            //Read content of the file
            std::string content = Entites::Convert::text_file_to_string(path_to_monitor.c_str());
            int counter = 0;
            while (counter < limit)
            {
                std::this_thread::sleep_for(timespan);
                counter++;
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