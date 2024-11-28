#ifndef VIDEOFRAMEPROCESSOR_H
#define VIDEOFRAMEPROCESSOR_H

#include <QAbstractVideoSurface>
#include <QVideoFrame>
#include <QImage>
#include <QObject>
#include <QZXing>

class VideoFrameProcessor : public QAbstractVideoSurface {
    Q_OBJECT

public:
    explicit VideoFrameProcessor(QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const override;
    bool present(const QVideoFrame &frame) override;

signals:
    void qrCodeDetected(const QString &text);

private:
    QZXing *decoder;
    QString lastDecodedText;
};

#endif // VIDEOFRAMEPROCESSOR_H
