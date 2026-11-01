#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define MIN_RAND -100
#define MAX_RAND 100

void part1();
void part2();
double randomBetween(const int start, const int end);
double mean_value(const unsigned int *A, unsigned int Size);
unsigned int find_min(const unsigned int *A, unsigned int Size);
float find_max(const float *B, unsigned int Size);
void fill_float_array(float *B, unsigned int Size);
void fill_uint_array(unsigned int *A, unsigned int Size);
void print_uint_array(const unsigned int *A, unsigned int Size);
void print_float_array(const float *B, unsigned int Size);
void pressEnterToContinue();
void fillrand_matrix(double **M, unsigned int rows, unsigned int cols);
void free_matrix(double ***M, unsigned int rows);
void alloc_matrix(double ***M, unsigned int rows, unsigned int cols);
void print_matrix(double **M, unsigned int rows, unsigned int cols);
void transpose_matrix(double ***M, unsigned int *rows, unsigned int *cols);
double** multiply_matrixes(double **A,
                           unsigned int rows_a,
                           unsigned int cols_a,
                           double **B,
                           unsigned int rows_b,
                           unsigned int cols_b);
void max_min(double **M, unsigned int rows, unsigned int cols, double *max, double *min);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    part1();
    part2();
    return 0;
}

// Перша частина лабораторної роботи
void part1()
{
    system("cls");
    printf("Лабораторна робота №4\n"
           "Частина 1. Одновимірні динамічні масиви\n");

    printf("Введіть розмір масиву: ");
    unsigned int size;
    scanf("%u", &size);

    unsigned int * A;
    float * B;

    A = (unsigned int *)calloc(size, sizeof(unsigned int));
    B = (float *) malloc(size * sizeof(float));

    if(A == NULL || B == NULL)
    {
        printf("Пам'ять не була виділена");
        exit(0);
    }

    fill_uint_array(A, size);
    fill_float_array(B, size);

    double average_value = mean_value(A, size);
    unsigned int min_value = find_min(A, size);
    float max_value = find_max(B, size);

    while(true)
    {
        char buffer[2];
        printf("Вивести масив?(y/n)");
        scanf("%2s", buffer);
        if(buffer[0] == 'y')
        {
            printf("Масив A:\n");
            print_uint_array(A, size);

            printf("Масив B:\n");
            print_float_array(B, size);
            break;
        }
        if(buffer[0] == 'n')
            break;
        printf("Невірно введене значення\n");
    }

    printf("Результати обчислень:\n"
           "\taverage_value(A) = %lf\n"
           "\tmin_value(A)     = %u\n"
           "\tmax_value(B)     = %f\n",
           average_value,
           min_value,
           max_value);

    free(A);
    free(B);

    pressEnterToContinue();
}

//Друга частина лабораторної роботи
void part2()
{
    system("cls");
    printf("Лабораторна робота №4\n"
           "Частина 2. Двовимірні динамічні масиви\n");

    srand(time(NULL));

    unsigned int n_a;
    while(true)
    {
        printf("\nВведіть варіант(порядок квадратної матриці A): ");
        scanf("%u", &n_a);
        if(n_a > 0) break;
        printf("Номер варіанта повинен бути > 0!\n");
    }
    unsigned int n_b = n_a;
    unsigned int m_b;

    while(true)
    {
        printf("\nВведіть кількість стовпців m_b матриці B: ");
        scanf("%u", &m_b);
        if(m_b > 0) break;
        printf("Кількість стовпців повинна бути > 0!\n");
    }

    double **A, **B;

    alloc_matrix(&A, n_a, n_a);
    alloc_matrix(&B, n_b, m_b);

    fillrand_matrix(A, n_a, n_a);
    fillrand_matrix(B, n_b, m_b);

    printf("\nМатриця A: \n");
    print_matrix(A, n_a, n_a);
    printf("\nМатриця B: \n");
    print_matrix(B, n_b, m_b);

    double ** prod_result = multiply_matrixes(A, n_a, n_a, B, n_b, m_b);

    printf("\nРезультат множення:\n");
    print_matrix(prod_result, n_a, m_b);

    double min, max;
    max_min(A, n_a, n_a, &max, &min);
    printf("\nСеред елементів матриці А над головною діагоналлю:\n"
           "\tмініум:   %.2lf\n"
           "\tмаксимум: %.2lf\n",
           min,
           max);

    free_matrix(&A, n_a);
    free_matrix(&B, n_b);

    pressEnterToContinue();
}

// Знаходження мінімального та максимального елементу квадратної матриці над головною діагоналлю
void max_min(double **M, unsigned int rows, unsigned int cols, double *max, double *min)
{
    if(rows != cols || M == NULL || min == NULL || max == NULL)
        return;
    for(unsigned int i = 0; i < rows; i++)
        if(M[i] == NULL)
            return;

    // Присвоєння змінам NaN
    *min = 0.0 / 0.0;
    *max = 0.0 / 0.0;

    for(unsigned int i = 0; i < rows; i++)
    {
        for(unsigned int j = i; j < cols; j++)
        {
            if(i == j)continue;
            if(*min != *min || *max != *max)// Встановлення першого значення
            {
                *min = M[i][j];
                *max = M[i][j];
            }
            if(*min > M[i][j])
                *min = M[i][j];
            if(*max < M[i][j])
                *max = M[i][j];
        }
    }
}

// Множення матриць
double** multiply_matrixes(double **A,
                           unsigned int rows_a,
                           unsigned int cols_a,
                           double **B,
                           unsigned int rows_b,
                           unsigned int cols_b)
{
    if(cols_a != rows_b) return NULL;
    if(A == NULL || B == NULL || rows_a == 0 || rows_b == 0 || cols_a == 0 || cols_b == 0) return NULL;
    for(unsigned int i = 0; i < rows_a; i++)
    {
        if(A[i] == NULL) return NULL;
    }
    for(unsigned int i = 0; i < rows_b; i++)
    {
        if(B[i] == NULL) return NULL;
    }

    double **buffer = malloc(rows_a * sizeof(double *));
    if(buffer == NULL) return NULL;
    for(unsigned int i = 0; i < rows_a; i++)
    {
        buffer[i] = malloc(cols_b * sizeof(double));
        if (buffer[i] == NULL) {
            // Очищаємо все, що встигли виділити до цього, щоб уникнути витоку
            for (unsigned int k = 0; k < i; k++) free(buffer[k]);
            free(buffer);
            return NULL;
        }
        for(unsigned int j = 0; j < cols_b; j++)
        {
            double sum = 0;
            for(unsigned int k = 0; k < rows_b;k++)
            {
                sum += A[i][k] * B[k][j];
            }
            buffer[i][j] = sum;
        }
    }

    return buffer;
}

// Заповнення матриці випадковими значеннями у проміжку [MIN_RAND, MAX_RAND]
void fillrand_matrix(double **M, unsigned int rows, unsigned int cols)
{
    if(M == NULL) return;
    for(unsigned int i = 0; i < rows; i++)
    {
        if(M[i] == NULL) return;
        for(unsigned int j = 0; j < cols; j++)
        {
            M[i][j] = randomBetween(MIN_RAND, MAX_RAND);
        }
    }
}

// Транспонування матриці
void transpose_matrix(double ***M, unsigned int *rows, unsigned int *cols)
{
    if(M == NULL || *M == NULL || rows == NULL || cols == NULL || *rows == 0 || *cols == 0)return;
    for(unsigned int i = 0; i < rows; i++)
    {
        if((*M)[i] == NULL) return;
    }
    double **N = *M;

    // rows та cols поміняні місцями як для транспонованої матриці
    alloc_matrix(M, *cols, *rows);

    for(unsigned int i = 0; i < *cols; i++)
    {
        for(unsigned int j = 0; j < *rows; j++)
        {
            (*M)[i][j] = N[j][i];
        }
    }

    free_matrix(&N, *rows);

    unsigned int buffer = *cols;
    *cols = *rows;
    *rows = buffer;
}

// Очищення пам'яті занятої матрицею
void free_matrix(double ***M, unsigned int rows)
{
    if(M == NULL || *M == NULL) return;

    for(unsigned int i = 0; i < rows; i++)
        free((*M)[i]);

    free(*M);
    *M = NULL;
}

// Виділення пам'яті під матрицю
void alloc_matrix(double ***M, unsigned int rows, unsigned int cols)
{
    *M = (double **)malloc(rows * sizeof(double *));

    for(unsigned int i = 0; i < rows; i++)
    {
        (*M)[i] = (double *)malloc(cols*sizeof(double));
    }
}

// Виводить матрицю в термінал
void print_matrix(double **M, unsigned int rows, unsigned int cols)
{
    if(rows == 0 || cols == 0) return;
    if(M == NULL)return;
    if(rows == 1)
    {
        printf("(");
        for(unsigned int i = 0; i < cols; i++)
        {
            if(M[0] == NULL)return;
            printf(" %7.2lf", M[0][i]);
            if(i != cols - 1)
                printf(",");
        }
        printf(" )\n");
    }
    else
    {
        for(unsigned int i = 0; i < rows; i++)
        {
            if(M[i] == NULL)return;
            if(i == 0)printf("/");
            else if (i == rows - 1) printf("\\");
            else printf("|");

            for(unsigned int j = 0; j < cols; j++)
            {
                printf(" %7.2lf", M[i][j]);
                if(j != cols - 1)
                    printf(",");
            }

            if(i == 0)printf(" \\\n");
            else if (i == rows - 1) printf(" /\n");
            else printf(" |\n");
        }
    }
}

// Знаходить середнє значення масиву цілих додатніхх чисел
double mean_value(const unsigned int * A, unsigned int Size)
{
    double sum = 0;
    for(unsigned int i = 0; i < Size; i++)
    {
        sum += (double)A[i];
    }
    return sum / (double) Size;
}

// Повертає випадкове число з плаваючою комою що належить проміжку [start, end]
double randomBetween(const int start, const int end)
{
    if(end < start) return 0.0 / 0.0;
    return (double)(rand() % abs(start * 100 - end * 100 + 1))/100 + (double)start;
}

// Знаходить мінімальне значення в масиві додатніх чисел
unsigned int find_min(const unsigned int * A, unsigned int Size)
{
    unsigned int buffer = UINT_MAX;
    for(unsigned int i = 0; i < Size; i++)
    {
        buffer = buffer > A[i] ? A[i] : buffer;
    }
    return buffer;
}

// Знаходить максимальне значення в масиві чисел з плаваючою комою
float find_max(const float *B, unsigned int Size)
{
    float buffer = Size != 0 ? B[0] : 0.0 / 0.0;
    for(unsigned int i = 0; i < Size; i++)
    {
        buffer = buffer < B[i] ? B[i] : buffer;
    }
    return buffer;
}

// Заповнює масив чисел з плаваючою комою згідно формули
void fill_float_array(float *B, unsigned int Size)
{
    for(unsigned int i = 0; i < Size; i++)
    {
        B[i] = 5.0 * i + 5.0;
    }
}

// Заповнює масив цілих додатніх чисел згідно формули
void fill_uint_array(unsigned int *A, unsigned int Size)
{
    for(unsigned int i = 0; i < Size; i++)
    {
        A[i] = Size - (i + 1);
    }
}

// Виводить масив цілих додатніх чисел в термінал
void print_uint_array(const unsigned int *A, unsigned int Size)
{

    printf("{");
    for(unsigned int i = 0; i < Size; i++)
    {
        printf("\n\t%u", A[i]);
        if(i == (Size - 1)) continue;
        printf(",");
    }
    printf("\n}\n");
}

// Виводить масив чисел з плаваючою комою в термінал
void print_float_array(const float *B, unsigned int Size)
{
    printf("{");
    for(unsigned int i = 0; i < Size; i++)
    {
        printf("\n\t%f", B[i]);
        if(i == (Size - 1)) continue;
        printf(",");
    }
    printf("\n}\n");
}


// Очікує Enter від користувача щоб продовжити
void pressEnterToContinue() {
    printf("\nНатисніть Enter щоб продовжити...");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    getchar();
}
