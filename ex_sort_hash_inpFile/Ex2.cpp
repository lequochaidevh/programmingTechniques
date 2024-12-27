#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <algorithm>
class HashTable {
private:
    static const int TABLE_SIZE = 5; // Example size of hash table
    std::vector<std::list<std::pair<std::string, int>>> table; // list like vector

    // Hash function to compute the index
    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char ch : key) {
            hash = (hash * 11 + ch) % TABLE_SIZE; // Simple hash function
        }
        std::cout << "\n hashing  return :" << hash << "\n";
        return hash;
    }

public:
    HashTable() : table(TABLE_SIZE) {}

    // Insert a key-value pair into the hash table
    void insert(const std::string& key, int value) {
        int index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second = value; // Update value if key exists
                return;
            }
        }
        table[index].insert(table[index].begin(), {key, value}); // push to front
        //table[index].emplace_back(key, value); // Add new key-value pair
    }

    // Search for a key in the hash table
    int search(const std::string& key) {
        int index = hashFunction(key);
        for (const auto& pair : table[index]) {
            if (pair.first == key) {
                return pair.second; // Key found
            }
        }
        return -1; // Key not found
    }

    // Display the hash table
    void display() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            std::cout << "Index " << i << ":";
            for (const auto& pair : table[i]) {
                std::cout << " -> (" << pair.first << ", " << pair.second << ")";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    HashTable ht;

    // Insert key-value pairs
    ht.insert("S", 0);
    ht.insert("E", 1);
    ht.insert("A", 2);
    ht.insert("R", 3);
    ht.insert("C", 4);
    ht.insert("H", 5);
    ht.insert("X", 7);
    ht.insert("L", 11);
    ht.insert("M", 9);
    ht.insert("P", 10);
    ht.insert("HELLO", 33);
    ht.insert("HOLLA", 33);
    // Display the hash table
    ht.display();

    // Search for keys
    std::cout << "Searching for key 'A': " << ht.search("A") << "\n";
    std::cout << "Searching for key 'M': " << ht.search("M") << "\n";
    std::cout << "Searching for key 'Z': " << ht.search("Z") << "\n"; // Key not present
    std::cout << "Searching for key 'HELLO': " << ht.search("HELLO") << "\n";
    std::cout << "Searching for key 'HLLOE': " << ht.search("HOLLA") << "\n";

    return 0;
}
