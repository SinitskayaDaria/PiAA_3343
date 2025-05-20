#include <iostream> 
#include <vector> 
#include <climits> 
#include <iomanip> 
#include <fstream> 
#include <random> 
#include <algorithm> 
#include <set> 
#include <utility> 
#include <sstream> 
#include <map> 

using namespace std; 

using Path = vector<int>;   // определяем тип Path как вектор целых чисел для хранения пути
using Cost = double;        // определяем тип Cost как double для хранения стоимости

Path best_path;             // глобальная переменная для хранения лучшего найденного пути
Cost best_cost = INT_MAX;   // глобальная переменная для хранения лучшей найденной стоимости 

// функция генерации случайной симметричной матрицы
vector<vector<int>> generate_symmetric_matrix(int N, int min_w = 1, int max_w = 100) {
    random_device rd;                               // генератор случайных чисел
    mt19937 gen(rd());                              // инициализация генератора
    uniform_int_distribution<> dis(min_w, max_w);   // определение распределения для целых чисел
    vector<vector<int>> matrix(N, vector<int>(N));  // создание матрицы NxN

    // проход по строкам матрицы
    for (int i = 0; i < N; ++i) { 
        // проход по столбцам, начиная с диагонали
        for (int j = i + 1; j < N; ++j) { 
            int w = dis(gen);                   // генерация случайного значения веса
            matrix[i][j] = matrix[j][i] = w;    // заполнение симметрично
        }
        matrix[i][i] = -1; // расстояние из себя в себя -1
    }
    return matrix; 
}

// функция генерации случайной матрицы
vector<vector<int>> generate_matrix(int N, int min_w = 1, int max_w = 100) {
    random_device rd;                               // генератор случайных чисел
    mt19937 gen(rd());                              // инициализация генератора
    uniform_int_distribution<> dis(min_w, max_w);   // определение распределения для целых чисел
    vector<vector<int>> matrix(N, vector<int>(N));  // создание матрицы NxN

    // проход по строкам матрицы
    for (int i = 0; i < N; ++i) { 
        // проход по столбцам
        for (int j = 0; j < N; ++j) { 
            // проверка на элемент на главной диагонали матрицы
            if (i == j) {
                matrix[i][j] = -1; // расстояние из себя в себя -1
            } else  {
                int w = dis(gen);  // генерация случайного значения веса
                matrix[i][j] = w;  // заполнение матрицы
            }
        }
    }
    return matrix; 
}

// метод сохранения размера матрицы и матрицы в файл
void save_matrix(const vector<vector<int>>& dist, const string& filename) {
    ofstream fout(filename);    // открытие файла для записи
    int N = dist.size();        // получение размера матрицы
    fout << N << "\n";          // запись размера в файл

    // проход по каждой строке матрицы
    for (const auto& row : dist) { 
        for (int val : row) fout << val << " "; // запись значения строки
        fout << "\n";                           // переход на новую строку
    }
    fout.close(); // закрытие файла
}

// функция загрузки размера матрицы и матрицы из файла
vector<vector<int>> load_matrix(const string& filename) {
    ifstream fin(filename);                      // открытие файла для чтения
    int N;                                       // размер матрицы
    fin >> N;                                    // считывание размера матрицы
    vector<vector<int>> dist(N, vector<int>(N)); // инициализация матрицы
 
    // проход по строкам
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j){
            fin >> dist[i][j];  // считывание элемента матрицы
        } 
    }

    fin.close(); // закрытие файла
    return dist; 
}

// функция вывода пути
string print_path(const vector<int>& path) {
    stringstream ss; // создание строкового потока

    // прохож по всем элементам пути кроме последнего
    for (size_t i = 0; i < path.size() - 1; ++i) { 
        ss << path[i] << " "; // добавление элемента в поток
    }
    ss << path.back();  // добавление последнего элемента 
   
    return ss.str();  
}

// функция вычисления общей стоимости пути
double calculate_total_cost(const std::vector<int>& path, const std::vector<std::vector<int>>& cost_matrix) {
    double total_cost = 0.0; // стоимость
    int n = path.size();     // размер пути

    // проход по всем элементам пути
    for (int i = 0; i < n; ++i) { 
        total_cost += cost_matrix[path[i]][path[(i + 1) % n]]; // стоимость между текущим и следующим городом
    }

    return total_cost; 
}

// функция поиска с алгоритмом минимального разностного приближения
pair<vector<int>, double> amr_algorithm(const vector<vector<int>>& cost_matrix) {
    int n = cost_matrix.size();                         // количество городов
    vector<int> initial_path(n);                        // массив для начального пути
    iota(initial_path.begin(), initial_path.end(), 0);  // заполнение для начального пути по порядку от 0 до n-1

    vector<int> best_path = initial_path;                               // лучший путь
    double best_cost = calculate_total_cost(initial_path, cost_matrix); // подсчёт стоимости начального пути

    bool m = true;          // флаг улучшения пути
    int iterations = 0;     // счетчик итераций
    int F = n;              // максимальное количество итераций

    cout << "Начальный путь: " << print_path(best_path) << " со стоимостью: " << best_cost << endl; 

    while (m && iterations < F) {
        m = false; // сброс флага улучшения пути
        
        // проход по всем городам
        for (int i = 1; i < n; ++i) { 
            for (int j = 1; j < n; ++j) {
                vector<int> new_path = best_path;                               // сохранение текущего лучшего пути
                swap(new_path[i], new_path[j]);                                 // обмен местами двух городов
                double new_cost = calculate_total_cost(new_path, cost_matrix);  // подсчет новой стоимости
                
                // проверка на лучше ли новая стоимость 
                if (new_cost < best_cost) { 
                   
                    cout << "\tМеняем местами города " << best_path[i] << " и " << best_path[j] << endl;
                    cout << "\tБыло обнаружено лучшее решение " << print_path(new_path) << " со стоимостью " << new_cost << " (улучшение на " << (best_cost - new_cost) << ")" << endl;

                    best_path = new_path;   // обновление лучшего пути
                    best_cost = new_cost;   // обновление лучшей стоимости
                    m = true;               // установка флага лучшего пути
                    iterations++;           // увеличение счетчика итераций
                    break;                  // выход из внутреннего цикла
                }
            }
        }
    }

    cout << "Все города были посещены в порядке: " << print_path(best_path) << " Стоимость найденного пути: " << best_cost << endl; 

    return {best_path, best_cost}; 
}

// фуункция получения допустимых рёбер для добавления к текущему пути
vector<pair<int, int>> get_allowed_edges(const vector<int>& path, const set<int>& remaining_cities) {
    vector<pair<int, int>> allowed_edges; // массив для хранения допустимых рёбер
    int last_city = path.back();          // последний город из пути
    
    // проход по всем оставшимся городам
    for (int city : remaining_cities) { 
        allowed_edges.emplace_back(last_city, city); // добавление рёбра к последнему городу
    }

    return allowed_edges; 
}

// функция поиска корня в структуре непересекающихся множеств
int find(int u, map<int, int>& parent) {
    // пока узел не является корнем
    while (parent[u] != u) { 
        parent[u] = parent[parent[u]]; // сокращаем путь (родитель текущего узла сразу ссылается на корень)
        u = parent[u];                 // переход к родителю
    }
    return u; 
}

// функция вычисления веса минимального остовного дерева 
int calculate_mst(const vector<vector<int>>& cost_matrix, const vector<int>& path, const set<int>& remaining_cities) {
    vector<vector<int>> chunks;             // массив для хранения частей пути
    chunks.push_back(path);                 // добавление текущего пути как первой части
    for (int city : remaining_cities) {
        chunks.push_back({city});           // добавление оставшиеся городов как отдельных частей
    }

    cout << "\tОценка оставшегося пути с помощью МОД для оставшихся кусков:\n\t";
    
    // проход по частям
    for (size_t i = 0; i < chunks.size(); ++i) {
        // проход по элементам в части 
        for (size_t j = 0; j < chunks[i].size(); ++j) { 
            cout << chunks[i][j] << " ";               // вывод элементов
        }
        if (i + 1 < chunks.size()) cout << "| ";       // запись разделителя между частями
    }
    cout << endl;

    cout << "\tВсе доступные рёбра:\n"; 

    vector<tuple<int, int, int>> edges; // массив для хранения рёбер

    // проход по частям
    for (size_t i = 0; i < chunks.size(); ++i) { 
        // проход по частям для соединения
        for (size_t j = 0; j < chunks.size(); ++j) { 
            // пропуск самосоединения
            if (i != j) { 
                int start = chunks[i].back();       // последний город из первой части
                int end = chunks[j].front();        // первый город из второй части
                int cost = cost_matrix[start][end]; // стоимость между городами

                // проверка, что путь допустим
                if (cost != -1) {
                    
                    cout << "\t\t" << start << " -> " << end << " стоимость = " << cost << endl;
                    
                    edges.emplace_back(cost, start, end); // добавление ребра в массив для хранения рёбер
                }
            }
        }
    }

    sort(edges.begin(), edges.end()); // сортировка рёбер по весу

    // инициализация родителей для поиска
    map<int, int> parent; // ассоциативный массив для хранения родителя каждого узла
    // проход по частям
    for (const auto& chunk : chunks) {
        // проход по городам в каждой части
        for (int city : chunk) {
            parent[city] = city; // установка себя как родителя
        }
    }

    int mst_weight = 0; // вес минимального остовного дерева
    // проход по всем рёбрам
    for (const auto& [cost, u, v] : edges) { 
        int root_u = find(u, parent); // нахождение кореня для первой точки
        int root_v = find(v, parent); // нахождение кореня для второй точки
        // проверка, если корни разные
        if (root_u != root_v) { 
            
            cout << "\tДобавление к каркасу ребра " << u << " -> " << v << " со стоимостью " << cost << endl; 
            
            mst_weight += cost;         // увеличение веса минимального остова
            parent[root_v] = root_u;    // объединение двух множеств
        }
    }

    return mst_weight; 
}

// функция вычисления полусуммы весов двух рёбер
double calculate_half_sum(const vector<vector<int>>& cost_matrix, const vector<int>& path, const set<int>& remaining_cities) {
    vector<vector<int>> chunks; // массив для хранения частей пути
    chunks.push_back(path);     // добавление текущего пути
    // добавление оставшихся городов как отдельных частей
    for (int city : remaining_cities) {
        chunks.push_back({city}); 
    }

    double half_sum = 0; // полусумма

    cout << "\n\tОценка оставшегося пути с помощью полусуммы весов двух легчайших рёбер по всем кускам:\n\t";
    
    // проход по частям
    for (size_t i = 0; i < chunks.size(); ++i) { 
        // проход по элементам в части
        for (size_t j = 0; j < chunks[i].size(); ++j) { 
            cout << chunks[i][j]<< " ";           // вывод элемента
        }
        if (i + 1 < chunks.size()) cout << "| "; // разделитель между частями
    }
    cout <<endl;
    
    // проход по частям
    for (const auto& chunk : chunks) { 
        vector<int> incoming_edges; // массив для хранения входящих рёбер
        // проход по всем частям для поиска входящих рёбер
        for (const auto& other_chunk : chunks) { 
            if (other_chunk != chunk) {
                int start = other_chunk.back();     // последний город из другой части
                int end = chunk.front();            // первый город
                int cost = cost_matrix[start][end]; // стоимость
                if (cost != -1) {
                    incoming_edges.push_back(cost); // добавление стоимости во входящие рёбра
                }
            }
        }
        int min_incoming = incoming_edges.empty() ? 0 : *min_element(incoming_edges.begin(), incoming_edges.end()); // нахождение минимального входящего ребра

        vector<int> outgoing_edges; // массив для хранения исходящих рёбер
        // проход по частям
        for (const auto& other_chunk : chunks) { 
            if (other_chunk != chunk) {
                int start = chunk.back();           // последний город в текущей части
                int end = other_chunk.front();      // первый город в другой части
                int cost = cost_matrix[start][end]; // стоимость
                if (cost != -1) {
                    outgoing_edges.push_back(cost); // добавление стоимости в исходящие рёбра
                }
            }
        }
        int min_outgoing = outgoing_edges.empty() ? 0 : *min_element(outgoing_edges.begin(), outgoing_edges.end()); // нахождение минимального исходящего ребра

        cout << "\tРассматриваем кусок {";
        for (size_t i = 0; i < chunk.size(); ++i) { 
            cout << chunk[i];
            if (i + 1 < chunk.size()) cout << " "; 
        }
        cout << "} Минимальное входящее ребро = " << min_incoming << ", исходящее = " << min_outgoing << endl; 

        half_sum += (min_incoming + min_outgoing) / 2.0; // оценка полусуммы
    }
    
    return half_sum; 
}

// функция вычисления нижней границы стоимости
double calculate_lower_bound(const vector<int>& path, const set<int>& remaining_cities, const vector<vector<int>>& cost_matrix) {
    double mst_estimate = calculate_mst(cost_matrix, path, remaining_cities);           // получение оценки минимального остовного дерева
    double half_sum_estimate = calculate_half_sum(cost_matrix, path, remaining_cities); // получение оценки полусуммы

    cout << "\tДля оставшегося пути вес минимального каркаса = " << mst_estimate << ", минимальная полусумма = " << half_sum_estimate 
         << "\n\t=> Берем максимальную из двух оценок = " << max(mst_estimate, half_sum_estimate) << endl;
    
    return max(mst_estimate, half_sum_estimate); 
}

// функция реализации метода ветвей и границ
pair<vector<int>, double> branch_and_bound(const vector<vector<int>>& cost_matrix) {
    int n = cost_matrix.size();                            // количество городов
    vector<int> best_path;                                 // массив для хранения лучшего пути
    double best_cost = numeric_limits<double>::infinity(); // лучшая стоимость

    // рекурсивная функция 
    function<void(vector<int>, double, set<int>)> backtrack = [&](vector<int> path, double current_cost, set<int> remaining) {
        // проверка, если все города посещены
        if (remaining.empty()) { 
            double total_cost = current_cost + cost_matrix[path.back()][path[0]]; // рассчет полной стоимости цикла
            
            cout << "Все города были посещены в порядке: " << print_path(path) << " Стоимость: " << total_cost << "\n-------------------------\n";
            
            // проверка, если новое решение лучше
            if (total_cost < best_cost) {
                best_cost = total_cost; // обновление лучшей стоимости
                best_path = path;       // обновление лучшего пути
            }
            return; // выход из функции
        }

        vector<pair<int, int>> edges = get_allowed_edges(path, remaining);  // получение допустимых рёбер для текущего пути

        // проход по каждому допустимому ребру
        for (const auto& [u, v] : edges) {
            double edge_cost = cost_matrix[u][v]; // стоимость ребра
            if (edge_cost < 0) continue;          // пропуск недопустимых рёбер

            set<int> new_remaining = remaining; // сохранение оставшихся городов
            new_remaining.erase(v);             // удаление текущего города из оставшихся
            
            vector<int> new_path = path; // сохрание текущего пути
            new_path.push_back(v);       // добавление текущего города
            
            double lower_bound = calculate_lower_bound(new_path, new_remaining, cost_matrix); // вычисление нижней границы
            double total_estimate = current_cost + edge_cost + lower_bound;                   // оценка стоимости

            // проверка, если оценка меньше лучшей стоимости
            if (total_estimate < best_cost) { 
                backtrack(new_path, current_cost + edge_cost, new_remaining); // рекурсивный вызов
            } else {
                cout << "\t=> Отсечение ветки для города " << v << " (оценка " << total_estimate << " >= " << best_cost << ")\n"; 
            }
        }
    };

    // инициализация: начинаем с города 0
    set<int> initial_remaining; // множество для хранения оставшихся городов
    // добавление всех городов кроме первого
    for (int i = 1; i < n; ++i) {
        initial_remaining.insert(i); 
    }
    
    backtrack({0}, 0.0, initial_remaining); // запуск рекурсии с первого города

    return {best_path, best_cost}; 
}

int main() {
    int N;                    // количество городов
    vector<vector<int>> dist; // матрица расстояний между городами
    int choice;               // выбор способа задания матрицы

    cout << "Выберите способ задания матрицы весов:\n";
    cout << "1 - Ввести вручную\n";
    cout << "2 - Сгенерировать случайную матрицу\n";
    cout << "3 - Сгенерировать случайную симметричную матрицу\n";
    cout << "4 - Загрузить матрицу из файла\n";
    cout << "Ваш выбор: ";
    cin >> choice; 

    switch (choice) {
        case 1: { // ввод вручную
            cout << "Введите количество городов: ";
            cin >> N; 
            dist.assign(N, vector<int>(N)); 
            cout << "Введите матрицу стоимостей (" << N << "x" << N << "):\n";
            for (int i = 0; i < N; ++i) 
                for (int j = 0; j < N; ++j) 
                    cin >> dist[i][j]; 
            save_matrix(dist, "manual_input_matrix.txt"); 
            cout << "Матрица сохранена в manual_input_matrix.txt\n"; 
            break;
        }

        case 2: { // генерация случайной матрицы
            cout << "Введите количество городов: ";
            cin >> N; 
            dist = generate_matrix(N); 
            save_matrix(dist, "matrix.txt"); 
            cout << "Произвольная матрица сгенерирована и сохранена в matrix.txt\n"; 
            break;
        }

        case 3: { // генерация симметричной матрицы
            cout << "Введите количество городов: ";
            cin >> N;
            dist = generate_symmetric_matrix(N); 
            save_matrix(dist, "generated_symmetric_matrix.txt"); 
            cout << "Симметричная матрица сгенерирована и сохранена в generated_symmetric_matrix.txt\n"; 
            break;
        }

        case 4: { // загрузка матрицы из файла
            string filename; 
            cout << "Введите имя файла: ";
            cin >> filename; 
            
            ifstream fin(filename);
            if (!fin) { 
                cerr << "Ошибка открытия файла.\n";
                return 1;
            }

            fin >> N; 
            dist = load_matrix(filename); 
            if (dist.empty()) { 
                cerr << "Ошибка загрузки файла.\n";
                return 1; 
            }
            break; 
        }

        default:
            cerr << "Неверный выбор.\n"; 
            return 1; 
    }

    vector<int> best_path_mvg; // лучший путь метода ветвей и границ
    double best_cost_mvg;      // лучшая стоимость метода ветвей и границ

    vector<int> best_path_amr; // лучший путь алгоритма минимального разностного приближения
    double best_cost_amr;      // лучшая стоимость алгоритма минимального разностного приближения

    cout<<"Метод Ветвей и Границ:"<<endl;
    auto result_mvg = branch_and_bound(dist);   // запуск метода ветвей и границ
    best_path_mvg = result_mvg.first;           // получение лучший путь
    best_cost_mvg = result_mvg.second;          // получение лучшую стоимость

    cout<<"Алгоритм Минимального Разностного приближения:"<<endl;
    auto result_amr = amr_algorithm(dist);      // запуск алгоритма АМР
    best_path_amr = result_amr.first;           // получение лучший путь
    best_cost_amr = result_amr.second;          // получение лучшую стоимость
    

    cout << endl << "====Лучший результат====" << endl;
    cout << endl << "--Метод Ветвей и Границ--" << endl;
    cout << "Города были посещены в порядке: " << print_path(best_path_mvg)<< endl; 
    cout << "Стоимость найденного пути: " << fixed << setprecision(1) << best_cost_mvg <<endl; 

    cout << endl << "--Алгоритм Минимального Разностного приближения--" << endl;

    cout << "Города были посещены в порядке: " << print_path(best_path_amr)<< endl; 
    cout << "Стоимость найденного пути: " << fixed << setprecision(1) << best_cost_amr <<endl; 

    return 0;
}
