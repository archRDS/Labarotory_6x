package main

import (
	"fmt"
	"math/rand"
	"time"
)

// Функция для вывода доски
func printBoard(board [][]int, n, m int) {
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			fmt.Print(board[i][j])
			if j < m-1 {
				fmt.Print(" ")
			}
		}
		fmt.Println()
	}
}

// Функция для подсчета ходов (без вывода шагов)
func countMovesToHalf(board [][]int, n, m, topValue, bottomValue int) int {
	moves := 0
	midRow := n / 2
	
	// Глубокое копирование доски (слайсы в Го — ссылочный тип)
	temp := make([][]int, n)
	for i := range temp {
		temp[i] = make([]int, m)
		copy(temp[i], board[i])
	}
	
	// Обрабатываем верхнюю половину
	for i := 0; i < midRow; i++ {
		for j := 0; j < m; j++ {
			if temp[i][j] != topValue {
				dx := []int{0, 1, 0, -1}
				dy := []int{1, 0, -1, 0}
				
				for k := 0; k < 4; k++ {
					ni := i + dx[k]
					nj := j + dy[k]
					
					if ni >= 0 && ni < n && nj >= 0 && nj < m {
						temp[i][j] = 1 - temp[i][j]
						temp[ni][nj] = 1 - temp[ni][nj]
						moves++
						break
					}
				}
			}
		}
	}
	
	// Обрабатываем нижнюю половину
	for i := midRow; i < n; i++ {
		for j := 0; j < m; j++ {
			if temp[i][j] != bottomValue {
				dx := []int{0, 1, 0, -1}
				dy := []int{1, 0, -1, 0}
				
				for k := 0; k < 4; k++ {
					ni := i + dx[k]
					nj := j + dy[k]
					
					if ni >= 0 && ni < n && nj >= 0 && nj < m {
						temp[i][j] = 1 - temp[i][j]
						temp[ni][nj] = 1 - temp[ni][nj]
						moves++
						break
					}
				}
			}
		}
	}
	
	return moves
}

// Функция для выполнения решения с выводом шагов
func solveWithSteps(board [][]int, n, m, topValue, bottomValue int) {
	moves := 0
	midRow := n / 2
	
	// Глубокое копирование доски
	temp := make([][]int, n)
	for i := range temp {
		temp[i] = make([]int, m)
		copy(temp[i], board[i])
	}
	
	fmt.Printf("\nЦелевая конфигурация: верх=%d, низ=%d\n", topValue, bottomValue)
	fmt.Println("Начальное состояние:")
	printBoard(temp, n, m)
	
	// Направления и их названия
	dx := []int{0, 1, 0, -1}
	dy := []int{1, 0, -1, 0}
	dir := []string{"вправо", "вниз", "влево", "вверх"}
	
	// Верхняя половина
	for i := 0; i < midRow; i++ {
		for j := 0; j < m; j++ {
			if temp[i][j] != topValue {
				for k := 0; k < 4; k++ {
					ni := i + dx[k]
					nj := j + dy[k]
					
					if ni >= 0 && ni < n && nj >= 0 && nj < m {
						moves++
						fmt.Printf("\nХод #%d: Переворачиваем (%d,%d) и (%d,%d) [%s]\n",
							moves, i, j, ni, nj, dir[k])
						
						temp[i][j] = 1 - temp[i][j]
						temp[ni][nj] = 1 - temp[ni][nj]
						
						fmt.Printf("  После: (%d,%d)=%d, (%d,%d)=%d\n",
							i, j, temp[i][j], ni, nj, temp[ni][nj])
						fmt.Println("Текущее состояние:")
						printBoard(temp, n, m)
						break
					}
				}
			}
		}
	}
	
	// Нижняя половина
	for i := midRow; i < n; i++ {
		for j := 0; j < m; j++ {
			if temp[i][j] != bottomValue {
				for k := 0; k < 4; k++ {
					ni := i + dx[k]
					nj := j + dy[k]
					
					if ni >= 0 && ni < n && nj >= 0 && nj < m {
						moves++
						fmt.Printf("\nХод #%d: Переворачиваем (%d,%d) и (%d,%d) [%s]\n",
							moves, i, j, ni, nj, dir[k])
						
						temp[i][j] = 1 - temp[i][j]
						temp[ni][nj] = 1 - temp[ni][nj]
						
						fmt.Printf("  После: (%d,%d)=%d, (%d,%d)=%d\n",
							i, j, temp[i][j], ni, nj, temp[ni][nj])
						fmt.Println("Текущее состояние:")
						printBoard(temp, n, m)
						break
					}
				}
			}
		}
	}
	
	fmt.Printf("\nВсего ходов: %d\n", moves)
}

// Генерация случайной доски
func generateRandomBoard(board [][]int, n, m int) {
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			board[i][j] = rand.Intn(2)
		}
	}
}

// Вспомогательная функция min для целых чисел
func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func main() {
	// Инициализация генератора случайных чисел
	rand.Seed(time.Now().UnixNano())
	
	var n, m int
	fmt.Print("Введите количество строк (N): ")
	fmt.Scan(&n)
	fmt.Print("Введите количество столбцов (M): ")
	fmt.Scan(&m)
	
	// Создание доски
	board := make([][]int, n)
	for i := range board {
		board[i] = make([]int, m)
	}
	
	// Выбор способа ввода
	fmt.Println("\nВыберите способ заполнения доски:")
	fmt.Println("1. Ручной ввод")
	fmt.Println("2. Случайная генерация")
	fmt.Print("Ваш выбор: ")
	var choice int
	fmt.Scan(&choice)
	
	if choice == 1 {
		fmt.Println("\nВведите состояние доски (0 - орел, 1 - решка):")
		for i := 0; i < n; i++ {
			fmt.Printf("Строка %d: ", i)
			for j := 0; j < m; j++ {
				fmt.Scan(&board[i][j])
			}
		}
	} else {
		generateRandomBoard(board, n, m)
	}
	
	fmt.Println("\nСгенерированная доска:")
	printBoard(board, n, m)
	
	// Считаем ходы для обоих вариантов
	moves1 := countMovesToHalf(board, n, m, 0, 1)
	moves2 := countMovesToHalf(board, n, m, 1, 0)
	
	// Определяем оптимальный вариант
	bestTop, bestBottom := 0, 1
	if moves1 > moves2 {
		bestTop, bestBottom = 1, 0
	}
	
	// Выбор режима отображения шагов
	fmt.Print("\nПоказывать пошаговое решение? (1 - да, 0 - нет): ")
	var showStepsChoice int
	fmt.Scan(&showStepsChoice)
	
	if showStepsChoice == 1 {
		// Выводим шаги только для оптимального варианта
		solveWithSteps(board, n, m, bestTop, bestBottom)
	}
	
	// Итоговый результат
	fmt.Println("\n Итоги")
	fmt.Printf("Минимальное количество ходов: %d\n", min(moves1, moves2))
	fmt.Printf("Оптимальная конфигурация: верхняя половина - %d, нижняя - %d\n",
		bestTop, bestBottom)
}