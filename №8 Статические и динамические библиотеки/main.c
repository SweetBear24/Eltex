#include <stdio.h>
#include <dlfcn.h> // Для dlopen, dlsym, dlclose

int main() {
    // Открытие динамической библиотеки
    void *handle = dlopen("./lib/libapp.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка при загрузке библиотеки: %s\n", dlerror());
        return 1;
    }

    // Очистка ошибок, если есть
    dlerror();

    // Получение адреса функции add из библиотеки
    int (*add_func)(int, int) = (int (*)(int, int)) dlsym(handle, "add");
    char *error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "Ошибка при поиске функции add: %s\n", error);
        dlclose(handle);
        return 1;
    }

    // Вызов функции add
    int result = add_func(5, 7);
    printf("Результат вызова функции add(5, 7): %d\n", result);

    // Закрытие библиотеки
    if (dlclose(handle) != 0) {
        fprintf(stderr, "Ошибка при закрытии библиотеки: %s\n", dlerror());
        return 1;
    }

    return 0;
}
