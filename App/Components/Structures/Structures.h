#pragma once

#include "../Includes/includes.h"

namespace Entites{
    template<typename T>
    class Queue{
        public:

        /**
        * add element to the end of queue
        * */
        void append(T var){
            data.push_back(var);
        }

        /**
         * Deletes and returns first element of queue
         * */
        T pop(){
            T d = this->begin();
            data.erase(data.begin());
            return d;
        }

        /**
         * Returns element on the begin of queue not deleting it
         * */
        T begin() const{
            return data.front();
        }

        bool empty() const{
            return data.size() == 0;
        }

        int size() const {return static_cast<int>(data.size());}

        private:
        std::vector<T> data;
    };
}