#include <iostream>   
#include <vector>    
#include <queue>      
#include <string>     
#include <map>       
#include <algorithm>  
#include <set>     

using namespace std; 

const int ALPHABET_SIZE = 5;                            // размер алфавита
const char CHAR_TO_INDEX[] = {'A', 'C', 'G', 'T', 'N'}; // массив для преобразования символов в индексы

// структура для узла в Trie 
struct TrieNode {
    map<char, TrieNode*> children;   // ассоциативный массив для хранения дочерних узлов
    TrieNode* fail;                  // суффиксная ссылка
    TrieNode* terminalLink;          // терминальная ссылка 
    vector<int> output;              // массив, который хранит номера шаблонов, заканчивающихся в этом узле

    TrieNode() : fail(nullptr), terminalLink(nullptr) {} // инициализация узла
};

// функция преобразования символа в индекс
int charToIndex(char c) {
    switch (c) {
        case 'A': return 0; 
        case 'C': return 1; 
        case 'G': return 2; 
        case 'T': return 3; 
        default:  return 4; 
    }
}

// метод визуализации структуры Trie
void printTrie(TrieNode* node, const vector<string>& patterns, const string& prefix = "", const string& indent = "", bool isLast = true) {
    static bool isRoot = true; // переменная для отслеживания корня
    if (isRoot) {
        cout << "Корень\n"; 
        isRoot = false;   
    }

    // определение ветки для визуализации узла
    string branch = indent + (isLast ? "└── " : "├── "); 
    if (!prefix.empty()) { 
        cout << branch << prefix.back(); // вывод последнего символа префикса

        // если выходные ссылки существуют, вывести их
        if (!node->output.empty()) {
            cout << " (конец шаблона "; 
            for (size_t i = 0; i < node->output.size(); ++i) {
                int patternIndex = node->output[i]; 
                cout << "[" << patternIndex << "] - \"" << patterns[patternIndex] << "\""; 
                if (i + 1 < node->output.size()) cout << ", "; 
            }
            cout << ")"; 
        }
        cout << "\n"; 
    }

    // рекурсивный вызов для всех детей узла
    auto it = node->children.begin();    // итератор для перебора детей
    while (it != node->children.end()) { 
        string nextIndent = indent + (isLast ? "    " : "│   "); 
        bool childIsLast = next(it) == node->children.end();
        printTrie(it->second, patterns, prefix + it->first, nextIndent, childIsLast); 
        ++it; 
    }
}

int nodeCount = 0; // счетчик узлов в Trie

// метод построения автомата Ахо-Корасика
void buildTrie(const vector<string>& patterns, TrieNode* root) {

    cout << "Построение trie:\n"; 

    // перебор всех шаблонов
    for (int i = 0; i < patterns.size(); ++i) { 
        TrieNode* node = root; // начанием с корня
        
        cout << "  Шаблон " << i + 1 << ": " << patterns[i] << endl; 
        
        // перебор всех символов в шаблоне
        for (char c : patterns[i]) { 
            int index = charToIndex(c); // индекс текущего символа

            // если символ не найден среди детей текущего узла
            if (node->children.find(CHAR_TO_INDEX[index]) == node->children.end()) {

                node->children[CHAR_TO_INDEX[index]] = new TrieNode();  // создаем новый узел для этого символа
                nodeCount++;                                            // увеличиваем счетчик узлов
                
                cout << "    Добавлена вершина для символа '" << c << "'\n"; 
            
            } else {
                
                // если узел уже существует, переходим к нему
                cout << "    Переход по уже существующему символу '" << c << "'\n"; 
            }

            node = node->children[CHAR_TO_INDEX[index]]; // переход к дочернему узлу
        }

        node->output.push_back(i);                        // номер шаблона в выходных ссылках узла

        cout << "     Отметка конца шаблона в вершине\n"; 
    }

    // построение суффиксных и терминальных ссылок
    cout << "\n Построение суффиксных и терминальных ссылок:\n"; 
    
    queue<TrieNode*> q; // очередь для работы с узлами
    
    // перебор дочерних узлов корня
    for (auto& pair : root->children) { 
        pair.second->fail = root; // установка суффиксной ссылки на корень
        q.push(pair.second);      // добавление узела в очередь для дальнейшей обработки
        
        cout << "  Корневой потомок '" << pair.first << "' получает ссылку на корень\n"; 
    }

    // поиск в ширину для создания суффиксных ссылок
    while (!q.empty()) { 
        TrieNode* node = q.front(); // получение текущего узела из очереди
        q.pop();                    // удаление его из очереди

        // перебор детей текущего узла
        for (auto& pair : node->children) {
            char c = pair.first;             // текущий символ
            TrieNode* child = pair.second;   // дочерний узел
            TrieNode* failNode = node->fail; // переменная для поиска суффиксной ссылки

            // поиск подходящей суффиксной ссылки
            while (failNode != nullptr && failNode->children.find(c) == failNode->children.end()) {
                failNode = failNode->fail; // переход по суффиксной ссылке
            }

            // установка суффиксной ссылки
            if (failNode == nullptr) {
                child->fail = root;                  // если подходящий узел не найден, указываем на корень
            } else {
                child->fail = failNode->children[c]; // устанавливаем ссылку на найденный узел
            }

            cout << "  Вершина по '" << c << "' получает суффиксную ссылку на "; 
            if (child->fail == root) cout << "корень\n"; 
            else cout << "другую вершину по '" << c << "'\n"; 

            // установка терминальных ссылок
            if (!child->fail->output.empty()) {     
                child->terminalLink = child->fail;               // если родительский узел имеет выходные ссылки, устанавливаем терминальную ссылку
            } else {
                child->terminalLink = child->fail->terminalLink; // унаследуем ссылку
            }

            cout << "  Вершина по '" << c << "' получает терминальную ссылку на ";
            if (child->terminalLink == nullptr) {
                cout << "ничего (nullptr)\n";
            } else if (!child->terminalLink->output.empty()) {
                cout << "вершину с шаблонами: ";
                for (size_t i = 0; i < child->terminalLink->output.size(); ++i) {
                    int patIndex = child->terminalLink->output[i];
                    cout << "\"" << patterns[patIndex] << "\"";
                    if (i + 1 < child->terminalLink->output.size()) cout << ", ";
                }
                cout << endl;
            } else {
                cout << "вершину (унаследованную), но без собственного шаблона\n";
            }
            cout<<endl;

            q.push(child); // добавляем дочерний узел в очередь
        }
    }

    cout << "\nСтруктура построенного trie:\n"; 
    printTrie(root, patterns); // визуализация структуры Trie
}

// метод поиска всех вхождений шаблонов в текст
void searchPatterns(const string& text, TrieNode* root, vector<string>& patterns) {
    TrieNode* node = root;              // начинаем с корня
    vector<pair<int, int>> results;     // массив для хранения результатов поиска
    vector<pair<int, int>> intervals;   // массив для хранения интервалов найденных шаблонов
    map<int, string> idToPattern;       // ассоциативный массив для сопоставления идентификаторов шаблонов с их текстом

    // заполнение ассоциативного массива для сопоставления идентификаторов шаблонов с их текстом
    for (int i = 0; i < patterns.size(); ++i)
        idToPattern[i + 1] = patterns[i]; // сохраняем каждый шаблон с его индексом

    cout << "\n Поиск шаблонов в тексте:\n"; 

    // проход по каждому символу текста
    for (int i = 0; i < text.size(); ++i) {
        char currentChar = text[i];             // текущий символ текста
        int index = charToIndex(currentChar);   // индекс текущего символа

        cout << "  Позиция " << i << ": символ '" << currentChar << "' — "; 

        // переход по суффиксным ссылкам, если нет перехода к дочернему узлу 
        while (node != nullptr && node->children.find(CHAR_TO_INDEX[index]) == node->children.end()) {
            
            cout << "нет перехода к дочернему узлу, переходим по суффиксной ссылке "; 
            
            node = node->fail;                        // переход по суффиксной ссылке
            if (node == nullptr) cout << "к корню\n"; // если достигли корня
        }

        // если достигли корня, перезапускаем поиск
        if (node == nullptr) {
            node = root; // сброс узела на корень
            
            cout << "перешли к корню\n"; 
        } else {
            node = node->children[CHAR_TO_INDEX[index]]; // переход к дочернему узлу
            
            cout << "переход выполнен\n"; 
        }

        TrieNode* temp = node; // указатель на текущий узел

        // проверка всех выходов в текущем узле
        while (temp != nullptr) { 
            // перебор выходных индексов
            for (int patternIndex : temp->output) { 
                int startPos = i - patterns[patternIndex].size() + 2; // начальная позиция вхождения
                results.push_back({startPos, patternIndex + 1});      // сохранение результатов
                intervals.push_back({startPos, i + 1});               // сохранение интервала вхождения

                cout << "    Найден шаблон " <<  patterns[patternIndex] << " на позиции " << startPos << endl; 
            }
            temp = temp->terminalLink; // переход к терминальной ссылке
        }
    }

    sort(results.begin(), results.end()); // сортируем результаты по позициям

    cout << "\nНайденные вхождения:\n"; 
    for (const auto& res : results) { 
        cout << res.first << " " << res.second << "\n"; 
    }

    // анализ пересечений
    set<int> overlappingPatterns;              // множество для хранения пересекающихся шаблонов
    sort(intervals.begin(), intervals.end());  // сортируем интервалы

    // поиск пересекающихся шаблонов
    // перебор всех интервалов
    for (int i = 0; i < intervals.size(); ++i) {            
        // сравнение с последующими интервалами
        for (int j = i + 1; j < intervals.size(); ++j) { 
            // проверка на пересечение
            if (intervals[i].second >= intervals[j].first) { 
                // сохранение идентификаторов пересекающихся шаблонов
                overlappingPatterns.insert(results[i].second); 
                overlappingPatterns.insert(results[j].second);
            } else break;
        }
    }

    cout << "\nОбразцы с пересечениями:\n"; 
    for (int id : overlappingPatterns) { 
        cout << idToPattern[id] << "\n"; 
    }
}

int main() {
    string text; // стока для хранения текста для поиска
    cin >> text; 

    int n;       // количество шаблонов
    cin >> n; 

    vector<string> patterns(n); // массив для хранения шаблонов
    for (int i = 0; i < n; ++i) { 
        cin >> patterns[i]; 
    }

    TrieNode* root = new TrieNode();        // создание корневого узла Trie
    nodeCount = 1;                          // начальное значение счетчика узлов
    buildTrie(patterns, root);              // построение Trie на основе шаблонов
    searchPatterns(text, root, patterns);   // поиск шаблонов в тексте

    cout << "\nКоличество вершин в автомате: " << nodeCount << endl;

    return 0; 
}
