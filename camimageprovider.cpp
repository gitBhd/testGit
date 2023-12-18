#include "camimageprovider.h"
#include<iostream>
#include<QFile>
CamImageProvider::CamImageProvider(QString passedName,QObject *parent) : QQuickImageProvider(QQuickImageProvider::Image)
{
    m_imageName=passedName;
    image = QImage(1280,720,QImage::Format_RGB32);
    image.fill(QColor("black"));
    //image.load("waitImg.JPG");
}

QString CamImageProvider::jsonFile(QString fileName)
{
  //  QString fileName="config_sat2.json";
    QFile loadFile(fileName);
    QByteArray fileData;
    if(!loadFile.open(QIODevice::ReadOnly|  QFile::Text))
    {
        qDebug()<<"error open";
    }
    else
    {
        fileData=loadFile.readAll();
        loadFile.close();

    }
    //qDebug()<<fileData;
    return QString(fileData);

}

QString CamImageProvider::name()
{

return m_imageName;
}

cv::Mat CamImageProvider::QImage2Mat(QImage const& src)
{
    cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result; //deep copy just in case (my lack of knowledge with open cv)
    cvtColor(tmp, result,cv::COLOR_BGR2RGB);
    return result;
}
 QImage CamImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
 /*   Q_UNUSED(id);

    if(size){
        *size = image.size();

    }
   // std::cout<<image.width()<<" "<<image.height()<<std::endl;
    if(requestedSize.width() > 0 && requestedSize.height() > 0) {
        image = image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
    }*/
    return image;
}

void CamImageProvider::updateImage(const QImage &image)
{
    //qDebug()<<"recept emit'";

    if(!image.isNull() && this->image != image) {
       // std::cout<<"provider refresh"<<std::endl;
        this->image = image;
        if(recordState==true)
        {
            //frame_threshold=cv::Mat::zeros(h,w,CV_8UC1);
            cv::Mat img(720,1280,CV_8UC3,cv::Scalar(128,127,200) );
            img=QImage2Mat(image);
            recCap<<QImage2Mat(image);
        }
        emit imageChanged();
    }


}

void CamImageProvider::getGuiRecord(bool onoff)
{

    //double tFps=24;
    recordState=!onoff;
    qDebug()<<"state:"<<recordState;
    if(recordState==0)
    {
        usleep(1000*500);

        recCap.release();
        qDebug()<<("Referme video");
    }
    if(recordState==1)
    {
        //double tFps=m_fpsSat/100;//getVideoFps();
        if (tFps==0)tFps=30;
        printf("\r\nDemarre enregistrement...");
        QDateTime now = QDateTime::currentDateTime();
        QString date=now.toString();
        int d=now.date().day();
        int m=now.date().month();
        int y=now.date().year();
        int hr=now.time().hour();
        int mn=now.time().minute();
        int  s=now.time().second();
        QString mdate="d"+QString::number(d)+QString::number(m)+QString::number(y)+"t"+QString::number(hr)+QString::number(mn)+QString::number(s);

        QString recFileName = QDir::currentPath() + "/outVid"+mdate+"_"+QString::number(ptrVal) +".avi";
        qDebug()<<"...rec dans"+recFileName+"@ fps="+ QString::number(tFps);
        int writterCompValue=50;
        recCap.set(cv::VIDEOWRITER_PROP_QUALITY ,static_cast<int>(writterCompValue) );
        int w=1280;int h=720;//setRecordingSize(&h,&w);
        //if(h==0)h=240;        if(w==0)h=320;
        recCap.open(recFileName.toStdString(), cv::VideoWriter::fourcc('x', '2', '6', '4' ),
                    tFps,
                    cv::Size(w,h), true);
        recCap.set(cv::VIDEOWRITER_PROP_QUALITY ,static_cast<int>(writterCompValue) );
                qDebug()<<"...open ok.";
        ptrVal++;
        if(ptrVal==100)ptrVal=0;
    }
}

void CamImageProvider::setRecordFps(double f)
{
tFps=f;
}
