#include <iostream>      
#include <vector>       
#include <queue>         
#include <map>           
#include <set>           
#include <algorithm>  

using namespace std;    

// структура для узла Trie
struct TrieNode {
    map<char, TrieNode*> children;  // ассоциативный массив для хранения дочерних узлов
    TrieNode* fail;                 // суффиксная ссылка
    TrieNode* terminalLink;         // терминальная ссылка 
    vector<pair<int, int>> output;  // массив, который хранит номера шаблонов, заканчивающихся в этом узле
    int id;                         // уникальный идентификатор узла

    static int counter;             // счетчик для уникальных идентификаторов узлов
    
    TrieNode() : fail(nullptr), terminalLink(nullptr), id(counter++) {} // конструктор узла
};

// метод визуализации структуры Trie
void printAutomaton(TrieNode* root) {
    cout << "\n    Структура построенного trie:\n";
    queue<TrieNode*> q;
    set<TrieNode*> visited;
    q.push(root);
    visited.insert(root);

    while (!q.empty()) {
        TrieNode* node = q.front();
        q.pop();

        cout << "Вершина " << node->id << ":\n";
        if (node->fail)
            cout << "  Суффиксная ссылка -> " << node->fail->id << "\n";
        else
            cout << "  Суффиксная ссылка -> nullptr\n";

        if (node->terminalLink)
            cout << "  Терминальная ссылка -> " << node->terminalLink->id << "\n";

        if (!node->output.empty()) {
            cout << "  Выходы: ";
            for (auto& [pid, _] : node->output)
                cout << pid << " ";
            cout << "\n";
        }

        for (auto& [c, child] : node->children) {
            cout << "  Переход по '" << c << "' -> вершина " << child->id << "\n";
            if (visited.find(child) == visited.end()) {
                q.push(child);
                visited.insert(child);
            }
        }
    }
}

int TrieNode::counter = 0;  // счетчик узлов
int totalNodes = 0;         // общее количество узлов в Trie

// метод построения автомата Ахо-Корасика
void buildTrie(const vector<string>& pieces, TrieNode* root) {

    totalNodes = 1; // корень уже существует
    
    cout << "   Построение trie\n"; 

    // перебор всех шаблонов
    for (int id = 0; id < pieces.size(); ++id) {
        TrieNode* node = root; // добавление с корня
        
        cout << "Добавляем кусок шаблона: \"" << pieces[id] << "\" (id=" << id << ")\n"; 
        
        // перебор символов в текущем шаблоне
        for (char c : pieces[id]) {
            // если символ еще не существует в дочерних узлах
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode(); // создаем новый узел для символа
                ++totalNodes;                       // увеличиваем счетчик узлов
                
                cout << "  Создана вершина " << node->children[c]->id << " по символу '" << c << "'\n"; 
            }
            node = node->children[c];    // переход к дочернему узлу
        }
        node->output.push_back({id, 0}); // запоминаем конец шаблона в узле
        
        cout << "  Отметка конца шаблона в вершине " << node->id << "\n"; 
    }

    queue<TrieNode*> q; // очередь для хранения узлов
    
    // инициализация суффиксных ссылок для детей корня
    for (auto& [c, child] : root->children) {
        child->fail = root; // установка ссылок на корень
        q.push(child);      // добавление узла в очередь
    }

    cout << "\n   Построение суффиксных и терминальных ссылок\n"; 
    
    // поиск в ширину для построения суффиксных ссылок
    while (!q.empty()) { 

        TrieNode* node = q.front(); // получение текущего узла
        q.pop();                    // удаление узла из очереди

        // перебор детей текущего узла
        for (auto& [c, child] : node->children) {
            TrieNode* failNode = node->fail; // инициализация узла для поиска по суффиксной ссылке
            
            // поиск подходящей суффиксной ссылки
            while (failNode && failNode->children.find(c) == failNode->children.end()) {
                failNode = failNode->fail; // переход по суффиксной ссылке
            }

            if (failNode) { 
                child->fail = failNode->children[c]; // если найдена подходящая ссылка, назначение ссылки на узел
                
                cout << "Вершина " << child->id << " получает суффиксную ссылку на " << child->fail->id << "\n"; 
            } else {
                child->fail = root; // если не найдено, указываем на корень
                
                cout << "Вершина " << child->id << " получает суффиксную ссылку на корень\n";
            }

            // установка терминальной ссылки
            if (!child->fail->output.empty()) { 
                child->terminalLink = child->fail; // если узел-предок имеет выходные ссылки, устанавливаем терминальную ссылку
                
                cout << "  Установлена терминальная ссылка на вершину " << child->terminalLink->id << "\n"; 
            } else {
                child->terminalLink = child->fail->terminalLink; // унаследуем ссылку
                
                if (child->terminalLink)
                    cout << "  Унаследована терминальная ссылка на вершину " << child->terminalLink->id << "\n"; 
            }

            q.push(child); // добавляем дочерний узел в очередь
        }
    }

    printAutomaton(root); // визуализация структуры Trie

}

// метод поиска шаблонов в тексте
void search(const string& text, TrieNode* root, const vector<pair<int, int>>& pieceInfo, int patternSize, const vector<string>& pieces) {
    
    int n = text.size();                // размер текста
    vector<int> count(n, 0);            // массив для подсчета вхождений шаблонов
    vector<vector<string>> matches(n);  // массив для хранения найденных совпадений в текстовых позициях
    TrieNode* node = root;              // начинаем с корня

    cout << "\n    Поиск в тексте\n";

    // перебор каждого символа в тексте
    for (int i = 0; i < n; ++i) {
        char c = text[i]; // текущий обрабатываемый символ
        
        cout << "Обработка символа '" << c << "' (позиция " << i << "):\n"; 

        // переход по суффиксным ссылкам, если перехода к символу нет
        while (node && node->children.find(c) == node->children.end()) {
            cout << "  Переход по суффиксной ссылке от вершины " << node->id << "\n"; 
            
            node = node->fail; // переход к родительскому узлу
        }

        // если достигли корня, возвращаемся
        if (!node) {
            node = root; 

            cout << "  Вернулись в корень\n"; 
        } else {
            node = node->children[c]; // переход к дочернему узлу

            cout << "  Перешли в вершину " << node->id << "\n"; 
        }

        TrieNode* temp = node; // указатель для проверки узлов
        
        // проверка выходов из текущего узла
        while (temp) {
            // перебор выходов
            for (auto& [pieceId, _] : temp->output) { 
                int pieceLen = pieceInfo[pieceId].first;    // длина образца
                int piecePos = pieceInfo[pieceId].second;   // позиция образца в шаблоне
                int start = i - pieceLen + 1 - piecePos;    // вычисляем стартовую позицию

                // проверка, что стартовая позиция действительна
                if (start >= 0 && start + patternSize <= n) {
                    count[start]++;                             // увеличиваем счетчик для найденного вхождения
                    matches[start].push_back(pieces[pieceId]);  // добавление совпадающего образца в позицию
                    
                    cout << "    Найден кусок \"" << pieces[pieceId] << "\" => возможное вхождение шаблона на позиции " << start + 1 << "\n"; 
                }
            }
            temp = temp->terminalLink; // переход к терминальной ссылке
        }
    }

    vector<int> resultPositions; // массив для хранения фактических позиций вхождений шаблона
    
    // перебор позиций в тексте
    for (int i = 0; i <= n - patternSize; ++i) {
        if (count[i] == pieceInfo.size()) {  
            resultPositions.push_back(i); // если количество вхождений соответствует количеству образцов, сохраняем позицию
        }
    }

    cout << "Позиции вхождения шаблона:\n"; 
    for (int pos : resultPositions) { 
        cout << pos + 1 << endl; // 
    }

    // поиск пересекающихся шаблонов
    set<string> overlappingPatterns;                        // множество для хранения пересекающихся образцов
    sort(resultPositions.begin(), resultPositions.end());   // сортируем позиции

    // проверка пересечений между образцами
    for (int i = 0; i < resultPositions.size(); ++i) {
        int startA = resultPositions[i];     // начало первого вхождения
        int endA = startA + patternSize - 1; // конец первого вхождения

        for (int j = i + 1; j < resultPositions.size(); ++j) {
            int startB = resultPositions[j]; // начало второго вхождения

            // проверка на пересечение
            if (startB <= endA) { 
                for (const string& p : matches[startA]) overlappingPatterns.insert(p); // сохранение образцов из первого вхождения
                for (const string& p : matches[startB]) overlappingPatterns.insert(p); // сохранение образцов из второго вхождения
            } else break; 
        }
    }

    cout << "\nОбразцы, участвующие в пересечениях:\n"; 
    
    // если найден только один шаблон
    if (resultPositions.size() < 2) { 
        for (const string& p : pieces) { // вывод всех образцов
            cout << p << endl;
        }
    } else { // если есть несколько шаблонов
        for (const string& pat : overlappingPatterns) { // перебор пересекающихся образцов
            cout << pat << endl; 
        }
    }

    cout << "\nКоличество вершин в автомате: " << totalNodes << endl; 
}

int main() {
    string text, pattern;   // переменные для хранения текста и шаблона
    char wildcard;          // символ джокера
    cin >> text >> pattern >> wildcard; 

    TrieNode* root = new TrieNode();    // создание корневого узла Trie
    vector<string> pieces;              // массив для хранения частей шаблона
    vector<pair<int, int>> pieceInfo;   // массив для хранения информации о частях шаблона

    int m = pattern.size(); // размер шаблона

    // перебор символов в шаблоне
    for (int i = 0; i < m;) { 
        if (pattern[i] == wildcard) { // 
            ++i;      // если символ - джокер, пропускаем этот символ
            continue; // переход к следующему
        }

        int j = i;      // начинаем с текущего символа
        string piece;   // строка для хранения текущей части шаблона
       
        // считываем всю часть шаблона до джокера
        while (j < m && pattern[j] != wildcard) {
            piece += pattern[j]; // добавляем символ к части
            ++j;                 // переход к следующему символу
        }

        pieces.push_back(piece);                        // сохраняем часть шаблона в вектор
        pieceInfo.push_back({(int)piece.size(), i});    // сохраняем информацию о части шаблона
        i = j;                                          // обновляем индекс
    }

    cout << "\nНайденные куски шаблона:\n"; 
    for (int i = 0; i < pieces.size(); ++i) { 
        cout << "  \"" << pieces[i] << "\" (позиция в шаблоне: " << pieceInfo[i].second << ")\n";
    }
    cout << "\n"; 

    buildTrie(pieces, root);                                // построение Trie на основе найденных частей
    search(text, root, pieceInfo, pattern.size(), pieces);  // поиск шаблонов в тексте

    return 0; 
}
