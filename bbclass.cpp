#include "bbclass.h"

using namespace std;

uint64_t BBClass::iterations;

BBClass::BBClass()
{
    startTime = QTime::currentTime();
}

BBClass::BBClass(const QVector<QVector<int> > &matrixA, const QVector<QVector<int> > &matrixB)
{
    this->matrixA = matrixA;
    this->matrixB = matrixB;
    startTime = QTime::currentTime();
}

int BBClass::upperBoundary()
{
    int upperBoundary1 = 0;
    int n = matrixA.size();
    for(int i=0; i<n; i++)
        if(upperBoundary1 < matrixA[i][i] + matrixB[i][i])
            upperBoundary1 = matrixA[i][i] + matrixB[i][i];
    int upperBoundary2 = 0;
    for(int i=0; i<n; i++)
        if(upperBoundary2 < matrixA[i][n-i-1] + matrixB[i][i])
            upperBoundary2 = matrixA[i][n-i-1] + matrixB[i][i];
    int upperBoundary3 = 0;
    for(int i=0; i<n; i++)
        if(upperBoundary3 < matrixA[i][i] + matrixB[i][n-i-1])
            upperBoundary3 = matrixA[i][i] + matrixB[i][n-i-1];
    int upperBoundary4 = 0;
    for(int i=0; i<n; i++)
        if(upperBoundary4 < matrixA[n-i-1][n-i-1] + matrixB[n-i-1][n-i-1])
            upperBoundary4 = matrixA[n-i-1][n-i-1] + matrixB[n-i-1][n-i-1];
    return min(min(upperBoundary1, upperBoundary2), min(upperBoundary3, upperBoundary4));
}

int BBClass::lowerBoundary()
{
    int lowerBoundaryValue = 0;
    int n = matrixA.size();
    for(int i=0; i<n; i++)
    {
        int minRowA = matrixA[i][0];
        int minRowB = matrixB[i][0];
        for(int j=0; j<n; j++)
        {
            if(minRowA > matrixA[i][j])
                minRowA = matrixA[i][j];
            if(minRowB > matrixB[i][j])
                minRowB = matrixB[i][j];
        }
        if(lowerBoundaryValue < minRowA + minRowB)
            lowerBoundaryValue = minRowA + minRowB;
    }
    return lowerBoundaryValue;
}

int BBClass::record()
{
    return BBRecord;
}

void BBClass::setRecord(int rec)
{
    BBRecord = rec;
}

void BBClass::branchCalc(int cBoundary)
{
    ++iterations;
    int n = matrixA.size();
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            int currentBoundary = max(cBoundary, matrixA[0][i] + matrixB[0][j]);//назначаем работы
            //создаём копии исходных матриц
            QVector<QVector<int> > newMA = matrixA;
            QVector<QVector<int> > newMB = matrixB;

          //----убираем в обеих матрицах строки и столбцы относящиеся к назначенным работам
            newMA.remove(0);
            for(int k=0; k<n-1; k++)
                newMA[k].remove(i);
            newMB.remove(0);
            for(int k=0; k<n-1; k++)
                newMB[k].remove(j);
          //--------

            //заполняем данные для вычисления подзадачи (без назначенных работ)
            BBClass nInstance(newMA, newMB);
            int currentLowerBoundary = max(currentBoundary, nInstance.lowerBoundary());
            int currentUpperBoundary = max(currentBoundary, nInstance.upperBoundary());
//            qDebug() << "n = " << n << " i = " << i << " j = " << j << " (" << currentLowerBoundary <<\
//                        " " << currentUpperBoundary << ")";

            if(currentUpperBoundary < BBRecord) //если текущая верхняя граница больше рекорда,
                BBRecord = currentUpperBoundary; //то это новый рекорд

            if(currentLowerBoundary >= BBRecord || currentUpperBoundary <= currentLowerBoundary)
                continue; //если нижняя граница больше рекорда, то пропускаем эту ветку

            else
                nInstance.branchCalc(currentBoundary); //продолжаем расчёт для текущей ветки
        }
    //    qDebug() << "record" << record;
}

int BBClass::workingTime()
{
    return startTime.secsTo(QTime::currentTime());
}
