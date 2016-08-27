#include "procstat.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QRegularExpression>
#include <QJsonArray>

ProcStat::ProcStat()
{
    cpuRe.setPattern("^(cpu)([0-9]*)");
    // TODO: ...
    jiffies.resize(4);
    jiffies[0].resize(2);
    jiffies[1].resize(2);
    jiffies[2].resize(2);
    jiffies[3].resize(2);
}

QJsonArray ProcStat::getStats()
{
    QJsonArray jsonData;
    QFile file("/proc/stat");
    file.open(QIODevice::ReadOnly);
    QByteArray contents = file.readAll();
    file.close();
    QTextStream in(&contents);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        QRegularExpressionMatch match = cpuRe.match(parts.at(0));
        if (match.hasMatch()) {
            if (!match.captured(2).isEmpty()) {
                int cpuNr = match.captured(2).toInt();
                int jiffTotal = 0;
                int jiffWork = 0;
                for (int i = 1; i <= 7; i++){
                    jiffTotal += parts.at(i).toInt();
                    if (i == 3) {
                        jiffWork = jiffTotal;
                    }
                }
                int usageTotal = jiffTotal - jiffies[cpuNr][0];
                int usageWork = jiffWork - jiffies[cpuNr][1];
                int usage = ((float)usageWork / (float)usageTotal) * 100;
                jsonData.append(usage);
                jiffies[cpuNr][0] = jiffTotal;
                jiffies[cpuNr][1] = jiffWork;
            }
        } else {
            break;
        }
    }
    return jsonData;
}
