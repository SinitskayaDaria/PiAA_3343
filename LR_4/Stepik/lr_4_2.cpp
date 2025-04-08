#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> computeKMPTable(const string &pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);
    int j = 0;
    for (int i = 1; i < m; ++i) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = lps[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            ++j;
            lps[i] = j;
        }
    }
    return lps;
}

int KMPSearch(const string &text, const string &pattern) {
    vector<int> lps = computeKMPTable(pattern);
    int n = text.size(), m = pattern.size();
    int j = 0;
    for (int i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) {
            j = lps[j - 1];
        }
        if (text[i] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            return i - m + 1;
        }
    }
    return -1;
}

int main() {
    string A, B;
    cin >> A >> B;
    
    if (A.size() != B.size()) {
        cout << -1 << endl;
        return 0;
    }
    
    string doubleA = A + A;
    int index = KMPSearch(doubleA, B);
    cout << index << endl;
    
    return 0;
}