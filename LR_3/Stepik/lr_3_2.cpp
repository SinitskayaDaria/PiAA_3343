#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// метод инициализации таблицы
void initializeDP(vector<vector<int>>& dp, vector<vector<char>>& op, int m, int n, int cost_delete, int cost_insert) {
    // инициализация первой колонки (строка A)
    for (int i = 0; i <= m; ++i) {
        dp[i][0] = i * cost_delete;  // стоимость удаления i символов из A
        if (i > 0) op[i][0] = 'D';   // запоминаем операцию 'D' (удаление)
    }
    // инициализация первой строки (строка B)
    for (int j = 0; j <= n; ++j) {
        dp[0][j] = j * cost_insert;  // стоимость вставки j символов в A
        if (j > 0) op[0][j] = 'I';   // запоминаем операцию 'I' (вставка)
    }
}

// метод заполнения таблицы 
void fillDPTable(const string& A, const string& B, int cost_replace, int cost_insert, int cost_delete, vector<vector<int>>& dp, vector<vector<char>>& op) {
    
    int m = A.size(), n = B.size();  // размеры строк А, В
    
    // проход по всем символам строк A и B
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            // если символы совпадают, цена остается прежней
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];    // операция не нужна
                op[i][j] = 'M';                 // запоминаем операцию 'M' (совпадение)
            } else {
                // рассчет стоимости операций
                int r = dp[i - 1][j - 1] + cost_replace; // замена символа
                int d = dp[i - 1][j] + cost_delete;      // удаление символа из A
                int ins = dp[i][j - 1] + cost_insert;    // вставка символа в A

                dp[i][j] = r;   // предполагаем, что замена - наименьшая стоимость
                op[i][j] = 'R'; // запоминаем операцию замены

                // если удаление дешевле, обновляем стоимость и операцию
                if (d < dp[i][j]) {
                    dp[i][j] = d;
                    op[i][j] = 'D'; // запоминаем операцию 'D' (удаление)
                }
                // если вставка дешевле, обновляем стоимость и операцию
                if (ins < dp[i][j]) {
                    dp[i][j] = ins;
                    op[i][j] = 'I'; // запоминаем операцию 'I' (вставка)
                }
            }
        }
    }
}

// функция восстановления последовательности операций
string reconstructOperations(const vector<vector<char>>& op, int m, int n) {
    string operations;  // строка для хранения последовательности операций
    int i = m, j = n;

    // восстанавление операций (начиная с конца)
    while (i > 0 || j > 0) {
        char action = op[i][j]; // получение текущей операции
        operations += action;   // добавление операции в результат

        // переход в предыдущее состояние в зависимости от операции
        switch (action) {
            case 'M':   // совпадение
            case 'R':   // замена
                --i;                 // переход к предыдущему символу в A
                --j;                 // переход к предыдущему символу в B
                break;
            case 'D':   // удаление
                --i;                 // переход к предыдущему символу в A
                break;
            case 'I':   // вставка
                --j;                 // переход к предыдущему символу в B
                break;
        }
    }

    // реверс операций, чтобы получить правильный порядок
    reverse(operations.begin(), operations.end());

    return operations; 
}

int main() {

    int cost_replace, cost_insert, cost_delete;          // стоимости операций замены, вставки и удаления
    cin >> cost_replace >> cost_insert >> cost_delete;

    string A, B;        // строки А, В
    cin >> A >> B;

    int m = A.size(), n = B.size();  // длины строк А, В

    // создание таблиц для хранения стоимости и действий
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    vector<vector<char>> op(m + 1, vector<char>(n + 1));

    initializeDP(dp, op, m, n, cost_delete, cost_insert);               // инициализация таблицы
    fillDPTable(A, B, cost_replace, cost_insert, cost_delete, dp, op);  // заполнение таблицы
    string operations = reconstructOperations(op, m, n);                // восстановление последовательности операций

    cout << operations << endl;
    cout << A << endl;
    cout << B << endl;

    return 0; 
}
