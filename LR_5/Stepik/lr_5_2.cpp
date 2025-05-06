#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

const int ALPHABET_SIZE = 5;  
const char ALPHABET[] = {'A', 'C', 'G', 'T', 'N'};

struct TrieNode {
    map<char, TrieNode*> children;
    TrieNode* fail;
    TrieNode* terminalLink;
    vector<pair<int, int>> output;  

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

void buildTrie(const vector<string>& pieces, TrieNode* root) {
    for (int id = 0; id < pieces.size(); ++id) {
        TrieNode* node = root;
        for (char c : pieces[id]) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->output.push_back({id, 0});
    }

    queue<TrieNode*> q;
    for (auto& p : root->children) {
        p.second->fail = root;
        q.push(p.second);
    }

    while (!q.empty()) {
        TrieNode* node = q.front();
        q.pop();

        for (auto& p : node->children) {
            char c = p.first;
            TrieNode* child = p.second;

            TrieNode* failNode = node->fail;
            while (failNode && failNode->children.find(c) == failNode->children.end()) {
                failNode = failNode->fail;
            }
            if (failNode) {
                child->fail = failNode->children[c];
            } else {
                child->fail = root;
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

void search(const string& text, TrieNode* root, const vector<pair<int,int>>& pieceInfo, int patternSize) {
    int n = text.size();
    vector<int> count(n, 0);

    TrieNode* node = root;
    for (int i = 0; i < n; ++i) {
        char c = text[i];

        while (node != nullptr && node->children.find(c) == node->children.end()) {
            node = node->fail;
        }
        if (node == nullptr) {
            node = root;
        } else {
            node = node->children[c];
        }

        TrieNode* temp = node;
        while (temp != nullptr) {
            for (auto& [pieceId, _] : temp->output) {
                int pieceLen = pieceInfo[pieceId].first;
                int piecePos = pieceInfo[pieceId].second;

                int startPos = i - pieceLen + 1 - piecePos;
                if (startPos >= 0 && startPos + patternSize <= n) {
                    count[startPos]++;
                }
            }
            temp = temp->terminalLink;
        }
    }

    int totalPieces = pieceInfo.size();
    for (int i = 0; i <= n - patternSize; ++i) {
        if (count[i] == totalPieces) {
            cout << i + 1 << endl;  
        }
    }
}

int main() {
    string text, pattern;
    char wildcard;
    cin >> text >> pattern >> wildcard;

    TrieNode* root = new TrieNode();
    vector<string> pieces;
    vector<pair<int, int>> pieceInfo; 

    int m = pattern.size();
    for (int i = 0; i < m;) {
        if (pattern[i] == wildcard) {
            ++i;
            continue;
        }
        int j = i;
        string piece;
        while (j < m && pattern[j] != wildcard) {
            piece += pattern[j];
            ++j;
        }
        pieces.push_back(piece);
        pieceInfo.push_back({(int)piece.size(), i});
        i = j;
    }

    buildTrie(pieces, root);
    search(text, root, pieceInfo, pattern.size());

    return 0;
}
