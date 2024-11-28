#include "DualVideoSurface.h"

DualVideoSurface::DualVideoSurface(QCameraViewfinder *viewfinder, VideoFrameProcessor *processor, QObject *parent)
    : QAbstractVideoSurface(parent), m_viewfinder(viewfinder), m_processor(processor) {}


bool DualVideoSurface::present(const QVideoFrame &frame)
{
    // Transmettre la frame au Viewfinder pour affichage
    if (m_viewfinder) {
        m_viewfinder->present(frame);
    }

    // Traiter la frame avec VideoFrameProcessor
    if (m_processor) {
        m_processor->present(frame);
    }

    return true;
}
