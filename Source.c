#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#define MAX_SIMS 100

typedef enum {
    FLIGHT_SIM,
    DRIVING_SIM,
    SPACE_SIM,
    TRAIN_SIM,
    MILITARY_SIM,
    OTHER_SIM
} SimType;

typedef enum {
    BEGINNER,
    INTERMEDIATE,
    ADVANCED,
    EXPERT
} Difficulty;

typedef struct {
    char title[40];
    char developer[40];
    char publisher[40];
    int year;
    SimType type;
    Difficulty difficulty;
    float physics;
    int missions;
} Simulator;

char* getTypeName(SimType t) {
    switch (t) {
    case FLIGHT_SIM: return "Авиасимулятор";
    case DRIVING_SIM: return "Автосимулятор";
    case SPACE_SIM: return "Космический";
    case TRAIN_SIM: return "Железнодорожный";
    case MILITARY_SIM: return "Военный";
    default: return "Другой";
    }
}

char* getDiffName(Difficulty d) {
    switch (d) {
    case BEGINNER: return "Начинающий";
    case INTERMEDIATE: return "Средний";
    case ADVANCED: return "Продвинутый";
    case EXPERT: return "Эксперт";
    default: return "Неизвестно";
    }
}

SimType getTypeFromString(char* str) {
    if (strcmp(str, "Авиасимулятор") == 0) return FLIGHT_SIM;
    if (strcmp(str, "Автосимулятор") == 0) return DRIVING_SIM;
    if (strcmp(str, "Космический") == 0) return SPACE_SIM;
    if (strcmp(str, "Железнодорожный") == 0) return TRAIN_SIM;
    if (strcmp(str, "Военный") == 0) return MILITARY_SIM;
    return OTHER_SIM;
}

Difficulty getDiffFromString(char* str) {
    if (strcmp(str, "Начинающий") == 0) return BEGINNER;
    if (strcmp(str, "Средний") == 0) return INTERMEDIATE;
    if (strcmp(str, "Продвинутый") == 0) return ADVANCED;
    if (strcmp(str, "Эксперт") == 0) return EXPERT;
    return BEGINNER;
}

int printTable(Simulator* a, int n);
int loadFromFile(char* fname, Simulator* a, int max);
int saveToFile(char* fname, Simulator* a, int n);
int addSimToArray(Simulator* a, int* n, int max);
int saveArrayToFile(Simulator* a, int n);
Simulator* sortByTitleTypeDiff(Simulator* a, int n);
int searchByType(Simulator* a, int n, SimType type);
int searchByPublisherAndDiff(Simulator* a, int n, char* publisher, Difficulty diff);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "RUS");
    puts("*************************************************************************************");
    puts("*                                                                                   *");
    puts("*     Курсовой проект по дисциплине: ""Основы программирования и алгоритмизации""       *");
    puts("*           Тема: ""Разработка файловой базы данных'Игры-Симуляторы'""                  *");
    puts("*                     Выполнил: Копытин М.Е., группа бТИИ-251                       *");
    puts("*                                                                                   *");
    puts("*************************************************************************************\n");
    puts("Программа представляет собой базу данных игр-симуляторов для управления.");
    puts("Основные функции: отображение списка симуляторов в табличном формате,");
    puts("загрузка из файла, добавление новых записей с сохранением в файл,");
    puts("сортировка, поиск по типу, комбинированный поиск по издателю и сложности.");

    Simulator sims[MAX_SIMS];
    int size = 0;

    printf("\nЗагрузка данных из файла 'sims.txt'...\n");
    size = loadFromFile("sims.txt", sims, MAX_SIMS);
    if (size > 0) {
        printf("Загружено %d записей.\n", size);
    }
    else {
        printf("Файл 'sims.txt' не найден. Начните с пустой базы.\n");
    }

    int choice;
    char fname[64];

    while (1) {
        printf("\n----- БАЗА ДАННЫХ ИГР-СИМУЛЯТОРОВ -----\n");
        printf("1. Показать все симуляторы\n");
        printf("2. Загрузить из файла\n");
        printf("3. Сохранить в файл\n");
        printf("4. Добавить симулятор в массив\n");
        printf("5. Сохранить весь массив в файл\n");
        printf("6. Сортировка (название -> тип -> сложность)\n");
        printf("7. Поиск по типу симулятора\n");
        printf("8. Комбинированный поиск (издатель + сложность)\n");
        printf("0. Выход\nВаш выбор: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            break;
        }
        if (choice == 0) break;

        switch (choice) {
        case 1:
            printTable(sims, size);
            break;
        case 2:
            printf("Имя файла для чтения: ");
            scanf("%63s", fname);
            size = loadFromFile(fname, sims, MAX_SIMS);
            break;
        case 3:
            printf("Имя файла для сохранения: ");
            scanf("%63s", fname);
            saveToFile(fname, sims, size);
            break;
        case 4: {
            int added = addSimToArray(sims, &size, MAX_SIMS);
            if (added) {
                printf("\nСимулятор добавлен в массив. Всего записей: %d\n", size);
                printTable(sims, size);
            }
            break;
        }
        case 5: {
            int result = saveArrayToFile(sims, size);
            if (result) {
                printf("Массив успешно сохранен.\n");
            }
            break;
        }
        case 6: {
            Simulator* result = sortByTitleTypeDiff(sims, size);
            if (result) {
                printf("Отсортировано по названию -> типу -> сложности.\n");
                printTable(result, size);
                free(result);
            }
            break;
        }
        case 7: {
            int typeChoice;
            printf("Выберите тип симулятора для поиска:\n");
            printf("1. Авиасимулятор\n");
            printf("2. Автосимулятор\n");
            printf("3. Космический\n");
            printf("4. Железнодорожный\n");
            printf("5. Военный\n");
            printf("6. Другой\n");
            printf("Ваш выбор: ");
            scanf("%d", &typeChoice);

            SimType searchType;
            switch (typeChoice) {
            case 1: searchType = FLIGHT_SIM; break;
            case 2: searchType = DRIVING_SIM; break;
            case 3: searchType = SPACE_SIM; break;
            case 4: searchType = TRAIN_SIM; break;
            case 5: searchType = MILITARY_SIM; break;
            default: searchType = OTHER_SIM;
            }

            int result_size = searchByType(sims, size, searchType);
            if (result_size == 0) printf("Ничего не найдено.\n");
            break;
        }
        case 8: {
            char publisher[40];
            int diffChoice;
            printf("Введите издателя: ");
            scanf("%39s", publisher);
            printf("Выберите уровень сложности:\n");
            printf("1. Начинающий\n");
            printf("2. Средний\n");
            printf("3. Продвинутый\n");
            printf("4. Эксперт\n");
            printf("Ваш выбор: ");
            scanf("%d", &diffChoice);

            Difficulty searchDiff;
            switch (diffChoice) {
            case 1: searchDiff = BEGINNER; break;
            case 2: searchDiff = INTERMEDIATE; break;
            case 3: searchDiff = ADVANCED; break;
            case 4: searchDiff = EXPERT; break;
            default: searchDiff = BEGINNER;
            }

            int result_size = searchByPublisherAndDiff(sims, size, publisher, searchDiff);
            if (result_size == 0) printf("Ничего не найдено.\n");
            break;
        }
        default:
            printf("Неверный пункт меню.\n");
        }
    }
    return 0;
}

int printTable(Simulator* a, int n) {
    int i;
    if (n <= 0) {
        printf("Список пуст.\n");
        return 0;
    }
    printf("\n---------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| № | %-28s  | %-17s | %-16s | Год  | %-16s | %-12s |  Физ. |  Миссии |\n", "Название", "Разработчик", "Издатель", "Тип", "Сложность");
    printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("| %2d | %-28s | %-17s | %-16s | %4d | %-16s | %-12s | %5.1f | %7d |\n", i + 1, a[i].title, a[i].developer, a[i].publisher, a[i].year, getTypeName(a[i].type), getDiffName(a[i].difficulty), a[i].physics, a[i].missions);
    }
    printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
    return 1;
}

int loadFromFile(char* fname, Simulator* a, int max) {
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Ошибка: не удалось открыть файл '%s'\n", fname);
        return 0;
    }

    char line[256];
    int count = 0;
    int line_num = 0;

    while (fgets(line, sizeof(line), f) && count < max) {
        line_num++;

        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;

        Simulator s;
        char typeStr[30], diffStr[20];

        int cnt = sscanf(line, "%[^;];%[^;];%[^;];%d;%[^;];%[^;];%f;%d",
            s.title, s.developer, s.publisher, &s.year,
            typeStr, diffStr, &s.physics, &s.missions);

        if (cnt == 8) {
            s.type = getTypeFromString(typeStr);
            s.difficulty = getDiffFromString(diffStr);
            a[count] = s;
            count++;
        }
        else {
            printf("Ошибка в строке %d: неверный формат\n", line_num);
        }
    }

    if (ferror(f)) {
        printf("Ошибка чтения файла '%s'\n", fname);
    }

    fclose(f);

    printf("Загружено записей: %d\n", count);
    return count;
}

int saveToFile(char* fname, Simulator* a, int n) {
    if (n <= 0) {
        printf("Ошибка: нечего сохранять (массив пуст)\n");
        return 0;
    }

    FILE* f = fopen(fname, "w");
    if (!f) {
        printf("Ошибка: не удалось создать файл '%s'\n", fname);
        return 0;
    }

    int saved = 0;
    for (int i = 0; i < n; i++) {
        int result = fprintf(f, "%s;%s;%s;%d;%s;%s;%.1f;%d\n",
            a[i].title, a[i].developer, a[i].publisher, a[i].year,
            getTypeName(a[i].type), getDiffName(a[i].difficulty),
            a[i].physics, a[i].missions);

        if (result < 0) {
            printf("Ошибка записи записи %d\n", i + 1);
        }
        else {
            saved++;
        }
    }

    if (fflush(f) != 0) {
        printf("Ошибка сброса буфера файла\n");
    }

    if (fclose(f) != 0) {
        printf("Ошибка закрытия файла\n");
    }

    printf("Сохранено записей: %d\n", saved);
    return saved;
}

int addSimToArray(Simulator* a, int* n, int max) {
    if (*n >= max) {
        printf("Ошибка: массив переполнен (максимум %d записей)\n", max);
        return 0;
    }

    Simulator s;
    int typeChoice, diffChoice;

    printf("Введите данные симулятора (без пробелов, вместо них _):\n");

    printf("Название: ");
    if (scanf("%39s", s.title) != 1) {
        printf("Ошибка ввода названия\n");
        while (getchar() != '\n');
        return 0;
    }

    printf("Разработчик: ");
    if (scanf("%39s", s.developer) != 1) {
        printf("Ошибка ввода разработчика\n");
        while (getchar() != '\n');
        return 0;
    }

    printf("Издатель: ");
    if (scanf("%39s", s.publisher) != 1) {
        printf("Ошибка ввода издателя\n");
        while (getchar() != '\n');
        return 0;
    }

    printf("Год выпуска: ");
    if (scanf("%d", &s.year) != 1) {
        printf("Ошибка ввода года\n");
        while (getchar() != '\n');
        return 0;
    }

    if (s.year < 1980 || s.year > 2030) {
        printf("Предупреждение: год %d вне диапазона\n", s.year);
    }

    printf("Тип симулятора (1-Авиа,2-Авто,3-Косм,4-Ж/д,5-Воен,6-Другой): ");
    if (scanf("%d", &typeChoice) != 1) {
        printf("Ошибка ввода типа\n");
        while (getchar() != '\n');
        return 0;
    }

    if (typeChoice < 1 || typeChoice > 6) {
        printf("Ошибка: некорректный тип\n");
        while (getchar() != '\n');
        return 0;
    }
    s.type = (SimType)(typeChoice - 1);

    printf("Уровень сложности (1-Начин,2-Средн,3-Продв,4-Эксперт): ");
    if (scanf("%d", &diffChoice) != 1) {
        printf("Ошибка ввода сложности\n");
        while (getchar() != '\n');
        return 0;
    }

    if (diffChoice < 1 || diffChoice > 4) {
        printf("Ошибка: некорректная сложность\n");
        while (getchar() != '\n');
        return 0;
    }
    s.difficulty = (Difficulty)(diffChoice - 1);

    printf("Реалистичность физики (0-10): ");
    if (scanf("%f", &s.physics) != 1) {
        printf("Ошибка ввода физики\n");
        while (getchar() != '\n');
        return 0;
    }

    if (s.physics < 0 || s.physics > 10) {
        printf("Предупреждение: физика %.1f вне диапазона\n", s.physics);
    }

    printf("Количество миссий: ");
    if (scanf("%d", &s.missions) != 1) {
        printf("Ошибка ввода миссий\n");
        while (getchar() != '\n');
        return 0;
    }

    if (s.missions < 0) {
        printf("Предупреждение: количество миссий %d отрицательное\n", s.missions);
    }

    a[*n] = s;
    (*n)++;
    return 1;
}

int saveArrayToFile(Simulator* a, int n) {
    if (n <= 0) {
        printf("Массив пуст. Нечего сохранять.\n");
        return 0;
    }

    char fname[64];
    printf("Введите имя файла: ");
    if (scanf("%63s", fname) != 1) {
        printf("Ошибка ввода имени файла\n");
        while (getchar() != '\n');
        return 0;
    }

    return saveToFile(fname, a, n);
}

Simulator* sortByTitleTypeDiff(Simulator* a, int n) {
    if (n <= 0) {
        printf("Массив пуст\n");
        return NULL;
    }

    Simulator* result = (Simulator*)malloc(n * sizeof(Simulator));
    if (!result) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        result[i] = a[i];
    }

    for (int i = 1; i < n; i++) {
        Simulator key = result[i];
        int j = i - 1;
        while (j >= 0) {
            int swap = 0;
            if (strcmp(result[j].title, key.title) > 0) {
                swap = 1;
            }
            else if (strcmp(result[j].title, key.title) == 0 &&
                result[j].type > key.type) {
                swap = 1;
            }
            else if (strcmp(result[j].title, key.title) == 0 &&
                result[j].type == key.type &&
                result[j].difficulty > key.difficulty) {
                swap = 1;
            }

            if (swap) {
                result[j + 1] = result[j];
                j--;
            }
            else {
                break;
            }
        }
        result[j + 1] = key;
    }
    return result;
}

int searchByType(Simulator* a, int n, SimType type) {
    if (n <= 0) {
        printf("Массив пуст\n");
        return 0;
    }

    Simulator* result = (Simulator*)malloc(n * sizeof(Simulator));
    if (!result) {
        printf("Ошибка выделения памяти\n");
        return 0;
    }

    int k = 0;
    for (int i = 0; i < n; i++) {
        if (a[i].type == type) {
            result[k++] = a[i];
        }
    }
    if (k) {
        printf("\n=== РЕЗУЛЬТАТЫ ПОИСКА ПО ТИПУ: %s ===\n", getTypeName(type));
        printTable(result, k);
    }
    free(result);
    return k;
}

int searchByPublisherAndDiff(Simulator* a, int n, char* publisher, Difficulty diff) {
    if (n <= 0) {
        printf("Массив пуст\n");
        return 0;
    }

    Simulator* result = (Simulator*)malloc(n * sizeof(Simulator));
    if (!result) {
        printf("Ошибка выделения памяти\n");
        return 0;
    }

    int k = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(a[i].publisher, publisher) == 0 && a[i].difficulty == diff) {
            result[k++] = a[i];
        }
    }
    if (k) {
        printf("\n=== РЕЗУЛЬТАТЫ КОМБИНИРОВАННОГО ПОИСКА ===\n");
        printf("Издатель: %s, Сложность: %s\n", publisher, getDiffName(diff));
        printTable(result, k);
    }
    free(result);
    return k;
}