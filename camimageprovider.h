#ifndef CAMIMAGEPROVIDER_H
#define CAMIMAGEPROVIDER_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<qdir.h>
#include<unistd.h>
class CamImageProvider :  public QQuickImageProvider
{
    Q_OBJECT
public:
    Q_INVOKABLE QString jsonFile(QString fileName);
    Q_INVOKABLE QString name(void);
    CamImageProvider(QString,QObject *parent = nullptr);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    cv::Mat QImage2Mat(QImage const& src);
public slots:
    void updateImage(const QImage &image);
    void getGuiRecord(bool state);
    void setRecordFps(double);
signals:
    void imageChanged();

private:
    QImage image;
    double tFps=24;
    cv::VideoWriter recCap;
    int ptrVal=0;
    bool recordState=true;//false=>doit record true doit arreter
    QString m_imageName="image";//bien penser a donner le bon nom à l'instanciation!
};

#endif // CAMIMAGEPROVIDER_H
