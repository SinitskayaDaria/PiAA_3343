#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// метод печати таблицы расстояний между 
void printDPTable(const vector<vector<int>>& dp, const string& A, const string& B, int m, int n) {

    cout << "\nТаблица расстояний (dp):\n";
    cout << setw(6) << " "; 
    cout << setw(6) << "' '"; 

    // заголовок столбцов для строки B
    for (char ch : B)
        cout << setw(6) << ch; // печать каждого символа строки B с отступом
    cout << endl;

    // строки для каждого символа строки A
    for (int i = 0; i <= m; ++i) {
        // если это первая строка 
        if (i == 0)
            cout << setw(6) << "' '"; // пустое значение
        else
            cout << setw(6) << A[i - 1]; // символ строки A с отступом

        // все значения таблицы для текущей строки
        for (int j = 0; j <= n; ++j) {
            cout << setw(6) << dp[i][j]; // текущая ячейка с отступом
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

    cout << "\nПосле инициализации базовых случаев:\n";
    printDPTable(dp, "", "", m, n);
}

// метод вычисления редакционного расстояния между строками A и B
void computeEditDistance(vector<vector<int>>& dp, const string& A, const string& B, int replace_cost, int insert_cost, int delete_cost, int m, int n) {

    // заполнение таблицы для хранения стоимости редактирования
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            
            // если текущие символы совпадают
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // стоимость остается прежней

                cout << "dp[" << i << "][" << j << "] = dp[" << i-1 << "][" << j-1 << "] (символы совпадают '" << A[i-1] << "') = " << dp[i][j] << "\n";
            
            } else {

                // выбор минимальной стоимости из трех возможных операций
                dp[i][j] = min({
                    dp[i - 1][j - 1] + replace_cost, // стоимость замены символа
                    dp[i][j - 1] + insert_cost,      // стоимость вставки символа
                    dp[i - 1][j] + delete_cost       // стоимость удаления символа
                });

                cout << "dp[" << i << "][" << j << "] = min(замена: " << dp[i - 1][j - 1] + replace_cost
                << ", вставка: " << dp[i][j - 1] + insert_cost
                << ", удаление: " << dp[i - 1][j] + delete_cost 
                << ") = " << dp[i][j] << "\n";
            }
        }
    }

    cout << "\nФинальная таблица расстояний:\n";
    printDPTable(dp, A, B, m, n);
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

    cout << "\nМинимальная стоимость преобразования: " << dp[m][n] << endl;

    return 0; 
}
