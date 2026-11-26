#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

#define LCL 35.00        // нижняя граница контроля
#define X_MIN 0.0        // минимальное показание датчика
#define X_MAX 21.0       // максимальное показание датчика
#define X_BREAK1 2.0     // граница диапазона 1
#define X_BREAK2 8.0     // граница диапазона 2
#define COEF1_F1 4.0     // коэф. 1 формулы 1
#define COEF2_F1 15.0    // коэф. 2 формулы 1
#define COEF1_F2 5.0     // коэф. 1 формулы 2
#define COEF2_F2 45.0    // коэф. 2 формулы 2
#define COEF1_F3 2.0     // коэф. 1 формулы 3
#define COEF2_F3 8.0     // коэф. 2 формулы 3
#define COEF3_F3 60.0    // коэф. 3 формулы 3

#define RC_OK        0
#define RC_RANGE     1   
#define RC_LOG_DOM   2   

// проверка диапазона х
int input_x(double x);

// преобразование х
int convert_x_to_q(double x, double* q);

// контроль по нижней границе LCL
int check_state(double q);

// среднее первой и второй половины интервала и их разность
void calc_halves_stats(const double q_values[], int n);

// будущая доп функция
void extra_function_stub(void);


int main(void)
{
    setlocale(LC_ALL, ".UTF8");

    double x;        // показание уровня кислорода 
    double q;        // преобразованное значение уровня кислорода 
    int    state;    // 0 - НОРМА, 1 - ТРЕВОГА 
    int    rc;

    printf(" Добро пожаловать в систему мониторинга кислорода! \n\n");
    printf("Определение уровня расчитывается по трем динамичным формулам. \n\n");
    printf("Расчет Q = f(X), контроль по LCL = %.2f \n\n", LCL);
    printf("В данном этапе: Q - преобразованное значение параметра кислорода, Х - параметр кислорода, измеряемый в процентах.\n\n");
    printf("Если при преобразование уровня кислорода его значение ниже LCL, то в системе приходит оповещение о тревоге \n\n");
    printf("Допустимый диапозон Х: от %.2f до %.2f\n\n", X_MIN, X_MAX);
    printf("*********************************************************************************************************************\n\n");

    //  ввод х 
    printf("Введите X  : ");
    scanf("%lf", &x);

    // проверка диапазона х
    rc = input_x(x);
    if (rc == RC_RANGE)
    {
        printf("Ошибка: X вне диапазона %.1f...%.1f\n\n", X_MIN, X_MAX);
        printf("Состояние программы: завершена (ошибка ввода)\n");
        return 1;
    }

    // преобразование х
    rc = convert_x_to_q(x, &q);
    if (rc == RC_LOG_DOM)
    {
        printf("Ошибка: при X <= 0 логарифм не определен\n\n");
        printf("Состояние программы: завершена (ошибка ввода)\n");
        return 1;
    }

    // контроль и вывод состояния 
    state = check_state(q);

    printf("X = %.2f   Q = %.2f   Состояние: ", x, q);
    switch (state)
    {
    case 0:
        printf("НОРМА\n\n");
        break;
    case 1:
        printf("ТРЕВОГА\n\n");
        break;
    default:
        printf("НЕИЗВЕСТНО\n\n");
        break;
    }

    printf("Статистика по Q[1..16] \n");
    calc_halves_stats(NULL, 0);

    printf("Дополнительная функция \n");
    extra_function_stub();

    printf("Состояние программы: завершена (успех выполнения) \n");
    return 0;
}

int input_x(double x)
{
    if (x < X_MIN || x > X_MAX)
    {
        return RC_RANGE;
    }
    return RC_OK;
}

int convert_x_to_q(double x, double* q)
{
    if (x < X_BREAK1)
    {
        if (x <= 0.0)
        {
            return RC_LOG_DOM;
        }
        *q = COEF1_F1 * log(x) + COEF2_F1;
    }
    else if (x <= X_BREAK2)
    {
        *q = (x - COEF1_F2) * (x - COEF1_F2) + COEF2_F2;
    }
    else
    {
        *q = COEF1_F3 * (x - COEF2_F3) + COEF3_F3;
    }
    return RC_OK;
}

int check_state(double q)
{
    return (q < LCL) ? 1 : 0;
}

void calc_halves_stats(const double q_values[], int n)
{
    if (q_values == NULL || n <= 1)
    {
        printf("Массив Q[1..16] пока не передан (функция-заглушка).\n\n");
        return;
    }

    int half = n / 2;
    double sum1 = 0.0;
    double sum2 = 0.0;

    for (int i = 0; i < half; i++)
    {
        sum1 += q_values[i];
    }
    for (int i = half; i < n; i++)
    {
        sum2 += q_values[i];
    }

    double avg1 = sum1 / half;
    double avg2 = sum2 / (n - half);
    double diff = avg1 - avg2;

    printf("Среднее первой половины: %.2f\n", avg1);
    printf("Среднее второй половины: %.2f\n", avg2);
    printf("Разность средних:        %.2f\n\n", diff);
}

void extra_function_stub(void)
{
    printf("Дополнительная функция находится в разработке (заглушка).\n\n");
}
