#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

const double EPS = 1e-3;
const int MAX_ITER = 1000;

// Функция для вывода матрицы
void printMatrix(const vector<vector<double>>& a, const vector<double>& b) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(8) << fixed << setprecision(2) << a[i][j] << " ";
        }
        cout << "| " << setw(8) << b[i] << endl;
    }
}

// Метод Гаусса с выбором главного элемента по столбцу
vector<double> gaussMethod(vector<vector<double>> a, vector<double> b) {
    int n = a.size();
    
    cout << " МЕТОД ГАУССА С ВЫБОРОМ ГЛАВНОГО ЭЛЕМЕНТА" << endl;
    cout << "\nИсходная система:" << endl;
    printMatrix(a, b);
    
    // Прямой ход
    for (int k = 0; k < n; k++) {
        // Выбор главного элемента
        int maxRow = k;
        double maxVal = abs(a[k][k]);
        for (int i = k + 1; i < n; i++) {
            if (abs(a[i][k]) > maxVal) {
                maxVal = abs(a[i][k]);
                maxRow = i;
            }
        }
        
        // Перестановка строк
        if (maxRow != k) {
            swap(a[k], a[maxRow]);
            swap(b[k], b[maxRow]);
            cout << "\nПерестановка строк " << k << " и " << maxRow << endl;
        }
        
        // Исключение
        for (int i = k + 1; i < n; i++) {
            double factor = a[i][k] / a[k][k];
            for (int j = k; j < n; j++) {
                a[i][j] -= factor * a[k][j];
            }
            b[i] -= factor * b[k];
        }
        
        cout << "\nПосле шага " << k + 1 << ":" << endl;
        printMatrix(a, b);
    }
    
    // Обратный ход
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= a[i][j] * x[j];
        }
        x[i] /= a[i][i];
    }
    
    cout << "\nРезультат прямого метода (Гаусс):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x[i] << endl;
    }
    
    return x;
}

// Метод Зейделя
vector<double> seidelMethod(vector<vector<double>> a, vector<double> b) {
    int n = a.size();
    vector<double> x(n, 0.0);
    vector<double> x_old(n, 0.0);
    
    cout << "\n МЕТОД ЗЕЙДЕЛЯ " << endl;
    cout << "\nНачальное приближение: ";
    for (int i = 0; i < n; i++) cout << "0 ";
    cout << endl;
    
    // Проверка условия диагонального преобладания
    bool diagonalDominance = true;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            if (i != j) sum += abs(a[i][j]);
        }
        if (abs(a[i][i]) <= sum) {
            diagonalDominance = false;
            break;
        }
    }
    
    int iter = 0;
    double error = 1.0;
    
    cout << "\nИтерации:" << endl;
    cout << setw(4) << "N" << setw(12) << "x1" << setw(12) << "x2" 
         << setw(12) << "x3" << setw(12) << "x4" << setw(12) << "Error" << endl;
    
    while (error > EPS && iter < MAX_ITER) {
        x_old = x;
        
        for (int i = 0; i < n; i++) {
            double sum = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    sum -= a[i][j] * x[j];
                }
            }
            x[i] = sum / a[i][i];
        }
        
        // Вычисление погрешности
        error = 0;
        for (int i = 0; i < n; i++) {
            error = max(error, abs(x[i] - x_old[i]));
        }
        
        iter++;
        cout << setw(4) << iter 
             << setw(12) << fixed << setprecision(6) << x[0]
             << setw(12) << x[1]
             << setw(12) << x[2]
             << setw(12) << x[3]
             << setw(12) << error << endl;
    }
    
    cout << "\nРезультат итерационного метода (Зейдель):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x[i] << endl;
    }
    cout << "Количество итераций: " << iter << endl;
    cout << "Погрешность: " << error << endl;
    
    return x;
}

int main() {
    setlocale(LC_ALL, "rus");
    
    // Система для варианта 8
    vector<vector<double>> a = {
        {0.91, -0.04,  0.21, -1.16},
        {0.25, -1.23, -0.23, -0.09},
        {-0.21, -0.23,  0.80, -0.13},
        {0.15, -1.31,  0.06, -1.04}
    };
    
    vector<double> b = {-1.24, -1.04, 2.56, 0.91};
    
    cout << "СИСТЕМА ЛИНЕЙНЫХ УРАВНЕНИЙ" << endl;
    cout << "Вариант 8" << endl;
    cout << "Точность: " << EPS << endl << endl;
    
    // Решение методом Гаусса
    vector<double> x_gauss = gaussMethod(a, b);
    
    // Решение методом Зейделя
    vector<double> x_seidel = seidelMethod(a, b);
    
    return 0;
}