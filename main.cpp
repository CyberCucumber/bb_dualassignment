#include <QCoreApplication>
#include <QDebug>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <bbclass.h>
#include <QTime>

using namespace std;

// функция создаёт файл in.txt с матрицей случайных чисел размерности n*2n
void randomDualMatrix(int n)
{
    ofstream out("in.txt");
    out << n << endl;
    for(int i=0; i<2*n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            out << rand() % 100 << " ";
        }
    out << endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ofstream out("statistics.txt"); //логфайл
    srand(time(NULL)); //делаем srand один раз в программе

    //начинаем с задачи размености 25 исполнителей 2*25 работ
    for(int i=1000; i<2000; i++)
    {
        QTime startTime = QTime::currentTime(); //время начала выполнения работы
        randomDualMatrix(i/100 + 15); //генерируем файл исходных данных

      //----считываем исходные данные----
        int n;
        ifstream input("in.txt");
        input >> n; //считываем размерность
        //считываем первую матрицу размерности nxn
        QVector<QVector<int> > matrixA; //первая матрица
        matrixA.resize(n);
        for(int i=0; i<n; i++)
        {
            matrixA[i].resize(n);
            for(int j=0; j<n; j++)
                input >> matrixA[i][j];
        }
        //считываем вторую матрицу размерности nxn
        QVector<QVector<int> > matrixB; //вторая матрица
        matrixB.resize(n);
        for(int i=0; i<n; i++)
        {
            matrixB[i].resize(n);
            for(int j=0; j<n; j++)
                input >> matrixB[i][j];
        }
      //---------

        BBClass calc(matrixA, matrixB); //создаём вычисляющий класс, задаём его считанными матрицами
        BBClass::iterations = 0; //обнуляем счётчик итаераций

        //вычисляются начальные верхние и нижние границы для исходной задачи
        int currentUpperBoundary = calc.upperBoundary();
        int currentLowerBoundary = calc.lowerBoundary();
        qDebug() << currentUpperBoundary << currentLowerBoundary;
        //устанавливаем текущий рекорд
        calc.setRecord(currentUpperBoundary);

        //запускаем вычисляющую функцию
        calc.branchCalc();

        //вывод информации на экран и в логфайл
        qDebug() << "record" << calc.record()<<"time"<<calc.workingTime();
        qDebug() << "done";
        out <<  i/100 + 15  << " iterations = " << BBClass::iterations << endl;
        QTime endTime = QTime::currentTime();
        QTime outTime(0,0,0,0);
        outTime = outTime.addMSecs(startTime.msecsTo(endTime));
        out << "time = "<< startTime.secsTo(endTime) << " sec "  << outTime.toString("HH:mm:ss.zzz").toStdString()  << endl;
    }
    return a.exec();
}
