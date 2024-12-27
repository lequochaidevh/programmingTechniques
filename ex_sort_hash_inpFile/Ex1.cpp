#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Entry {
    std::string location;
    std::string time;
};

// Function to print the entries
void printEntries(const std::vector<Entry>& entries) {
    for (const auto& entry : entries) {
        std::cout << entry.location << " " << entry.time << "\n";
    }
    std::cout << "------------------------\n";
}

int main() {
    // Sample data
    std::vector<Entry> entries = {
        {"Chicago", "09:00:00"}, {"Phoenix", "09:00:59"}, {"Houston", "09:01:10"},
        {"Chicago", "09:01:10"}, {"Chicago", "09:03:13"}, {"Seattle", "09:10:25"},
        {"Seattle", "09:22:54"}, {"Phoenix", "09:35:21"}, {"Seattle", "09:36:14"},
        {"Phoenix", "09:37:44"}
    };

    // Step 1: Sort by time (primary key)
    std::stable_sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        return a.time < b.time;
    });

    std::cout << "Sorted by Time:\n";
    printEntries(entries);

    // Step 2: Stable sort by location (secondary key)
    std::stable_sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        return a.location < b.location;
    });

    std::cout << "Sorted by Location (Stable):\n";
    printEntries(entries);

    return 0;
}
