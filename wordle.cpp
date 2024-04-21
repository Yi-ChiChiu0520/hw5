#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// Helper function prototypes
void generate(int pos, string &current, const string &fixed, const map<char, int> &floatingOriginal, map<char, int> &floatingRemaining, const set<string> &dict, set<string> &results);

// Main wordle function
set<string> wordle(const string& in, const string& floating, const set<string>& dict) {
    string current = string(in.size(), ' ');  // Placeholder for current word being built
    map<char, int> floatingCounts;  // Counts of each floating letter
    
    // Count each floating letter
    for (char ch : floating) {
        floatingCounts[ch]++;
    }

    // Prepare results set
    set<string> results;

    // Start the recursive generation
    generate(0, current, in, floatingCounts, floatingCounts, dict, results);

    return results;
}

// Recursive function to generate valid words
void generate(int pos, string &current, const string &fixed, const map<char, int> &floatingOriginal, map<char, int> &floatingRemaining, const set<string> &dict, set<string> &results) {
    if (pos == fixed.size()) {  // Base case: when the word is fully constructed
        if (dict.find(current) != dict.end()) {  // Check if the word is in the dictionary
            results.insert(current);
        }
        return;
    }

    if (fixed[pos] != '-') {  // If the position is already fixed
        current[pos] = fixed[pos];
        generate(pos + 1, current, fixed, floatingOriginal, floatingRemaining, dict, results);
    } else {
        // Try all possible letters for this position
        for (auto& pair : floatingRemaining) {
            if (pair.second > 0) {  // Only use letters that are still available
                current[pos] = pair.first;
                pair.second--;  // Use this letter now
                generate(pos + 1, current, fixed, floatingOriginal, floatingRemaining, dict, results);
                pair.second++;  // Backtrack: restore the count
            }
        }

        // Also try any letter that is not in floating (could be improved by limiting to a-z)
        for (char c = 'a'; c <= 'z'; ++c) {
            if (floatingOriginal.find(c) == floatingOriginal.end()) {  // If 'c' is not a floating letter
                current[pos] = c;
                generate(pos + 1, current, fixed, floatingOriginal, floatingRemaining, dict, results);
            }
        }
    }
}

#ifndef RECCHECK
// Example usage
int main() {
    // Sample dictionary
    set<string> dict = {"bind", "dine", "ding", "dins", "dint", "find", "hind", "kind", "mind", "rind", "wind"};

    // Test case
    set<string> results = wordle("-i--", "dn", dict);

    // Print results
    for (const string& word : results) {
        cout << word << endl;
    }

    return 0;
}
#endif
