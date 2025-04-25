/**
 * .cpp files for "Structures/Threading.h"
 * */

#include "./Threads.h"

Threading::Threading(size_t number_of_threads){
    for(size_t x = 0; x < number_of_threads; x++){
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                { // Lock scope
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this]() { return !tasks.empty() || stop; });
                    
                    if (stop && tasks.empty()) return; // Exit thread loop
                    
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task(); // Execute the task
            }
        });
    }
}

Threading::~Threading(){
    { // Lock scope
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }
    condition.notify_all(); // Wake all threads to exit
    for (std::thread &worker : workers) {
        worker.join(); // Wait for threads to finish
    }
    std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.txt");
    std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
}

void Threading::awaitJoin(){
    { // Lock scope
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }
    condition.notify_all(); // Wake all threads to exit
    for(std::thread &worker:workers){
        worker.join();
    }
    std::cout << "All workers Finished" << std::endl;
}

void Threading::enqueueTask(std::function<void()> task) {
    { // Lock scope
        std::unique_lock<std::mutex> lock(mutex);
        tasks.push(std::move(task));
    }
    condition.notify_one(); // Notify one thread that a task is available
}