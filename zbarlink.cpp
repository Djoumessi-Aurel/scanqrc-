#include "zbarlink.h"

ZBarLink::ZBarLink()
{
}

QString ZBarLink::scanQRCode(const QImage& image) {
    // Lire le chemin de zbarimg depuis le fichier
    QString zbarPath;
    QFile pathFile("./config/zbar.txt");
    if (pathFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pathFile);
        zbarPath = in.readLine().trimmed();
        pathFile.close();

        // Vérifier que le chemin n'est pas vide
        if (zbarPath.isEmpty()) {
            qDebug() << "Chemin zbarimg vide dans le fichier zbar.txt";
            return QString();
        }
    } else {
        qDebug() << "Impossible d'ouvrir zbar.txt";
        return QString();
    }

    // Créer un fichier temporaire pour sauvegarder l'image
    QTemporaryFile tempFile(QDir::tempPath() + "/qrcode_XXXXXX.png");

    // Ouvrir le fichier temporaire
    if (!tempFile.open()) {
        qDebug() << "Impossible de créer le fichier temporaire";
        return QString();
    }

    // Sauvegarder l'image dans le fichier temporaire
    if (!image.save(tempFile.fileName(), "PNG")) {
        qDebug() << "Impossible de sauvegarder l'image";
        return QString();
    }

    // Fermer le fichier
    tempFile.close();

    // Préparer le processus ZBar
    QProcess process;
    QStringList arguments;
    arguments << "--quiet"        // Mode silencieux
             << tempFile.fileName();

    // Lancer le processus zbarimg avec le chemin complet
    process.start(zbarPath, arguments);

    // Attendre que le processus se termine
    if (!process.waitForFinished(1000)) { // Timeout de 150 millisecondes
        qDebug() << "Délai dépassé lors du scan du QR code";
        return QString();
    }

    // Vérifier le code de sortie
    if (process.exitCode() != 0) {
        //qDebug() << "Erreur de lecture du QR code:" << process.errorString() << "*--*" << QString::fromUtf8(process.readAllStandardOutput());
        return QString();
    }

    // Lire la sortie standard
    QString output = QString::fromUtf8(process.readAllStandardOutput());

    // Extraire le texte (enlever le préfixe "QR-Code:")
    if (output.startsWith("QR-Code:")) {
        output = output.mid(8).trimmed();
    }

    return output;
}
