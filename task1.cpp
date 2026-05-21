#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

// ЗАДАНИЕ 1
void task1() {
    int m, n;
    cout << "Введите M (>7): ";
    cin >> m;
    cout << "Введите N (>5): ";
    cin >> n;

    if (m <= 7 || n <= 5) {
        cout << "Ошибка: M должно быть > 7, N > 5\n";
        return;
    }

    vector<vector<int>> matrix(m, vector<int>(n));
    
    // Заполнение случайными числами [-40, 20]
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 61 - 40;
        }
    }

    // Вывод матрицы
    cout << "\nМатрица:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    // Поиск строк с >= 3 отрицательными элементами
    vector<int> validRows;
    vector<int> resultArray;

    for (int i = 0; i < m; i++) {
        int negCount = 0;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < 0) negCount++;
        }
        
        if (negCount >= 3) {
            validRows.push_back(i + 1); // Нумерация с 1
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] < 0) {
                    resultArray.push_back(matrix[i][j]);
                }
            }
        }
    }

    // Вывод результатов
    cout << "\nСтроки с >= 3 отрицательными элементами: ";
    for (int row : validRows) cout << row << " ";
    cout << "\n\nКоличество найденных строк: " << validRows.size() << endl;
    cout << "Количество отрицательных элементов в массиве: " << resultArray.size() << endl;
    cout << "Массив: ";
    for (int val : resultArray) cout << val << " ";
    cout << endl;
}

// ЗАДАНИЕ 2
void task2() {
    int n;
    cout << "Введите N (>3, нечётное): ";
    cin >> n;

    if (n <= 3 || n % 2 == 0) {
        cout << "Ошибка: N должно быть > 3 и нечётным\n";
        return;
    }

    vector<vector<int>> matrix(n, vector<int>(n));
    
    // Заполнение случайными числами [10, 30]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 21 + 10;
        }
    }

    // Вывод исходной матрицы
    cout << "\nИсходная матрица:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    int center = n / 2;
    
    // 1. Заменяем левую нижнюю четверть на 0
    for (int i = center + 1; i < n; i++) {
        for (int j = 0; j < center; j++) {
            matrix[i][j] = 0;
        }
    }
    
    // 2. Заменяем правую нижнюю четверть на 10
    for (int i = center + 1; i < n; i++) {
        for (int j = center + 1; j < n; j++) {
            matrix[i][j] = 10;
        }
    }
    
    // 3. Меняем местами верхнюю левую и верхнюю правую четверти
    for (int i = 0; i < center; i++) {
        for (int j = 0; j < center; j++) {
            swap(matrix[i][j], matrix[i][j + center + 1]);
        }
    }

    // Вывод обработанной матрицы
    cout << "\nОбработанная матрица:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

//ЗАДАНИЕ 3  
int cntneighbours(vector<vector<int>>& matrx, int x, int y, int m, int n) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + m) % m;  // Замкнутая плоскость
            int ny = (y + dy + n) % n;
            count += matrx[nx][ny];
        }
    }
    return count;
}

void nextGen(vector<vector<int>>& matrx, int m, int n) {
    vector<vector<int>> newmat = matrx;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int ngbr = cntneighbours(matrx, i, j, m, n);
            if (matrx[i][j] == 1) {
                newmat[i][j] = (ngbr == 2 || ngbr == 3) ? 1 : 0;
            } else {
                newmat[i][j] = (ngbr == 3) ? 1 : 0;
            }
        }
    }
    matrx = newmat;
}

void print(vector<vector<int>>& matrx, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            cout << (matrx[i][j] ? "0 " : ". ");
        cout << endl;
    }
}

// Gosper Glider Gun
void addGosperGliderGun(vector<vector<int>>& matrx, int x, int y) {
    // Левая часть
    matrx[x+4][y] = matrx[x+4][y+1] = matrx[x+5][y] = matrx[x+5][y+1] = 1;
    matrx[x+4][y+10] = matrx[x+5][y+10] = matrx[x+6][y+10] = 1;
    matrx[x+3][y+11] = matrx[x+7][y+11] = 1;
    matrx[x+2][y+12] = matrx[x+8][y+12] = 1;
    matrx[x+1][y+13] = matrx[x+9][y+13] = 1;
    matrx[x][y+14] = matrx[x+10][y+14] = 1;
    matrx[x+1][y+15] = matrx[x+9][y+15] = 1;
    matrx[x+2][y+16] = matrx[x+3][y+16] = matrx[x+4][y+16] = matrx[x+5][y+16] = 1;
    matrx[x+3][y+17] = matrx[x+7][y+17] = 1;
    matrx[x+4][y+18] = matrx[x+5][y+18] = matrx[x+6][y+18] = 1;
    // Правая часть
    matrx[x+2][y+24] = matrx[x+2][y+25] = matrx[x+3][y+24] = matrx[x+3][y+25] = 1;
    matrx[x+1][y+24] = matrx[x+1][y+26] = 1;
    matrx[x+3][y+26] = 1;
    matrx[x+2][y+34] = matrx[x+2][y+35] = matrx[x+3][y+34] = matrx[x+3][y+35] = 1;
    matrx[x+1][y+34] = matrx[x+1][y+36] = 1;
    matrx[x+3][y+36] = 1;
}

// Simkin Glider Gun 
void addSimkinGliderGun(vector<vector<int>>& matrx, int x, int y) {
    addGosperGliderGun(matrx, x, y);
    // Дополнительные блоки для Simkin
    matrx[x+12][y+5] = matrx[x+12][y+6] = matrx[x+13][y+5] = matrx[x+13][y+6] = 1;
}

// Twin Bees Shuttle
void addTwinBeesShuttle(vector<vector<int>>& matrx, int x, int y) {
    // Левая пчела
    matrx[x][y+2] = matrx[x][y+3] = matrx[x+1][y+1] = matrx[x+1][y+4] = 1;
    matrx[x+2][y+2] = matrx[x+2][y+3] = 1;
    // Правая пчела
    matrx[x][y+8] = matrx[x][y+9] = matrx[x+1][y+7] = matrx[x+1][y+10] = 1;
    matrx[x+2][y+8] = matrx[x+2][y+9] = 1;
    // Стабилизаторы
    matrx[x+4][y+1] = matrx[x+4][y+2] = matrx[x+5][y] = matrx[x+5][y+3] = 1;
    matrx[x+4][y+9] = matrx[x+4][y+10] = matrx[x+5][y+8] = matrx[x+5][y+11] = 1;
}

void randomFill(vector<vector<int>>& matrx, int m, int n, int density) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrx[i][j] = (rand() % 100 < density) ? 1 : 0;
        }
    }
}

void runSimulation(vector<vector<int>>& matrx, int m, int n, int maxGenerations) {
    cout << "\nСимуляция запущена (макс. " << maxGenerations << " поколений)\n";
    cout << "Нажмите Ctrl+C для остановки...\n\n";

    for (int gen = 0; gen < maxGenerations; gen++) {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        
        cout << "=== Поколение: " << gen + 1 << " ===\n\n";
        print(matrx, m, n);
        nextGen(matrx, m, n);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    cout << "\n\nСимуляция завершена. Нажмите Enter...";
    cin.get();
}

void task3() {
    const int M = 30, N = 60;
    
    while (true) {
        cout << "ИГРА КОНВЕЯ\n\n";
        cout << "1. Gosper Glider Gun\n";
        cout << "2. Simkin Glider Gun\n";
        cout << "3. Twin Bees Shuttle\n";
        cout << "4. Случайная генерация\n";
        cout << "0. Выход\n\n";
        cout << "Выберите фигуру: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        vector<vector<int>> matrx(M, vector<int>(N, 0));
        
        switch (choice) {
        case 1:
            addGosperGliderGun(matrx, 5, 5);
            runSimulation(matrx, M, N, 170);
            break;
        case 2:
            addSimkinGliderGun(matrx, 5, 5);
            runSimulation(matrx, M, N, 170);
            break;
        case 3:
            addTwinBeesShuttle(matrx, 10, 20);
            runSimulation(matrx, M, N, 170);
            break;
        case 4: {
            int density;
            cout << "Введите плотность заполнения (10-40%): ";
            cin >> density;
            cin.ignore();
            randomFill(matrx, M, N, density);
            runSimulation(matrx, M, N, 170);
            break;
        }
        default:
            cout << "Неверный выбор!\n";
            cout << "Нажмите Enter...";
            cin.get();
        }
    }
}


int main() {
    srand(time(0));
    
    while (true) {
        system("cls");
        cout << "ВАРИАНТ 9\n\n";
        cout << "1. Задание 1: Матрица с отрицательными элементами\n";
        cout << "2. Задание 2: Квадратная матрица с четвертями\n";
        cout << "3. Задание 3: Игра Жизнь Конвея\n";
        cout << "0. Выход\n\n";
        cout << "Выберите задание: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
        case 1:
            task1();
            break;
        case 2:
            task2();
            break;
        case 3:
            task3();
            break;
        case 0:
            cout << "Выход из программы.\n";
            return 0;
        default:
            cout << "Неверный выбор!\n";
        }
        
        cout << "\nНажмите Enter для продолжения...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
}