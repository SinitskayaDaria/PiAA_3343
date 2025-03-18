#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// структура для квадратного обрезка доски
struct Square {
    int x, y, w;                    // x, y - координаты верхнего левого угла, w - ширина
    Square() : x(0), y(0), w(0) {} 
    Square(int x, int y, int w) : x(x), y(y), w(w) {} 
};

// структура для состояния столешницы
struct State {
    int grid[20];       // сетка, представляющая размещенные квадраты
    int count;          // количество размещенных квадратов
    int x, y;           // позиция для размещения следующего квадрата
    int next_w;         // следующий размер квадрата для размещения
    int sum_areas;      // сумма площадей размещенных квадратов
    int current_size;   // текущий размер пути
    Square path[50];    // последовательность размещенных квадратов
};


State stack[1000];  // стек для хранения состояния
int stack_ptr = -1; // указатель на верхушку стека

// метод для добавления состояния в стек
void push_state(const State& s) {
    if (stack_ptr < 999) stack[++stack_ptr] = s; // увелечение указателя и добавление состояния в стек
}

// функция для извлечения состояния из стека
State pop_state() {
    return stack[stack_ptr--]; // возврат верхнего состояния из стека и уменьшение указателя
}

// метод инициализации начального состояния
void initialize_initial_state(State& state, int N) {
    state.count = 0;                                        // количество квадратов 
    state.sum_areas = 0;                                    // сумма площадей 
    state.x = -1;                                           // координаты для следующего размещения
    state.y = -1; 
    state.next_w = 0;                                       // следующий размер квадрата
    state.current_size = 0;                                 // текущий размер пути
    for (int i = 0; i < N; i++) state.grid[i] = 0;          // инициализизация сетки нулями
    for (int i = 0; i < 50; i++) state.path[i] = {0, 0, 0}; // последовательность размещенных квадратов
}

// функция проверки на полное покрытие столешницы
bool is_fully_covered(const State& s, int N) {
    const int full_row = (1 << N) - 1;  // строка, где все клетки заняты
    for (int y = 0; y < N; y++)
        if (s.grid[y] != full_row) return false; 
    return true; 
}

// метод поиска первой свободной клетки для размещения
void find_first_free_cell(const State& s, int N, int& x, int& y) {
    x = y = -1; // координаты
    for (y = 0; y < N; y++) {
        if (s.grid[y] == ((1 << N) - 1)) continue; // пропуск заполненных строк
        for (x = 0; x < N; x++) {
            if (!(s.grid[y] & (1 << (N - 1 - x)))) { // поиск первой свободной клетки
                break;
            }
        }
        if (x < N) break; // нашли свободную клетку, выходим
    }
}

// метод обработки состояния, когда столешница полностью заполнена
void handle_full_coverage(const State& s, int N, int& best, Square* best_squares, int& best_size) {
    if (s.sum_areas == N * N && s.count < best) {            
        best = s.count;                                      // обновление лучшего количества квадратов
        best_size = s.current_size;                          // обновление размера лучшего пути
        copy(s.path, s.path + s.current_size, best_squares); // копирование лучшего пути
    }
}

// метод для обработки состояния
void process_state(State& s, int N, int& best, Square* best_squares, int& best_size) {
    if (s.count >= best) return; // текущее количество квадратов больше или равно лучшему - выход

    if (is_fully_covered(s, N)) {
        handle_full_coverage(s, N, best, best_squares, best_size); // проверка и обрабатка полного покрытия
        return;
    }

    int x, y;
    if (s.x == -1) find_first_free_cell(s, N, x, y);    // нахождение первой свободной клетки
    else { x = s.x; y = s.y; }                          // сохраненные координаты

    const int max_w = min({N - x, N - y, N - 1});       // поиск максимального размера квадрата для размещения
    if (max_w <= 0) return;                             // нет возможности разместить квадрат, выход

    // перебор возможных размеров квадратов для размещения
    for (int w = s.next_w == 0 ? max_w : s.next_w; w >= 1; w--) {
        const int mask = ((1U << w) - 1) << (N - x - w); // маска для размещения квадрата
        bool can_place = true;                           // флаг для проверки возможности размещения

        // проверка возможности размещения квадрата
        for (int dy = 0; dy < w; dy++) {
            const int cy = y + dy;
            if (cy >= N || (s.grid[cy] & mask)) {
                can_place = false; // размещение не возможно
                break;
            }
        }
        if (!can_place) continue; // нельзя разместить, переход к следующему размеру

        const int remaining = N*N - (s.sum_areas + w*w);                                                   // остаток площади
        const int max_possible = min(max_w, static_cast<int>(sqrt(remaining)) + 1);                        // максимально возможный размер
        const int lower_bound = (remaining + max_possible*max_possible - 1) / (max_possible*max_possible); // нижняя граница
        if (s.count + 1 + lower_bound >= best) continue; // текущее количество квадратов вместе с нижней границей больше или равно лучшему, выходим

        State new_state = s;            // создание нового состояния
        new_state.count++;              // увеличение количества квадратов
        new_state.sum_areas += w*w;     // обновление суммы площадей
        new_state.x = -1;               // сброс координат
        new_state.y = -1;
        new_state.next_w = 0;           // сброс следующего размера квадрата

        // обновление сетки
        for (int dy = 0; dy < w; dy++)
            new_state.grid[y + dy] |= mask;

        // добавление в путь информации о квадрате
        new_state.path[new_state.current_size++] = {x + 1, y + 1, w}; 

        // сохранение текущего размера для следующей итерации
        if (w > 1) {
            State continue_state = s;       // сохранение текущеего состояния
            continue_state.x = x;           // сохранение координат
            continue_state.y = y;
            continue_state.next_w = w - 1;  // уменьшение размера на 1
            push_state(continue_state);     // добавление состояния в стек
        }

        push_state(new_state); // добавление нового состояния в стек
        break;                 // обрабатка по одному размеру за шаг
    }
}

// метод вывода результатов
void print_result(int best, const Square* best_squares, int best_size) {
    cout << best << endl;                                   
    for (int i = 0; i < best_size; i++) {
        const auto& sq = best_squares[i];
        cout << sq.x << " " << sq.y << " " << sq.w << endl; 
    }
}

int main() {
    int N;    // размер столешницы
    cin >> N; 

    // проверка на корректность входных данных
    if (N < 2 || N > 20) {
        cout << "Некорректные данные. N должно быть в диапазоне от 2 до 20." << endl;
        
    } else {
        int best = 1000;            // лучшее количество квадратов
        Square best_squares[50];    // массив лучших квадратов
        int best_size = 0;          // размер лучшего пути

        State initial;                        // начальное состояние
        initialize_initial_state(initial, N); // инициализация начального состояния
        push_state(initial);                  // добавление начального состояния в стек

        // обработка состояний
        while (stack_ptr >= 0) {
            State s = pop_state();                              // извлечение состояния из стека
            process_state(s, N, best, best_squares, best_size); // обработка состояния
        }

        print_result(best, best_squares, best_size); // вывод результатов
    }

    return 0;
}