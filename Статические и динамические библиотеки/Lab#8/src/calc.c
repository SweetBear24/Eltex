#include "calc.h"

// Реализация публичной функции
int add(int a, int b) {
    return a + b;
}

// Вспомогательная функция (скрытая)
static int multiply(int a, int b) {
    return a * b;
}
