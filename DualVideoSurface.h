#ifndef DUALVIDEOSURFACE_H
#define DUALVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QCameraViewfinder>
#include <QVideoFrame>
#include "VideoFrameProcessor.h"

class DualVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    // Constructeur qui prend en paramètre un viewfinder et un frameProcessor
    explicit DualVideoSurface(QCameraViewfinder *viewfinder, VideoFrameProcessor *processor, QObject *parent = nullptr);

    // Méthode pour recevoir et traiter les frames vidéo
    bool present(const QVideoFrame &frame) override;

private:
    QCameraViewfinder *m_viewfinder;  // Pour afficher l'image
    VideoFrameProcessor *m_processor; // Pour traiter l'image
};

#endif // DUALVIDEOSURFACE_H
