#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
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
    void showDialog(const QImage &image);

private:
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
    QTimer *timer;
    QString lastDecodedText;
    QTextEdit *resultArea;
    QPushButton *scanButton;
    QComboBox *camerasComboBox;
    QMediaPlayer *notificationSound;
    bool isCameraActive;

};

#endif // QRCODESCANNER_H
