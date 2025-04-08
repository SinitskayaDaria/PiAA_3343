#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

// функция проверки состоит ли строка только из латиницы
bool isLatin(const string &text) {
    for (char ch : text) {
        if (!isalpha(ch) || (ch < 'a' && ch > 'Z')) { 
            return false; 
        }
    }
    return true; 
}

// функция вычисления префиксного массива 
vector<int> computePrefix(const string &P) {
    int m = P.size();            // размер строки P
    vector<int> prefix(m, 0);    // инициализация массива prefix нулями
    int len = 0;                 // длина предыдущего наибольшего префикса
    int i = 1;                   // текущая позиция в строке P
    
    // проход по строке P для заполнения массива prefix
    while (i < m) {
        if (P[i] == P[len]) {           // символы совпадают
            prefix[i++] = ++len;        // увеличение длины и сохранение в prefix
        } else {
            // символы не совпадают
            if (len != 0) {
                len = prefix[len - 1];     // переход к предыдущему префиксу
            } else {
                prefix[i++] = 0;           // нет префикса, установка 0
            }
        }
    }
    return prefix; 
}

// функция поиска всех вхождений строки P в строке T 
vector<int> KMPSearch(const string &P, const string &T) {
    int m = P.size(), n = T.size();             // размеры строк P и T
    vector<int> prefix = computePrefix(P);      // вычисление массива prefix
    vector<int> result;                         // массив для хранения позиций вхождений
  
    int i = 0, j = 0;                           // i - индекс в T, j - индекс в P
    while (i < n) {
        if (P[j] == T[i]) {                     // символы совпадают
            i++, j++;                           // переход к следующему символу в обеих строках
        }
        if (j == m) {                           // все символы P совпали с T
            result.push_back(i - j);            // сохранение позиции начала вхождения
            j = prefix[j - 1];                  // обновление j согласно prefix
        } else if (i < n && P[j] != T[i]) {     // символы не совпадают
            if (j != 0) {
                j = prefix[j - 1];              // обновление j согласно prefix, если он не нулевой
            } else {
                i++;                            // j == 0, переходим к следующему символу в T
            }
        }
    }
    return result; 
}

int main() {
    string P, T;        // строки для поиска и текст
    cin >> P >> T;    

    // проверка обе строки состоят только из латиницы
    if (!isLatin(P) || !isLatin(T)) {
        cout << "Ошибка: строки должны содержать только латинские буквы." << endl;
        return 1; 
    }

    vector<int> positions = KMPSearch(P, T); // поиск вхождения P в T

    // проверка найдены ли позиции
    if (positions.empty()) {
        cout << "-1";          
    } else {
        for (size_t i = 0; i < positions.size(); i++) {
            if (i > 0) cout << ",";
            cout << positions[i];
        }
    }
    cout << endl; 

    return 0; 
}
