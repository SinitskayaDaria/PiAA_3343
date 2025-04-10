#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

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

    // заполнение таблицы для хранения стоимости редактирования
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            
            // если текущие символы совпадают
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // стоимость остается прежней
            } else {

                // выбор минимальной стоимости из трех возможных операций
                dp[i][j] = min({
                    dp[i - 1][j - 1] + replace_cost, // стоимость замены символа
                    dp[i][j - 1] + insert_cost,      // стоимость вставки символа
                    dp[i - 1][j] + delete_cost       // стоимость удаления символа
                });
            }
        }
    }
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

    cout << dp[m][n] << endl;

    return 0; 
}
