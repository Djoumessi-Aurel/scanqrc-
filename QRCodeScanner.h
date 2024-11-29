#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <QZXing.h>
#include <QMediaPlayer>

class QRCodeScanner : public QWidget {
    Q_OBJECT

public:
    explicit QRCodeScanner(QWidget *parent = nullptr);
    ~QRCodeScanner();

private slots:
    void captureAndDecode();
    void processImage(int id, const QImage& preview);
    void toggleCamera();
    void populateCameraList();
    void onCameraChanged(int index);

private:
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
    QTimer *timer;
    QZXing *zxing;
    QString lastDecodedText;
    QTextEdit *resultArea;
    QPushButton *scanButton;
    QComboBox *camerasComboBox;
    QMediaPlayer *notificationSound;
    bool isCameraActive;
};

#endif // QRCODESCANNER_H
