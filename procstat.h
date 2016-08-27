#ifndef PROCSTAT_H
#define PROCSTAT_H

#include <QVector>
#include <QRegularExpression>
#include <QJsonArray>

class ProcStat
{
public:
    ProcStat();
    QJsonArray getStats();
private:
    QVector< QVector< int > > jiffies;
    QRegularExpression re;
};

#endif // PROCSTAT_H
