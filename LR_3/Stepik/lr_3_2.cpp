#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

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
            
            // если текущие символы совпадают, переносим значение диагонально
            if (S[i - 1] == T[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; 
            } else {
                
                // берем минимум из трех операций: удаление, вставка, замена
                dp[i][j] = min({
                    dp[i - 1][j] + 1,     // удаление (S[i-1] не учитываем)
                    dp[i][j - 1] + 1,     // вставка (добавляем T[j-1] к S)
                    dp[i - 1][j - 1] + 1  // замена (заменяем S[i-1] на T[j-1])
                });
            }
        }
    }
}

int main() {
    string S, T;
    cin >> S >> T; 

    int m = S.size(); // длина строки S
    int n = T.size(); // длина строки T

    vector<vector<int>> dp(m + 1, vector<int>(n + 1)); // таблица для хранения расстояний
    initializeDP(dp, m, n);                            // инициализация таблицы
    fillDPTable(S, T, dp);                             // заполнение таблицы расстояний

    cout << dp[m][n] << endl;

    return 0; 
}
