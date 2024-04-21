#include "wordle.h"
#include "dict-eng.h"  // Ensure you have this header for dictionary definitions
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

void generateWords(const string& in, string& currentWord, vector<int>& floatingCount, const set<string>& dict, set<string>& results, size_t index) {
    if (index == in.size()) {
        // Check if all floating letters are used by finding any count greater than 0
        if (std::find_if(floatingCount.begin(), floatingCount.end(), [](int count){ return count > 0; }) == floatingCount.end()) {
            // If currentWord forms a valid word in dictionary, add to results
            if (dict.find(currentWord) != dict.end()) {
                results.insert(currentWord);
            }
        }
        return;
    }

    if (in[index] != '-') {
        // Fixed character, use it
        currentWord[index] = in[index];
        generateWords(in, currentWord, floatingCount, dict, results, index + 1);
    } else {
        // Try all possible letters from a-z
        for (char c = 'a'; c <= 'z'; ++c) {
            currentWord[index] = c;
            int idx = c - 'a';
            if (floatingCount[idx] > 0) {
                floatingCount[idx]--;
                generateWords(in, currentWord, floatingCount, dict, results, index + 1);
                floatingCount[idx]++; // Backtrack
            } else if (std::find(in.begin(), in.end(), c) == in.end()) { // Check if it is not part of fixed characters
                generateWords(in, currentWord, floatingCount, dict, results, index + 1);
            }
        }
    }
}

std::set<std::string> wordle(const std::string& in, const std::string& floating, const std::set<std::string>& dict) {
    set<string> results;
    string currentWord = in;
    vector<int> floatingCount(26, 0);

    for (char c : floating) {
        floatingCount[c - 'a']++;
    }

    generateWords(in, currentWord, floatingCount, dict, results, 0);
    return results;
}
