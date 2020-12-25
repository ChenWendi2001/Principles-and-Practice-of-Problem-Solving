#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H
#include <QNetworkAccessManager>
#include <QNetWorkReply>
#include <QJsonDocument>
#include <QString>
#include <QJsonObject>
#include <QEventLoop>
#include <QJsonArray>
#include <QCoreApplication>
#include <QListWidget>
#include <QMutex>

class networkThread : public QObject
{
    Q_OBJECT
public:
    networkThread(QObject* parent = nullptr);
    double start_x,start_y,end_x,end_y;
    QNetworkReply *reply;
    QVector<std::pair<double,double>> *R;
    QVector<QString> *road;
    QString *duration;
    QListWidget *m_list;
    ~networkThread();
public slots:
    void start1();
    void doWork();
    void m_emit();
signals:
    void workFinished();
    void workStart();
};


#endif // NETWORKTHREAD_H
