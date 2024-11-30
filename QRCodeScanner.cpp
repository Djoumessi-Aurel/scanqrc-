#include "QRCodeScanner.h"
#include <QLabel>
#include <QDir>
#include <QDebug>
#include <QDialog>
#include <QImage>
#include <QThread>
#include "zbarlink.h"

QRCodeScanner::QRCodeScanner(QWidget *parent)
    : QWidget(parent),
      camera(new QCamera(this)),
      viewfinder(new QCameraViewfinder(this)),
      resultArea(new QTextEdit("", this)),
      imageLabel(new QLabel(this)),
      layoutH(new QHBoxLayout()), // pour contenir layoutV et imageLabel
      scanButton(new QPushButton("Démarrer le scan", this)),
      camerasComboBox(new QComboBox(this)),
      isCameraActive(false) {

    fenetre = parent;

    resultArea->setAlignment(Qt::AlignCenter);
    resultArea->setStyleSheet("font-family: 'Book Antiqua'; font-size: 12pt;");
    resultArea->setReadOnly(true);

    populateCameraList();

    camera->setViewfinder(viewfinder);

    connect(scanButton, &QPushButton::clicked, this, &QRCodeScanner::toggleCamera);
    connect(camerasComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onCameraChanged(int)));

    imageCapture = new QCameraImageCapture(camera);
    connect(imageCapture, &QCameraImageCapture::imageCaptured,
            this, &QRCodeScanner::processImage);

    QHBoxLayout *cameraSelectionLayout = new QHBoxLayout();
    cameraSelectionLayout->addWidget(new QLabel("Sélectionner la caméra :"));
    cameraSelectionLayout->addWidget(camerasComboBox);
    QVBoxLayout *layout = new QVBoxLayout();
    QVBoxLayout *layoutV = new QVBoxLayout(); // pour contenir viewfinder et resultArea

    layout->addLayout(cameraSelectionLayout);
    layoutV->addWidget(viewfinder);
    layoutV->addWidget(resultArea);
    layoutH->addLayout(layoutV);
    //layoutH->addWidget(imageLabel);
    layout->addLayout(layoutH);
    layout->addWidget(scanButton);

    viewfinder->hide(); viewfinder->setMaximumWidth(800); resultArea->setMinimumWidth(600);
    imageLabel->hide();
    setLayout(layout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QRCodeScanner::captureAndDecode);
    timer->stop();

    notificationSound = new QMediaPlayer(this);
    QString filepath = QDir::currentPath();
    notificationSound->setMedia(QUrl::fromLocalFile(filepath + "/sounds/beep.wav"));
}

QRCodeScanner::~QRCodeScanner() {
}

void QRCodeScanner::captureAndDecode() {
    imageCapture->capture();
}

void QRCodeScanner::processImage(int id, const QImage& preview) {
    QImage normalImage = preview.mirrored(true, false); // Horizontal mirroring
    QString result = ZBarLink::scanQRCode(normalImage);

    if (!result.isEmpty()) {
        resultArea->setText(result);
        lastDecodedText = result;
        qDebug() << "QR Code détecté : " << result;

        notificationSound->play();

        toggleCamera();
        setMaximumWidth(1300);
        imageLabel->setPixmap(QPixmap::fromImage(normalImage));
        imageLabel->setAlignment(Qt::AlignCenter);
        layoutH->addWidget(imageLabel);
        imageLabel->show();
        //showDialog(normalImage);
    }
    else{
        setMaximumWidth(800);
    }
}

void QRCodeScanner::toggleCamera() {
    isCameraActive = !isCameraActive;

    if (isCameraActive) {

        fenetre->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        fenetre->resize(800, 600);
        fenetre->setFixedHeight(600);
        setMaximumWidth(800);
        camera->start();
        viewfinder->show();
        resultArea->hide();
        layoutH->removeWidget(imageLabel);
        imageLabel->hide();

        resultArea->setText("");
        scanButton->setText("Arrêter le scan");
        timer->start(300); //On capture l'image chaque 300 milliseconde
    } else {
        camera->stop();
        viewfinder->hide();
        scanButton->setText("Démarrer le scan");
        resultArea->show();
        timer->stop();
    }
}

void QRCodeScanner::populateCameraList() {
    QList<QByteArray> cameras = QCamera::availableDevices();

    camerasComboBox->clear();
    for (int i = 0; i < cameras.size(); ++i) {
        camerasComboBox->addItem(QString("Caméra %1").arg(i+1), cameras[i]);
    }
}

void QRCodeScanner::onCameraChanged(int index) {
    if (camera) {
        camera->stop();
        delete camera;
        delete imageCapture;
    }

    camera = new QCamera(QCamera::availableDevices()[index]);
    //qDebug() <<QString::fromUtf8(QCamera::availableDevices()[index])<<endl;
    camera->setViewfinder(viewfinder);

    imageCapture = new QCameraImageCapture(camera);
    connect(imageCapture, &QCameraImageCapture::imageCaptured,
            this, &QRCodeScanner::processImage);

    if (isCameraActive) {
        camera->start();
    }
}


void QRCodeScanner::showDialog(const QImage &image) {
    // Créer un QDialog
    QDialog dialog;

    // Configurer le QLabel pour afficher l'image
    QLabel *imageLabel = new QLabel(&dialog);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    imageLabel->setAlignment(Qt::AlignCenter);  // Centrer l'image

    // Ajouter le QLabel dans un layout
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(imageLabel);

    // Configurer le QDialog
    dialog.setLayout(layout);
    dialog.setWindowTitle("Affichage de l'image");
    dialog.resize(800, 600);  // Taille initiale

    // Afficher le QDialog en modal
    dialog.exec();
}
