#include <filethread.h>
void fileThread::changeFlag()
{
    flag = !flag;
}

void fileThread::run()
{
    QDir fileDir(dir);
    QFileInfoList list = fileDir.entryInfoList(QStringList() << "*.csv");
    emit(readAll(list.size()-1));

    QString Dir = dir+QString("\\")+"rectangle_grid_table.csv";
    QFile _file(Dir);
    _file.open(QIODevice::ReadOnly);
    auto line = _file.readLine();
    while(!_file.atEnd()){

        if(flag){
               line = _file.readLine();
               QString str = QString(line);
               grid->push_back(Grid(str));
        }
        else{
            _file.close();
            return;
        }
    }

    //qDebug()<<grid->at(0).lng[2]<<""<<grid->at(99).lng[0]<<""<<grid->at(0).lat[0]<<""<<grid->at(99).lat[1];

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
             counterInThread++;
             emit changed(counterInThread);
             auto line = file.readLine();
             while(!file.atEnd()){
                 if(flag){
                    if(list.at(i).fileName()=="rectangle_grid_table.csv"){
                        break;
                    }
                    else{
                        line = file.readLine();
                        //qDebug() <<counterInThread;
                        QString str = QString(line);
                        data->push_back(dataInfo(str,grid->at(0).lat[0],grid->at(99).lat[1],grid->at(0).lng[2],grid->at(99).lng[0]));
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
