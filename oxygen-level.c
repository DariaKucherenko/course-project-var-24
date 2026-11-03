#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <locale.h>
#include <math.h>


#define LCL 35.00        // нижняя граница контроля 
#define X_MIN 0.0        // минимальное показание датчика 
#define X_MAX 21.0      // максимальное показание датчика 
#define X_BREAK1 2.0    //граница диапозона 1
#define X_BREAK2 8.0    //граница диапозона 2
#define COEF1_F1 4.0    //коэф. 1 формулы 1
#define COEF2_F1 15.0    //коэф. 2 формулы 1
#define COEF1_F2 5.0    //коэф. 1 формулы 2
#define COEF2_F2 45.0    //коэф. 2 формулы 2
#define COEF1_F3 2.0    //коэф. 1 формулы 3
#define COEF2_F3 8.0    //коэф. 2 формулы 3
#define COEF3_F3 60.0    //коэф. 3 формулы 3



//ввод и проверка диапазона х
int input_x(double* x);

//преобразование X в Q по кусочной формуле. возвращает 0 если успех (q записывается), 1  ошибка 
int convert_x_to_q(double x, double* q);

//контроль по нижней границе LCL и определение состояния. возвращает 0 - НОРМА, 1 — ТРЕВОГА 
int check_state(double q);

// вывод состояния реактора по коду state
void print_state(int state);



int main(void)
{
    setlocale(LC_ALL, ".UTF8");

    double x;        /* показание уровня кислорода */
    double q;        /* преобразованное значение уровня кислорода */
    int    state;    /* 0 - НОРМА, 1 - ТРЕВОГА */

    printf(" Добро пожаловать в систему мониторинга кислорода! \n\n");
    printf("Определение уровня расчитывается по трем динамичным формулам. \n\n");
    printf("Этап 1. Расчет Q = f(X), контроль по LCL = %.2f \n\n", LCL);
    printf("В данном этапе: Q - преобразованное значение параметра кислорода, Х - параметр кислорода, измеряемый в процентах.\n\n");
    printf("Если при преобразование уровня кислорода его значение ниже LCL, то в системе приходит оповещение о тревоге \n\n");
    printf("Допустимый диапозон Х: от %.2f до %.2f\n\n", X_MIN, X_MAX);
    printf("*********************************************************************************************************************\n\n");


    if (input_x(&x) != 0)
    {
        return 1;
    }

    if (convert_x_to_q(x, &q) != 0)
    {
        return 1;
    }

    state = check_state(q);

    printf("X = %.2f   Q = %.2f   Состояние: ", x, q);
    print_state(state);

    printf("Состояние программы: завершена (успех выполнения) \n");
    return 0;
}



int input_x(double* x)
{
    printf("Введите X  : ");
    scanf("%lf", x);

    /* проверка диапазона */
    if (*x < X_MIN || *x > X_MAX)
    {
        printf("Ошибка: X вне диапазона %.1f...%.1f\n\n", X_MIN, X_MAX);
        printf("Состояние программы: завершена (ошибка ввода)\n");
        return 1;
    }

    return 0;   // успех
}



int convert_x_to_q(double x, double* q)
{
    // преобразование Q = f(X) по формуле 
    if (x < X_BREAK1)
    {
        if (x <= 0.0)
        {
            printf("Ошибка: при X <= 0 логарифм не определен\n\n");
            printf("Состояние программы: завершена (ошибка ввода)\n");
            return 1;
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

    return 0;   // успех 
}



int check_state(double q)
{
    if (q < LCL)
    {
        return 1;   /* ТРЕВОГА */
    }
    else
    {
        return 0;   /* НОРМА */
    }
}


void print_state(int state)
{
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
}

