#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Функция для вывода доски
void printBoard(const vector<vector<int>>& board, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << board[i][j];
            if (j < m - 1) cout << " ";
        }
        cout << endl;
    }
}

// Функция для подсчета ходов (без вывода шагов)
int countMovesToHalf(const vector<vector<int>>& board, int n, int m, 
                     int topValue, int bottomValue) {
    int moves = 0;
    int midRow = n / 2;
    vector<vector<int>> temp = board;
    
    // Обрабатываем верхнюю половину
    for (int i = 0; i < midRow; i++) {
        for (int j = 0; j < m; j++) {
            if (temp[i][j] != topValue) {
                int dx[] = {0, 1, 0, -1};
                int dy[] = {1, 0, -1, 0};
                
                for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        temp[i][j] = 1 - temp[i][j];
                        temp[ni][nj] = 1 - temp[ni][nj];
                        moves++;
                        break;
                    }
                }
            }
        }
    }
    
    // Обрабатываем нижнюю половину
    for (int i = midRow; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (temp[i][j] != bottomValue) {
                int dx[] = {0, 1, 0, -1};
                int dy[] = {1, 0, -1, 0};
                
                for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        temp[i][j] = 1 - temp[i][j];
                        temp[ni][nj] = 1 - temp[ni][nj];
                        moves++;
                        break;
                    }
                }
            }
        }
    }
    
    return moves;
}

// Функция для выполнения решения с выводом шагов
void solveWithSteps(const vector<vector<int>>& board, int n, int m, 
                    int topValue, int bottomValue) {
    int moves = 0;
    int midRow = n / 2;
    vector<vector<int>> temp = board;
    
    cout << "\nЦелевая конфигурация: верх=" << topValue << ", низ=" << bottomValue << "\n";
    cout << "Начальное состояние:\n";
    printBoard(temp, n, m);
    
    // Верхняя половина
    for (int i = 0; i < midRow; i++) {
        for (int j = 0; j < m; j++) {
            if (temp[i][j] != topValue) {
                int dx[] = {0, 1, 0, -1};
                int dy[] = {1, 0, -1, 0};
                string dir[] = {"вправо", "вниз", "влево", "вверх"};
                
                for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        moves++;
                        cout << "\nХод #" << moves << ": Переворачиваем (" 
                             << i << "," << j << ") и (" << ni << "," << nj << ") [" << dir[k] << "]\n";
                        
                        temp[i][j] = 1 - temp[i][j];
                        temp[ni][nj] = 1 - temp[ni][nj];
                        
                        cout << "  После: (" << i << "," << j << ")=" << temp[i][j] 
                             << ", (" << ni << "," << nj << ")=" << temp[ni][nj] << "\n";
                        cout << "Текущее состояние:\n";
                        printBoard(temp, n, m);
                        break;
                    }
                }
            }
        }
    }
    
    // Нижняя половина
    for (int i = midRow; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (temp[i][j] != bottomValue) {
                int dx[] = {0, 1, 0, -1};
                int dy[] = {1, 0, -1, 0};
                string dir[] = {"вправо", "вниз", "влево", "вверх"};
                
                for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        moves++;
                        cout << "\nХод #" << moves << ": Переворачиваем (" 
                             << i << "," << j << ") и (" << ni << "," << nj << ") [" << dir[k] << "]\n";
                        
                        temp[i][j] = 1 - temp[i][j];
                        temp[ni][nj] = 1 - temp[ni][nj];
                        
                        cout << "  После: (" << i << "," << j << ")=" << temp[i][j] 
                             << ", (" << ni << "," << nj << ")=" << temp[ni][nj] << "\n";
                        cout << "Текущее состояние:\n";
                        printBoard(temp, n, m);
                        break;
                    }
                }
            }
        }
    }
    
    cout << "\nВсего ходов: " << moves << "\n";
}

// Генерация случайной доски
void generateRandomBoard(vector<vector<int>>& board, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            board[i][j] = rand() % 2;
        }
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    srand(time(0));
    
    int n, m;
    cout << "Введите количество строк (N): ";
    cin >> n;
    cout << "Введите количество столбцов (M): ";
    cin >> m;
    
    vector<vector<int>> board(n, vector<int>(m));
    
    // Выбор способа ввода
    cout << "\nВыберите способ заполнения доски:\n";
    cout << "1. Ручной ввод\n";
    cout << "2. Случайная генерация\n";
    cout << "Ваш выбор: ";
    int choice;
    cin >> choice;
    
    if (choice == 1) {
        cout << "\nВведите состояние доски (0 - орел, 1 - решка):\n";
        for (int i = 0; i < n; i++) {
            cout << "Строка " << i << ": ";
            for (int j = 0; j < m; j++) {
                cin >> board[i][j];
            }
        }
    } else {
        generateRandomBoard(board, n, m);
    }
    
    cout << "\nСгенерированная доска:\n";
    printBoard(board, n, m);
    
    // Считаем ходы для обоих вариантов
    int moves1 = countMovesToHalf(board, n, m, 0, 1);
    int moves2 = countMovesToHalf(board, n, m, 1, 0);
    
    // Определяем оптимальный вариант
    int bestTop, bestBottom;
    if (moves1 <= moves2) {
        bestTop = 0;
        bestBottom = 1;
    } else {
        bestTop = 1;
        bestBottom = 0;
    }
    
    // Выбор режима отображения шагов
    cout << "\nПоказывать пошаговое решение? (1 - да, 0 - нет): ";
    int showStepsChoice;
    cin >> showStepsChoice;
    
    if (showStepsChoice == 1) {
        // Выводим шаги только для оптимального варианта
        solveWithSteps(board, n, m, bestTop, bestBottom);
    }
    
    // Итоговый результат
    cout << "\n Итоги\n";
    cout << "Минимальное количество ходов: " << min(moves1, moves2) << "\n";
    cout << "Оптимальная конфигурация: верхняя половина - " << bestTop 
         << ", нижняя - " << bestBottom << "\n";
    
    return 0;
}