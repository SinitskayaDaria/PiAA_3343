#include <iostream>
#include <vector>
#include <climits>
#include <iomanip> 

using namespace std;

// Тип представления пути
using Path = vector<int>;
using Cost = double;

// Глобальные переменные хранения лучшего пути и стоимости
Path best_path;
Cost best_cost = INT_MAX;

// Функция вычисления нижней границы для текущего пути
Cost lower_bound(const Path& path, const vector<vector<int>>& dist, int N) {
    Cost lb = 0;
    vector<bool> visited(N, false);

    // Суммируем стоимость текущего пути
    for (int i = 0; i < path.size() - 1; ++i) {
        lb += dist[path[i]][path[i + 1]];
        visited[path[i]] = true;
    }

    // Добавляем минимальные стоимости не посещенных городов
    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            int min_edge = INT_MAX;
            for (int j = 0; j < N; ++j) {
                if (!visited[j] && dist[i][j] < min_edge) {
                    min_edge = dist[i][j];
                }
            }
            lb += min_edge;
        }
    }
    return lb;
}

// Функция обновления лучшего пути
void update_best_solution(const Path& path, Cost cost) {
    if (cost < best_cost) {
        best_cost = cost;
        best_path = path;
    }
}

// Функция ветвей и границ
void branch_bound(int current_city, Path path, vector<bool>& visited, Cost current_cost, 
                  const vector<vector<int>>& dist, int N) {

    int city_count = path.size();

    // Если все города посещены, возвращаемся в стартовый
    if (city_count == N) {
        current_cost += dist[path[city_count - 1]][path[0]];  // Стоимость возвращения в стартовый город

        // Обновляем лучший путь
        update_best_solution(path, current_cost);
        return;
    }

    // Получаем нижнюю границу для текущего пути
    Cost lb = lower_bound(path, dist, N);
    if (current_cost + lb >= INT_MAX) {
        return; // Отсекаем пути, которые не могут привести к лучшему решению
    }

    // Продолжаем путь по всем возможным городам
    for (int next_city = 0; next_city < N; ++next_city) {
        if (!visited[next_city]) {
            path.push_back(next_city);
            visited[next_city] = true;

            branch_bound(next_city, path, visited, current_cost + dist[path[city_count - 1]][next_city],
                          dist, N);

            visited[next_city] = false;
            path.pop_back();
        }
    }
}

int main() {
    int N;
    cin >> N;

    vector<vector<int>> dist(N, vector<int>(N));

    // Чтение матрицы стоимостей
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> dist[i][j];
        }
    }

    // Начинаем с города 0
    Path initial_path = {0};
    vector<bool> visited(N, false);
    visited[0] = true;

    // Ищем оптимальный маршрут
    branch_bound(0, initial_path, visited, 0.0, dist, N);

    // Выводим оптимальный маршрут
    for (int i = 0; i < best_path.size(); ++i) {
        cout << best_path[i];
        if (i != best_path.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;

    // Выводим суммарную ману с точностью до 1 знака после запятой
    cout << fixed << setprecision(1) << best_cost << endl;

    return 0;
}
