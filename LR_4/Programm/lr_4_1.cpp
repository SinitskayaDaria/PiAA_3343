#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

// функция проверки состоит ли строка только из латиницы
bool isLatin(const string &text) {
    for (char ch : text) {
        if (!isalpha(ch) || (!isupper(ch) && !islower(ch))) {
            return false;
        }
    }
    return true;
}

// функция вычисления префиксного массива 
vector<int> computePrefix(const string &P) {
    int m = P.size();           // размер строки P
    vector<int> prefix(m, 0);   // инициализация массива prefix нулями
    int len = 0;                // длина наибольшего префикса
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
vector<int> KMPSearch(const string &P, const string &T) {
    int m = P.size(), n = T.size();             // размеры строк P и T
    vector<int> prefix = computePrefix(P);      // вычисление массива prefix
    vector<int> result;                         // массив для хранения позиций вхождений
    int i = 0, j = 0;                           // i - индекс в T, j - индекс в P

    cout << "Начало поиска в тексте " << T << " :" << endl;

    while (i < n) {

        if (P[j] == T[i]) {     // символы совпадают

            cout << "Сравниваем T[" << i << "] = " << T[i] << " и P[" << j << "] = " << P[j] << endl;
            cout << "  Совпадение! Увеличиваем i и j (" << i << " -> " << i+1 << ", " << j << " -> " << j+1 << ")\n";
            
            i++;                // переход к следующему символу в строке T
            j++;                // переход к следующему символу в строке P
        }

        if (j == m) {           // все символы P совпали с T
            int pos = i - j;
            cout << ">>> Найдено вхождение на позиции " << pos << "!\n";
            
            result.push_back(pos);  // сохранение позиции начала вхождения
            j = prefix[j - 1];      // обновление j согласно prefix
            
            cout << "  j сброшен до prefix[" << m-1 << "] = " << j << endl;
        
        } else if (i < n && P[j] != T[i]) {     // символы не совпадают

            cout << "Сравниваем T[" << i << "] = " << T[i] << " и P[" << j << "] = " << P[j] << endl;
            
            if (j != 0) {
        
                cout << "  Несовпадение! j меняем с " << j << " на prefix[" << j-1 << "] = " << prefix[j-1] << endl;
                
                j = prefix[j - 1];      // обновление j согласно prefix, если он не нулевой
            } else {

                cout << "  Несовпадение! j = 0 -> увеличиваем i (" << i << " -> " << i+1 << ")\n";
                
                i++;    // j == 0, переходим к следующему символу в T
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

    vector<int> positions = KMPSearch(P, T);    // поиск вхождения P в T

    cout << endl <<"Результат работы программы: " << endl;

    // проверка найдены ли позиции
    if (positions.empty()) {
        cout <<"Вхождений нет ";
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
