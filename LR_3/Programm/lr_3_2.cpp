#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// метод печати таблицы LCS
void printLCSTable(const vector<vector<int>>& lcs, const string& A, const string& B, int m, int n) {

    cout << "\nТаблица значений LCS:\n\n";
    cout << "    ";
    for (int j = 0; j <= n; ++j) {
        if (j == 0) cout << "  ";
        else cout << " " << B[j - 1] << " ";
    }
    cout << endl;

    for (int i = 0; i <= m; ++i) {
        if (i == 0) cout << "  ";
        else cout << A[i - 1] << " ";

        for (int j = 0; j <= n; ++j) {
            cout << setw(2) << lcs[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// функция вычисление длины наибольшей общей подпоследовательности
int computeLCSLength(const string& A, const string& B, int m, int n) {

    vector<vector<int>> lcs(m + 1, vector<int>(n + 1, 0)); // массив для хранения значений LCS

    cout << "\nНачинаем вычисление длины наибольшей общей подпоследовательности (LCS):\n";

     // заполнение массива для хранения значений LCS
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {

            // если символы совпадают, увеличиваем длину на 1
            if (A[i - 1] == B[j - 1]) {
                lcs[i][j] = lcs[i - 1][j - 1] + 1;
                
                cout << "lcs[" << i << "][" << j << "] (match " << A[i - 1] << ") = "
                     << "lcs[" << (i - 1) << "][" << (j - 1) << "] + 1 = " << lcs[i][j] << endl;
            
            } else {

                // иначе берем максимум между LCS без текущего символа A или B
                lcs[i][j] = max(lcs[i - 1][j], lcs[i][j - 1]);

                cout << "lcs[" << i << "][" << j << "] = max("
                     << "lcs[" << (i - 1) << "][" << j << "] = " << lcs[i - 1][j] << ", "
                     << "lcs[" << i << "][" << (j - 1) << "] = " << lcs[i][j - 1] << ") = "
                     << lcs[i][j] << endl;
            }
        }
    }

    printLCSTable(lcs, A, B, m, n);

    return lcs[m][n];
}

// Функция для вывода таблицы расстояний
void printDPTable(const vector<vector<int>>& dp, const string& S, const string& T) {
    int m = S.size();
    int n = T.size();

    cout << "\nТаблица расстояний (dp):\n\n";

    // Верхняя строка — символы строки T
    cout << setw(4) << " ";
    cout << setw(4) << "' '";  // пустой символ перед T
    for (char c : T)
        cout << setw(4) << c;
    cout << endl;

    // Вся таблица
    for (int i = 0; i <= m; ++i) {
        if (i == 0)
            cout << setw(4) << "' '";  // пустой символ перед S
        else
            cout << setw(4) << S[i - 1];

        for (int j = 0; j <= n; ++j) {
            cout << setw(4) << dp[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

// метод инициализации таблицы
void initializeDP(vector<vector<int>>& dp, int m, int n) {
    // первая строка соответствует расстоянию от пустой строки T к префиксам строки S
    for (int i = 0; i <= m; ++i) dp[i][0] = i;  // расстояние до пустой строки T
    
    // первая колонка соответствует расстоянию от пустой строки S к префиксам строки T
    for (int j = 0; j <= n; ++j) dp[0][j] = j;  // расстояние до пустой строки S
}

// метод заполнения таблицы расстояний Левенштейна
void fillDPTable(const string& S, const string& T, vector<vector<int>>& dp) {
   
    int m = S.size(), n = T.size(); // длины строк А и В

    // заполнение таблицы 
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {

            cout << "Сравниваем S[" << i - 1 << "] = " << S[i - 1] << " и T[" << j - 1 << "] = " << T[j - 1] << "\n";

            // если текущие символы совпадают, переносим значение диагонально
            if (S[i - 1] == T[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
                
                cout << "  Совпадают -> операция: M (match), стоимость: " << dp[i][j] << "\n";
            
            } else {
                
                // берем минимум из трех операций: удаление, вставка, замена
                dp[i][j] = min({
                    dp[i - 1][j] + 1,     // удаление (S[i-1] не учитываем)
                    dp[i][j - 1] + 1,     // вставка (добавляем T[j-1] к S)
                    dp[i - 1][j - 1] + 1  // замена (заменяем S[i-1] на T[j-1])
                });

                cout << "  Не совпадают -> рассматриваем:\n";
                cout << "    Удаление (D): " << dp[i - 1][j] + 1 << "\n";
                cout << "    Вставка (I): " << dp[i][j - 1] + 1 << "\n";
                cout << "    Замена (R): " << dp[i - 1][j - 1] + 1 << "\n";

                char op;
                if (dp[i][j] == dp[i - 1][j - 1] + 1)
                    op = 'R';
                else if (dp[i][j] == dp[i - 1][j] + 1)
                    op = 'D';
                else
                    op = 'I';

                cout << "  Выбрана операция: " << op << ", стоимость: " << dp[i][j] << "\n";
            }
            cout << "-------------------------------------\n";
        }
    }

    printDPTable(dp, S, T);
}

int main() {
    string S, T;
    cin >> S >> T; 

    int m = S.size(); // длина строки S
    int n = T.size(); // длина строки T

    vector<vector<int>> dp(m + 1, vector<int>(n + 1)); // таблица для хранения расстояний
    initializeDP(dp, m, n);                            // инициализация таблицы
    fillDPTable(S, T, dp);                             // заполнение таблицы расстояний
    int lcsLength = computeLCSLength(S, T, m, n);      // вычисление длины наибольшей общей подпоследовательности

    cout << "Расстояние Левенштейна между \"" << S << "\" и \"" << T << "\" = " << dp[m][n] << endl;
    cout << "Длина наибольшей подпоследовательности: " << lcsLength << endl;

    return 0;
}
