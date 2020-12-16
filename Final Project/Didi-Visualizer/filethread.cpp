#include <filethread.h>
void fileThread::changeFlag()
{
    flag = !flag;
}

void fileThread::run()
{
    QDir fileDir(dir);
    QFileInfoList list = fileDir.entryInfoList(QStringList() << "*.csv");
    for (int i = 0; i < list.length(); i++)
    {
         //qDebug() << list.at(i).baseName();
         QString curDir = dir+QString("\\")+list.at(i).fileName();

         QFile file(curDir);
         if(!file.open(QIODevice::ReadOnly)){
             return;
         }
         else
         {
             auto line = file.readLine();
             while(!file.atEnd()){
                 if(flag){
                    if(list.at(i).fileName()=="rectangle_grid_table.csv"){
                        line = file.readLine();
                        QString str = QString(line);
                        grid->push_back(Grid(str));
                    }
                    else{
                        line = file.readLine();
                        //qDebug() <<counterInThread;
                        QString str = QString(line);
                        data->push_back(dataInfo(str));
                    }
                    counterInThread++;
                    if(counterInThread %1000==0)
                    {
                         emit changed(counterInThread);
                    }
                 }
                 else{
                     file.close();
                     return;
                 }
             }
         }
         file.close();
    }

}
