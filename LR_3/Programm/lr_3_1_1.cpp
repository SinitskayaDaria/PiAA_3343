#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// метод печати таблицы dp
void printDPTable(const vector<vector<int>>& dp, const string& A, const string& B, int m, int n) {
    
    cout << "\nТаблица стоимости редактирования:\n\n";
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
            cout << setw(2) << dp[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

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

// метод инициализации базовых случаев для динамической таблицы
void initializeBaseCases(vector<vector<int>>& dp, int m, int n, int delete_cost, int insert_cost) {
    // заполнение первой колонки (число операций удаления символов)
    for (int i = 0; i <= m; ++i)
        dp[i][0] = i * delete_cost; // стоимость удаления i символов

    // заполнение первой (число операций вставки символов)
    for (int j = 0; j <= n; ++j)
        dp[0][j] = j * insert_cost; // стоимость вставки j символов
}

// метод вычисления редакционного расстояния между строками A и B
void computeEditDistance(vector<vector<int>>& dp, const string& A, const string& B, int replace_cost, int insert_cost, int delete_cost, int m, int n) {

    // применяем правило треугольника
    replace_cost = min(replace_cost, delete_cost + insert_cost);

    cout << "\nНачинаем вычисление редакционного расстояния:\n";

    // заполнение таблицы для хранения стоимости редактирования
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {

            // если текущие символы совпадают
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // стоимость остается прежней

                cout << "dp[" << i << "][" << j << "] (" << A[i - 1] << " == " << B[j - 1] << ") = dp[" << (i - 1) << "][" << (j - 1) << "] = " << dp[i][j] << endl;
            
            } else {

                // выбор минимальной стоимости из трех возможных операций
                dp[i][j] = min({
                    dp[i - 1][j - 1] + replace_cost, // стоимость замены символа
                    dp[i][j - 1] + insert_cost,      // стоимость вставки символа
                    dp[i - 1][j] + delete_cost       // стоимость удаления символа
                });

                cout << "dp[" << i << "][" << j << "] (replace " << A[i - 1] << " -> " << B[j - 1]
                     << ") = min(replace=" << dp[i - 1][j - 1] + replace_cost
                     << ", insert=" << dp[i][j - 1] + insert_cost
                     << ", delete=" << dp[i - 1][j] + delete_cost
                     << ") = " << dp[i][j] << endl;
            }
        }
    }

    printDPTable(dp, A, B, m, n);
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


int main() {
    
    int replace_cost, insert_cost, delete_cost;         // стоимости операций замены, вставки и удаления
    cin >> replace_cost >> insert_cost >> delete_cost;

    string A, B;    // строки А, В
    cin >> A >> B;

    int m = A.size();                                   // длина строки А
    int n = B.size();                                   // длина строки В
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));  // инициализация таблицы для хранения стоимости редактирования

    initializeBaseCases(dp, m, n, delete_cost, insert_cost);                        // инициализация базовых случаев
    computeEditDistance(dp, A, B, replace_cost, insert_cost, delete_cost, m, n);    // вычисление расстояния редактирования
    int lcsLength = computeLCSLength(A, B, m, n);                                   // вычисление длины наибольшей общей подпоследовательности
    
    cout << "\nРедакционное расстояние: " << dp[m][n] << endl;
    cout << "\nДлина наибольшей подпоследовательности: " << lcsLength << endl;

    return 0;
}
