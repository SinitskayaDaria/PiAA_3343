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

            cout << "Сравниваем A[" << i - 1 << "] = " << A[i - 1] << " и B[" << j - 1 << "] = " << B[j - 1] << "\n";
            
            // если символы совпадают, цена остается прежней
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];    // операция не нужна
                op[i][j] = 'M';                 // запоминаем операцию 'M' (совпадение)

                cout << "  Совпадают -> операция: M (match), стоимость: " << dp[i][j] << "\n";
            
            } else {

                // рассчет стоимости операций
                int r = dp[i - 1][j - 1] + cost_replace; // замена символа
                int d = dp[i - 1][j] + cost_delete;      // удаление символа из A
                int ins = dp[i][j - 1] + cost_insert;    // вставка символа в A

                dp[i][j] = r;   // предполагаем, что замена - наименьшая стоимость
                op[i][j] = 'R'; // запоминаем операцию замены

                cout << "  Не совпадают -> рассматриваем:\n";
                cout << "    Замена (R): " << r << "\n";
                cout << "    Удаление (D): " << d << "\n";
                cout << "    Вставка (I): " << ins << "\n";

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

                cout << "  Выбрана операция: " << op[i][j] << ", стоимость: " << dp[i][j] << "\n";
            }
            cout << "-------------------------------------\n";
        }
    }
}

// вывод таблицы DP и операций
void printTables(const vector<vector<int>>& dp, const vector<vector<char>>& op, const string& A, const string& B) {
    int m = A.size(), n = B.size();

    cout << "\nТаблица стоимости операций (dp):\n    ";
    for (int j = 0; j <= n; ++j)
        cout << setw(4) << (j == 0 ? '-' : B[j - 1]);
    cout << "\n";

    for (int i = 0; i <= m; ++i) {
        cout << (i == 0 ? '-' : A[i - 1]) << " ";
        for (int j = 0; j <= n; ++j)
            cout << setw(4) << dp[i][j];
        cout << "\n";
    }

    cout << "\nТаблица операций (op):\n    ";
    for (int j = 0; j <= n; ++j)
        cout << setw(4) << (j == 0 ? '-' : B[j - 1]);
    cout << "\n";

    for (int i = 0; i <= m; ++i) {
        cout << (i == 0 ? '-' : A[i - 1]) << " ";
        for (int j = 0; j <= n; ++j)
            cout << setw(4) << (op[i][j] ? op[i][j] : ' ');
        cout << "\n";
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
    int lcsLength = computeLCSLength(A, B, m, n);                       // вычисление длины наибольшей общей подпоследовательности

    cout << "\nПоследовательность операций: " << operations << "\n";
    cout << "Исходная строка A: " << A << "\n";
    cout << "Целевая строка B: " << B << "\n";
    cout << "Длина наибольшей подпоследовательности: " << lcsLength << endl;

    return 0;
}
