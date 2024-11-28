#include "VideoFrameProcessor.h"

VideoFrameProcessor::VideoFrameProcessor(QObject *parent)
    : QAbstractVideoSurface(parent), decoder(new QZXing(this)) {
    lastDecodedText = "";
}

QList<QVideoFrame::PixelFormat> VideoFrameProcessor::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const {
    Q_UNUSED(handleType);
    return {QVideoFrame::Format_ARGB32, QVideoFrame::Format_RGB32, QVideoFrame::Format_RGB24};
}

bool VideoFrameProcessor::present(const QVideoFrame &frame) {
    if (!frame.isValid())
        return false;

    QVideoFrame copyFrame(frame);
    copyFrame.map(QAbstractVideoBuffer::ReadOnly);

    QImage image(
        copyFrame.bits(),
        copyFrame.width(),
        copyFrame.height(),
        QVideoFrame::imageFormatFromPixelFormat(copyFrame.pixelFormat())
    );

    if (!image.isNull()) {
        QString decodedText = decoder->decodeImage(image);

        if (!decodedText.isEmpty() && decodedText != lastDecodedText) {
            lastDecodedText = decodedText;
            emit qrCodeDetected(decodedText);
        }
    }

    copyFrame.unmap();
    return true;
}
