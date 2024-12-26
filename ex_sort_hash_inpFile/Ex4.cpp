#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

using namespace std;

// Function to clean a word by removing punctuation and converting to lowercase
string cleanWord(const string& word) {
    string cleaned;
    for (char ch : word) {
        if (isalpha(ch) || isdigit(ch)) {
            cleaned += tolower(ch);
        }
    }
    return cleaned;
}

int main() {
    string filePath = "file.txt"; // Replace with the actual file path
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file!" << endl;
        return 1;
    }

    map<string, pair<int, set<int>>> wordData; // Word -> (Count, Set of Line Numbers)
    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        stringstream ss(line);
        string word;

        while (ss >> word) {
            string cleanedWord = cleanWord(word);

            if (!cleanedWord.empty()) {
                wordData[cleanedWord].first++;           // Increment count
                wordData[cleanedWord].second.insert(lineNumber); // Add line number
            }
        }
    }

    file.close();

    // Print results
    for (const auto& entry : wordData) {
        const string& word = entry.first;
        int count = entry.second.first;
        const set<int>& lines = entry.second.second;

        cout << word << " " ;//<< count << " ";
        for (auto it = lines.begin(); it != lines.end(); ++it) {
            if (it != lines.begin()) cout << "-";
            cout << *it;
        }
        cout << endl;
    }

    return 0;
}