#include <iostream>
#include <string>
#include <vector>

using namespace std;

// функция вычисления префиксного массива 
vector<int> computeKMPTable(const string &P) {
    int m = P.size();           // размер строки P
    vector<int> prefix(m, 0);   // инициализация массива prefix нулями
    int len = 0;                // длина предыдущего наибольшего префикса
    int i = 1;                  // текущая позиция в строке P
    
    cout << "Вычисление префикс-функции для " << P << endl;

    // проход по строке P для заполнения массива prefix
    while (i < m) {

        cout << "i = " << i << ", len = " << len << ", сравниваем P[" << i << "] = " << P[i] << " и P[" << len << "] = " << P[len] << endl;
        
        if (P[i] == P[len]) {       // символы совпадают
            len++;                  // увеличение длины 
            prefix[i] = len;        // сохранение длины в prefix

            cout << "   Совпадение! prefix[" << i << "] = " << len << endl;
            
            i++;                    // увеличение позиции в строке P 
        } else {
            // символы не совпадают
            if (len != 0) {

                cout << "  Несовпадение! len меняем с " << len << " на prefix[" << len-1 << "] = " << prefix[len-1] << endl;
                
                len = prefix[len - 1];  // переход к предыдущему префиксу
            } else {

                cout << "  Несовпадение! prefix[" << i << "] = 0" << endl;
                
                prefix[i] = 0;          // нет префикса, установка 0
                i++;                    // увеличение позиции в строке P 
            }
        }
    }
    
    cout << "Итоговый префикс-массив: [ ";
    for (int val : prefix) cout << val << " ";
    cout << "]\n" << endl;

    return prefix;
}

// функция поиска всех вхождений строки P в строке T 
int KMPSearch(const string &text, const string &pattern) {
    vector<int> lps = computeKMPTable(pattern); // вычисление массива prefix
    int n = text.size(), m = pattern.size();    // размеры строк P и T
    int j = 0;                                  // индекс в P

    cout << "Начинаем поиск шаблона '" << pattern << "' в тексте '" << text << "'" << endl;

    // проход по всем символам текста
    for (int i = 0; i < n; ++i) {
        
        cout << "i = " << i << ", text[" << i << "] = " << text[i] << ", pattern[" << j << "] = " << (j < m ? pattern[j] : '-') << endl;

        // если несовпадение, откатываемся до последнего возможного совпадения
        while (j > 0 && text[i] != pattern[j]) {
            
            cout << "  Несовпадение, откат j = lps[" << j - 1 << "] = " << lps[j - 1] << endl;
            
            j = lps[j - 1]; // откатываем j, используя prefix
        }

        // символ текста совпадает с символом шаблона
        if (text[i] == pattern[j]) {
            ++j; // увеличиваем j, чтобы проверить следующий символ шаблона
            
            cout << "  Совпадение, увеличиваем j до " << j << endl;
        }

        // если j достигло длины шаблона, значит мы нашли совпадение
        if (j == m) {
            
            cout << "Шаблон найден на позиции " << (i - m + 1) << endl; 
            
            return i - m + 1; 
        }
    }

    cout << "Шаблон не найден в тексте." << endl; 
    
    return -1; 
}

int main() {
    string A, B; // строки для поиска и текст
    cin >> A;   
    cin >> B;   

    // проверка на равенство длины строк
    if (A.size() != B.size()) {
        cout << "Строки разной длины. Ответ: -1" << endl; 
        return 0;
    }

   
    string doubleA = A + A;              // создание удвоенной строки A для учета циклических сдвигов
    int index = KMPSearch(doubleA, B);   // запуск поиска
    
    cout << "Минимальное количество сдвигов: " << index << endl; 

    return 0; 
}
