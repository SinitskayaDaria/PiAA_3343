#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

const int ALPHABET_SIZE = 5;  
const char CHAR_TO_INDEX[] = {'A', 'C', 'G', 'T', 'N'};

struct TrieNode {
    map<char, TrieNode*> children; 
    TrieNode* fail;                 
    TrieNode* terminalLink;     
    vector<int> output;         
    
    TrieNode() : fail(nullptr), terminalLink(nullptr) {}
};

int charToIndex(char c) {
    switch (c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default:  return 4;  
    }
}

void buildTrie(const vector<string>& patterns, TrieNode* root) {
    for (int i = 0; i < patterns.size(); ++i) {
        TrieNode* node = root;
        for (char c : patterns[i]) {
            int index = charToIndex(c);
            if (node->children.find(CHAR_TO_INDEX[index]) == node->children.end()) {
                node->children[CHAR_TO_INDEX[index]] = new TrieNode();
            }
            node = node->children[CHAR_TO_INDEX[index]];
        }
        node->output.push_back(i); 
    }
    
    queue<TrieNode*> q;
    for (auto& pair : root->children) {
        pair.second->fail = root;
        pair.second->terminalLink = nullptr;
        q.push(pair.second);
    }
    
    while (!q.empty()) {
        TrieNode* node = q.front();
        q.pop();
        
        for (auto& pair : node->children) {
            char c = pair.first;
            TrieNode* child = pair.second;
            
            TrieNode* failNode = node->fail;
            while (failNode != nullptr && failNode->children.find(c) == failNode->children.end()) {
                failNode = failNode->fail;
            }
            
            if (failNode == nullptr) {
                child->fail = root;
            } else {
                child->fail = failNode->children[c];
            }
            
            if (!child->fail->output.empty()) {
                child->terminalLink = child->fail;
            } else {
                child->terminalLink = child->fail->terminalLink;
            }
            
            q.push(child);
        }
    }
}

void searchPatterns(const string& text, TrieNode* root, vector<string>& patterns) {
    TrieNode* node = root;
    vector<pair<int, int>> results;  

    for (int i = 0; i < text.size(); ++i) {
        int index = charToIndex(text[i]);
        
        while (node != nullptr && node->children.find(CHAR_TO_INDEX[index]) == node->children.end()) {
            node = node->fail;
        }
        
        if (node == nullptr) {
            node = root;
        } else {
            node = node->children[CHAR_TO_INDEX[index]];
        }

        TrieNode* temp = node;
        while (temp != nullptr) {
            for (int patternIndex : temp->output) {
                results.push_back({i - patterns[patternIndex].size() + 2, patternIndex + 1});
            }
            temp = temp->terminalLink;
        }
    }

    sort(results.begin(), results.end());

    for (const auto& res : results) {
        cout << res.first << " " << res.second << endl;
    }
}

int main() {
    string text;
    cin >> text;

    int n;
    cin >> n;

    vector<string> patterns(n);
    for (int i = 0; i < n; ++i) {
        cin >> patterns[i];
    }

    TrieNode* root = new TrieNode();
    buildTrie(patterns, root);
    searchPatterns(text, root, patterns);

    return 0;
}
