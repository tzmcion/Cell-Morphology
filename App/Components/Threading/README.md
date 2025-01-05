This folder is used for:
    -   Handling threading of operations: It provides a Queue structure, to which the operations can be added, and it will run it whenever a thread is free
        Thanks to this, it can run 4 operations in the same time, depending on CPU load
    -   Providing a funtion which stops the program and wait for file to change: used to communicate with subprocess run in python