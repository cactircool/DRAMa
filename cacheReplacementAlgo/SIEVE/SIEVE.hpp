#ifndef SIEVE_hpp
#define SIEVE_hpp

#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;



class Sieve {
    private:
        size_t capacity;
        queue<int> cacheQueue;
        unordered_map <int, pair<int, bool>> cacheMap;

    public:
        Sieve (int max_size) {
            capacity = max_size;
        }


        int get(int key) {
            if (cacheMap.find(key) != cacheMap.end()) {
                queue<int> tempQueue;
                while (!cacheQueue.empty()) {
                    int front = cacheQueue.front();
                    cacheQueue.pop();
                    if (front != key) {
                        tempQueue.push(front); 
                    }
                }
                tempQueue.push(key);
                cacheQueue = tempQueue;
                return cacheMap[key].first;
            }
            return -1;
        }


        void put(int key, int value) {
            if (cacheMap.find(key) == cacheMap.end()) {
                if (cacheQueue.size() == capacity) {
                    int oldest = cacheQueue.front();
                    cacheQueue.pop();
                    cacheMap.erase(oldest);
                }
            }
            cacheQueue.push(key);
            cacheMap[key] = {value,true};
        }

        void printCache() {
            queue<int>tempQueue = cacheQueue;
            while (!tempQueue.empty()) {
                int key = tempQueue.front();
                tempQueue.pop();
                cout << "Key: " << key << " Value: "<< cacheMap[key].first << endl;
            }
            cout<< endl;
        }
        
        
};



#endif