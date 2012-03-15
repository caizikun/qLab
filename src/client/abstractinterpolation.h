#ifndef ABSTRACTINTERPOLATION_H
#define ABSTRACTINTERPOLATION_H


//размер массивов точек данных
#define N_T 651
//количество базисных функций
#define M_T 10


#include <QVector>
#include <QPointF>

//mathematical functions from C language (pow,exp...)
#include <cmath>

class AbstractInterpolation
{
public:
    AbstractInterpolation();
    virtual void interpolate(const QVector<double> & xValue,
                             const QVector<double> & yValue

                             )=0;


    //FIXME: this function should be moved to appropriate subclass
    void Line_Reg(int valcount,int *val,double *XD,double *YD,double *k,double *b0,double *xs,double *ys,double *xmin,double *xmax);

    void lineApproximation (const QVector<double> &x,
                            const QVector<double> &y,
                            double &k,
                            double &b0,
                            double &avgX,
                            double &avgY,
                            double &minX,
                            double &maxX
                            );

protected:
    /** функция вычисляет значение S полинома степени М в точке Х1
    X1 - значение аргумента
    С - массив коэффициентов
    */
    static double Bas_Poly(int M, double X1,double *C);

    /** функция вычисляет значение производной S полинома степени М в точке Х1
    X1 - значение аргумента
    С - массив коэффициентов
    */
    static double Bas_Poly_h(int M,double X1,double *C);

    /**
    возвращает значение базисные функции,
    N - размер входных массивов
    M - номер функции (c нуля)
    X1 - значение аргумента
    X - массив аргументов
    T - массив значений (выход)
    */
    static double Bas_Exp(int M,double X0,double X1,double c,double *CH);

    /** базисные функции,
    N - размер входных массивов
    M - номер функции (c нуля)
    X1 - значение аргумента
    X - массив аргументов
    T - массив значений (выход)
    */
    static void Bas(int N,int M,int Poly,double X1,double c,double *X,double *T);

    /** заполнение матрицы Грамма
    N - размер входных массивов
    M - количество функций (с нуля)
    X - массив абсцисс входных точек
    F - массив ординат входных точек
    A - результирующая матрица
    */
    static void Gram(int N,int M,int Poly,double c,double *X,double *F,long double A[M_T][M_T]);

    //решение СЛАУ методом Гаусса
    static void Gauss(int N,double *X,long double A[M_T][M_T]);

    /** аппроксимирующая функция
    возвращает значение производной функции в точке X1 для полинома
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi_h(int N,int M,int Poly,int *val,double X0,double c_k,double X1,double *C,double *XData);

    /** аппроксимирующая функция
    возвращает значение производной функции в точке X1 для полинома
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi_h_Exp(int M,double c_k,double X1,double *C);

    /** аппроксимирующая функция
    возвращает значение функции в точке X1 для полинома
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi_Exp(int M,double c_k,double X1,double *C);

    /** аппроксимирующая функция
    возвращает значение функции в точке X1
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi(int N,int M,int Poly,int *val,double X0,double c_k,double X1,double *C,double *XData);

    /** вызов расчета аппроксимации функции методом
     наименьших квадратов с
    произвольным базисом
     c_k - коэфициент экспоненты
     X,Y - массивы входных точек
    C - выходной массив коэфициентов
    */
    static void CalcMNK(int N,int M,int Poly,int *val,double X0,double c_k,double *XData,double *YData,double *C,bool *error);

    /** возвращает значение суммы среднеквадратичных отклонений
    аппроксимирующей функции в точках XData относительно
    значений YData */
    static double Skvo(int N,int M,int Poly,int *val,double X0,double c_k,double *C,double *XData,double *YData);

    /** ВОЗВРАЩАЕТ оптимизированное значение
    коэфициента экспоненты для
    расчета аппроксимации функции методом
     наименьших квадратов с
    произвольным базисом
     c_k_Start - стартовое значение коэфициента экспоненты
     c_k_End - конечное значение коэфициента экспоненты
     XData,YData - массивы входных точек
    C - выходной массив коэфициентов */
    static double CalcMNK_opt(int N,int M,int Poly,int *val,double X0,double c_k_Start,double c_k_End,double *XData,double *YData,double *C,int *error_TH,bool *error,int col_h);



};

#endif // ABSTRACTINTERPOLATION_H
