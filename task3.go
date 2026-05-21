package main

import (
	"fmt"
	"math"
)

const EPS = 1e-3
const MAX_ITER = 1000

// Функция для вывода матрицы
func printMatrix(a [][]float64, b []float64) {
	n := len(a)
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			fmt.Printf("%8.2f ", a[i][j])
		}
		fmt.Printf("| %8.2f\n", b[i])
	}
}

// Метод Гаусса с выбором главного элемента по столбцу
func gaussMethod(a [][]float64, b []float64) []float64 {
	n := len(a)
	
	// Создаем копии, чтобы не модифицировать оригинал
	// В С++ векторы передавались по значению, но в Го слайсы - ссылочный тип
	aa := make([][]float64, n)
	for i := range aa {
		aa[i] = make([]float64, n)
		copy(aa[i], a[i])
	}
	bb := make([]float64, n)
	copy(bb, b)
	
	fmt.Println(" МЕТОД ГАУССА С ВЫБОРОМ ГЛАВНОГО ЭЛЕМЕНТА")
	fmt.Println("\nИсходная система:")
	printMatrix(aa, bb)
	
	// Прямой ход
	for k := 0; k < n; k++ {
		// Выбор главного элемента
		maxRow := k
		maxVal := math.Abs(aa[k][k])
		for i := k + 1; i < n; i++ {
			if math.Abs(aa[i][k]) > maxVal {
				maxVal = math.Abs(aa[i][k])
				maxRow = i
			}
		}
		
		// Перестановка строк
		if maxRow != k {
			aa[k], aa[maxRow] = aa[maxRow], aa[k]
			bb[k], bb[maxRow] = bb[maxRow], bb[k]
			fmt.Printf("\nПерестановка строк %d и %d\n", k, maxRow)
		}
		
		// Исключение
		for i := k + 1; i < n; i++ {
			factor := aa[i][k] / aa[k][k]
			for j := k; j < n; j++ {
				aa[i][j] -= factor * aa[k][j]
			}
			bb[i] -= factor * bb[k]
		}
		
		fmt.Printf("\nПосле шага %d:\n", k+1)
		printMatrix(aa, bb)
	}
	
	// Обратный ход
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		x[i] = bb[i]
		for j := i + 1; j < n; j++ {
			x[i] -= aa[i][j] * x[j]
		}
		x[i] /= aa[i][i]
	}
	
	fmt.Println("\nРезультат прямого метода (Гаусс):")
	for i := 0; i < n; i++ {
		fmt.Printf("x%d = %.6f\n", i+1, x[i])
	}
	
	return x
}

// Метод Зейделя
func seidelMethod(a [][]float64, b []float64) []float64 {
	n := len(a)
	x := make([]float64, n)
	xOld := make([]float64, n)
	
	fmt.Println("\n МЕТОД ЗЕЙДЕЛЯ ")
	fmt.Print("\nНачальное приближение: ")
	for i := 0; i < n; i++ {
		fmt.Print("0 ")
	}
	fmt.Println()
	
	// Проверка условия диагонального преобладания
	_ = true // diagonalDominance вычислялся, но не использовался в оригинале
	// Оставляем вычисление для совместимости с логикой
	_ = func() bool {
		for i := 0; i < n; i++ {
			sum := 0.0
			for j := 0; j < n; j++ {
				if i != j {
					sum += math.Abs(a[i][j])
				}
			}
			if math.Abs(a[i][i]) <= sum {
				return false
			}
		}
		return true
	}()
	
	iter := 0
	error := 1.0
	
	fmt.Println("\nИтерации:")
	fmt.Printf("%4s%12s%12s%12s%12s%12s\n", "N", "x1", "x2", "x3", "x4", "Error")
	
	for error > EPS && iter < MAX_ITER {
		copy(xOld, x)
		
		for i := 0; i < n; i++ {
			sum := b[i]
			for j := 0; j < n; j++ {
				if i != j {
					sum -= a[i][j] * x[j]
				}
			}
			x[i] = sum / a[i][i]
		}
		
		// Вычисление погрешности
		error = 0
		for i := 0; i < n; i++ {
			diff := math.Abs(x[i] - xOld[i])
			if diff > error {
				error = diff
			}
		}
		
		iter++
		fmt.Printf("%4d%12.6f%12.6f%12.6f%12.6f%12.6f\n", iter, x[0], x[1], x[2], x[3], error)
	}
	
	fmt.Println("\nРезультат итерационного метода (Зейдель):")
	for i := 0; i < n; i++ {
		fmt.Printf("x%d = %.6f\n", i+1, x[i])
	}
	fmt.Printf("Количество итераций: %d\n", iter)
	fmt.Printf("Погрешность: %g\n", error)
	
	return x
}

func main() {
	// Система для варианта 8
	a := [][]float64{
		{0.91, -0.04, 0.21, -1.16},
		{0.25, -1.23, -0.23, -0.09},
		{-0.21, -0.23, 0.80, -0.13},
		{0.15, -1.31, 0.06, -1.04},
	}
	
	b := []float64{-1.24, -1.04, 2.56, 0.91}
	
	fmt.Println("СИСТЕМА ЛИНЕЙНЫХ УРАВНЕНИЙ")
	fmt.Println("Вариант 8")
	fmt.Printf("Точность: %g\n\n", EPS)
	
	// Решение методом Гаусса
	_ = gaussMethod(a, b)
	
	// Решение методом Зейделя
	_ = seidelMethod(a, b)
}