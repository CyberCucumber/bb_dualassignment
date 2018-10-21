#ifndef BBCLASS_H
#define BBCLASS_H

#include <QVector>
#include <fstream>
#include <QTime>

class BBClass
{
public:
    BBClass();

    //Задача задаётся 2-мя матрицами одинаковой размерности
    BBClass(const QVector<QVector<int> > &matrixA, const QVector<QVector<int> > &matrixB);

    //функция вычисления верхней границы
    int upperBoundary();

    //функция вычисления нижней границы
    int lowerBoundary();

    //функция возвращающая текущий рекорд
    int record();

    //функция устанавливающая текущий рекорд
    void setRecord(int rec);

    //основная вычисляющая функция (рекурсивная)
    void branchCalc(int cBoundary = 0);

    static uint64_t iterations; //количество итераций (считаются криво)

    //функция определения затраченного времени на вычисления
    int workingTime();


private:
    QVector<QVector<int> > matrixA; //матрица первых заданий
    QVector<QVector<int> > matrixB; //матрица вторых заданий
    int BBRecord; //Текущий рекорд (первая инициализация происходит в мейне)
    QTime startTime; //время начала вычисления

};

#endif // BBCLASS_H
