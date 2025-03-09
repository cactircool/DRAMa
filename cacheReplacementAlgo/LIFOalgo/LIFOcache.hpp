#ifndef LIFOcache_hpp
#define LIFOcache_hpp

#include <iostream>
#include <stack>
#include <unordered_map>
using namespace std;

class LIFOcache {
    private: 
        size_t capacity;
        stack<int> cacheStack;
        unordered_map<int,int> CacheMap;
    public:
        LIFOcache(size_t cap){
            capacity = cap;
        }

        int get(int key) {
            if(CacheMap.find(key) != CacheMap.end()) {
                return CacheMap[key];
            }
            return -1;
        }
        void put(int key, int value) {
            if(CacheMap.find(key) == CacheMap.end()) {
                if(CacheMap.size() == capacity) {
                    int removingKey = cacheStack.top();
                    cacheStack.pop();
                    CacheMap.erase(removingKey);
                }
            }
            CacheMap[key] = value;
            cacheStack.push(key);
        }

        void printCache() {
            for (auto cacheline : CacheMap) {
                cout << "Key: " << cacheline.first << " Value: " << cacheline.second << endl;
            }
            cout << endl;
        }
};





#endif