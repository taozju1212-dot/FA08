#include "SvgIcon.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QFile>
#include <QIcon>

QPixmap SvgIcon::pixmap(const QString &name, int size, const QColor &color)
{
    QString path = QString(":/icons/%1.svg").arg(name);
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        // Return blank pixmap if not found
        QPixmap px(size, size);
        px.fill(Qt::transparent);
        return px;
    }
    // Replace "currentColor" with the actual hex color so SVG renders correctly
    QByteArray data = f.readAll();
    data.replace("currentColor", color.name(QColor::HexRgb).toUtf8());

    QSvgRenderer renderer(data);
    QPixmap px(size, size);
    px.fill(Qt::transparent);
    QPainter painter(&px);
    renderer.render(&painter);
    return px;
}

QIcon SvgIcon::icon(const QString &name, int size, const QColor &color)
{
    return QIcon(pixmap(name, size, color));
}

// ── SvgIconLabel ──

SvgIconLabel::SvgIconLabel(const QString &name, int size, const QColor &color, QWidget *parent)
    : QLabel(parent)
{
    setIcon(name, size, color);
    setAttribute(Qt::WA_TranslucentBackground);
}

void SvgIconLabel::setIcon(const QString &name, int size, const QColor &color)
{
    setFixedSize(size, size);
    setPixmap(SvgIcon::pixmap(name, size, color));
}
