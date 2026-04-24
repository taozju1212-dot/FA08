#pragma once
#include <QWidget>
#include <QPixmap>
#include <QColor>
#include <QString>

// Renders an SVG icon from :/icons/<name>.svg, tinted to the given color.
// Usage: SvgIcon::pixmap("play", 16, QColor("#fff"))
// Or as a label widget: new SvgIconLabel("play", 16, Qt::white, parent)

class SvgIcon {
public:
    // Returns a tinted pixmap. SVG paths with currentColor are replaced with the given color.
    static QPixmap pixmap(const QString &name, int size, const QColor &color = QColor("#1F2A37"));
    static QIcon   icon(const QString &name, int size, const QColor &color = QColor("#1F2A37"));
};

// Lightweight label that shows an SVG icon
#include <QLabel>
class SvgIconLabel : public QLabel {
    Q_OBJECT
public:
    explicit SvgIconLabel(const QString &name, int size = 16,
                          const QColor &color = QColor("#1F2A37"),
                          QWidget *parent = nullptr);
    void setIcon(const QString &name, int size, const QColor &color);
};
