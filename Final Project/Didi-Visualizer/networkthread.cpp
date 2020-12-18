#include "networkthread.h"

networkThread::networkThread(QObject* parent) : QObject (parent)
{
}
networkThread::~networkThread()
{
}
void networkThread::start1()
{
    emit workStart();
    doWork();
}
void networkThread::doWork()
{
    QString url = QString("https://restapi.amap.com/v3/direction/driving?origin=")+QString::number(start_x,'f',6)+QString(",")+QString::number(start_y,'f',6)+
                    QString("&destination=")+QString::number(end_x,'f',6)+QString(",")+QString::number(end_y,'f',6)+
                    QString("&extensions=all&output=json&key=25824a502a0e2666ffe1b3744dbe8806");

    qDebug()<<url;

    auto netManager = new QNetworkAccessManager(this);
    QEventLoop loop;
    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(m_emit()));
    QNetworkRequest request;
    request.setUrl(url);
    QCoreApplication::processEvents();
    reply = netManager->get(request);
    loop.exec();
}

void networkThread::m_emit()
{
    if(reply->error()==QNetworkReply::NoError)
    {
        qDebug()<<"111";
        QByteArray resBytes = reply->readAll();
        //qDebug()<<"resBytes="<<QString(resBytes);
        QJsonDocument doc = QJsonDocument::fromJson(resBytes);
        QJsonObject json = doc.object();
        QJsonObject route= json.value("route").toObject();
        auto path = route.value("paths").toArray().at(0).toObject();
        //qDebug()<<path.value("distance");
        *duration = path.value("duration").toString();
        //qDebug()<<duration;
        auto steps = path.value("steps").toArray();
        //qDebug()<<steps.size();
        for(auto step : steps)
        {
            auto a = step.toObject();
            auto list = a.value("polyline").toString().split(';');
            if(a.value("road").toString()!="" && (road->size()==0||a.value("road").toString()!=road->at(road->size()-1)))
            {
                road->append(a.value("road").toString());
                m_list->addItem(road->at(road->size()-1));
            }
            for(auto l:list){
                //qDebug()<<l;
                auto p = l.split(',');
                R->append(std::make_pair(p[0].toDouble(),p[1].toDouble()));
            }
            //qDebug()<<list.size();
        }
        //qDebug()<<r;
        //qDebug()<<json;
    }
    else{
        qDebug()<<"network error";
    }
    emit workFinished();
}
