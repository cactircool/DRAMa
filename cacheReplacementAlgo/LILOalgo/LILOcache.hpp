#ifndef LILOcache_hpp
#define LILOcache_hpp

#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

class LILO {
    private:
        size_t capacity;
        queue<int> cacheQueue;
        unordered_map<int, int> cacheMap;
    
    public:
        LILO (int max_size) {
            capacity = max_size;
        }


        int get(int key) {
            if(cacheMap.find(key) != cacheMap.end()) {
                return cacheMap[key];
            }
            return -1;
        }

        void put(int key, int value) {
            if(cacheMap.find(key) == cacheMap.end()) {
                if(cacheQueue.size() == capacity) {
                    int removingElement = cacheQueue.front();
                    cacheQueue.pop();
                    cacheMap.erase(removingElement);
                }
            }
            cacheMap[key] = value;
            cacheQueue.push(key);
        }

        void printCache() {
            for (auto cacheline : cacheMap) {
                cout << "Key: " << cacheline.first << " Value: " << cacheline.second << endl;
            }
            cout << endl;
        }
};





#endif