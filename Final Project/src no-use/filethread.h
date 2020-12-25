#include <QThread>
#include <datainfo.h>
#include <grid.h>
#include <QDir>
#include <QFileInfoList>
#include <QVector>
#include <QDebug>

class fileThread:public QThread
{
    Q_OBJECT


public:
    QString dir;
    QVector<dataInfo> *data;
    QVector<Grid> *grid;
    bool flag = true;
    int counterInThread = 0;
private:

    void run() override;

signals:
    void changed(int);
    void readAll(int);
public slots:
    void changeFlag();
};
