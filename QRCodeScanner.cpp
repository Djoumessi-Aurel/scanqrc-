#include "QRCodeScanner.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDir>
#include <QDebug>

QRCodeScanner::QRCodeScanner(QWidget *parent)
    : QWidget(parent),
      camera(new QCamera(this)),
      viewfinder(new QCameraViewfinder(this)),
      resultArea(new QTextEdit("", this)),
      scanButton(new QPushButton("Démarrer le scan", this)),
      camerasComboBox(new QComboBox(this)),
      isCameraActive(false) {

    resultArea->setAlignment(Qt::AlignCenter);
    resultArea->setStyleSheet("font-family: 'Book Antiqua'; font-size: 12pt;");
    resultArea->setReadOnly(true);

    // Combo box pour la sélection de la caméra
    populateCameraList();

    // Configurer la caméra
    camera->setViewfinder(viewfinder);  // Affichage de la caméra dans le viewfinder

    // Démarrage - Arrêt du scan
    connect(scanButton, &QPushButton::clicked, this, &QRCodeScanner::toggleCamera);

    // Changement de la caméra
    connect(camerasComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onCameraChanged(int)));


    // Capture d'image
    imageCapture = new QCameraImageCapture(camera);
    connect(imageCapture, &QCameraImageCapture::imageCaptured,
            this, &QRCodeScanner::processImage);

    /* Configurer l'interface graphique */

    // Layout pour la sélection de caméra
    QHBoxLayout *cameraSelectionLayout = new QHBoxLayout();
    cameraSelectionLayout->addWidget(new QLabel("Sélectionner la caméra :"));
    cameraSelectionLayout->addWidget(camerasComboBox);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Layout principal
    layout->addLayout(cameraSelectionLayout);
    layout->addWidget(viewfinder); // Afficher le viewfinder dans l'interface
    layout->addWidget(resultArea); // Afficher le résultat du QR code
    layout->addWidget(scanButton);

    // Cacher initialement le viewfinder
    viewfinder->hide();

    setLayout(layout);

    // Configuration de QZXing
    zxing = new QZXing(this);
    zxing->setDecoder(QZXing::DecoderFormat_QR_CODE);

    // Timer pour capturer et analyser régulièrement
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QRCodeScanner::captureAndDecode);
    timer->stop(); // Ne démarre pas immédiatement


    // Préparation du son de notification
    notificationSound = new QMediaPlayer(this);
    QString filepath = QDir::currentPath();
    qDebug() << "Chemin du dossier : " << filepath;
    notificationSound->setMedia(QUrl::fromLocalFile(filepath + "/sounds/beep.wav")); // Chemin à adapter selon le fichier .qrc
}


QRCodeScanner::~QRCodeScanner() {
    // Pas besoin de libérer manuellement les ressources car `QObject` gère les parents.
}

void QRCodeScanner::captureAndDecode() {
    imageCapture->capture();
}


void QRCodeScanner::processImage(int id, const QImage& preview) {
    // Créer une copie non constante de l'image
    QImage mutablePreview = preview;

    // Décodage du QR code
    QString result = zxing->decodeImage(mutablePreview);
    lastDecodedText = result;

    if (!result.isEmpty()) {
        resultArea->setText(result);
        qDebug() << "QR Code détecté : " << result;

        // Jouer le son de notification
        notificationSound->play();

        // Arrêter la caméra automatiquement
        toggleCamera();
    }
}


void QRCodeScanner::toggleCamera() {
    isCameraActive = !isCameraActive;

    if (isCameraActive) {
        // Démarrer la caméra
        camera->start();
        viewfinder->show();
        resultArea->hide();
        resultArea->setText("");
        scanButton->setText("Arrêter le scan");
        timer->start(200); // Démarrer le timer de scan
    } else {
        // Arrêter la caméra
        camera->stop();
        viewfinder->hide();
        scanButton->setText("Démarrer le scan");
        resultArea->show(); // if(!lastDecodedText.isEmpty()) resultArea->show();
        timer->stop();
    }
}

void QRCodeScanner::populateCameraList() {
    // Récupérer la liste des caméras disponibles
    QList<QByteArray> cameras = QCamera::availableDevices();

    camerasComboBox->clear();
    for (int i = 0; i < cameras.size(); ++i) {
        camerasComboBox->addItem(cameras[i], i);
    }
}


void QRCodeScanner::onCameraChanged(int index) {
    // Si une caméra était déjà active, l'arrêter
    if (camera) {
        camera->stop();
        delete camera;
        delete imageCapture;
    }

    // Créer une nouvelle caméra avec l'index sélectionné
    camera = new QCamera(QCamera::availableDevices()[index]);
    camera->setViewfinder(viewfinder);

    // Reconnection signal-slot pour la caméra
    imageCapture = new QCameraImageCapture(camera);
    connect(imageCapture, &QCameraImageCapture::imageCaptured,
            this, &QRCodeScanner::processImage);

    // Si la caméra était déjà active, la redémarrer
    if (isCameraActive) {
        camera->start();
    }
}
