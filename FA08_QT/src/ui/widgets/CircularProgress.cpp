#include "CircularProgress.h"
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <cmath>

CircularProgress::CircularProgress(QWidget *parent) : QWidget(parent)
{
    setFixedSize(46, 46);
    setAttribute(Qt::WA_TranslucentBackground);
}

void CircularProgress::setData(const QString &name, int qty, int capacity, bool loaded)
{
    m_name     = name;
    m_qty      = qty;
    m_capacity = capacity > 0 ? capacity : 1;
    m_loaded   = loaded;
    update();
}

CircularProgress::State CircularProgress::state() const
{
    if (!m_loaded) return Unloaded;
    double pct = (double)m_qty / m_capacity;
    if (pct < 0.12) return Crit;
    if (pct < 0.25) return Warn;
    return Ok;
}

QColor CircularProgress::arcColor() const
{
    switch (state()) {
    case Unloaded: return QColor("#C5CCD4");
    case Crit:     return QColor("#E0574B");
    case Warn:     return QColor("#E8A33D");
    case Ok:       return QColor("#4CAF50");
    }
    return QColor("#4CAF50");
}

void CircularProgress::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width(), h = height();
    // Leave 3px margin for label below
    int margin = 4;
    int diameter = qMin(w, h - 14) - margin * 2;
    if (diameter < 10) diameter = 10;
    QRectF arcRect(
        (w - diameter) / 2.0,
        margin,
        diameter, diameter
    );

    QPen trackPen;
    trackPen.setWidthF(2.5);
    trackPen.setCapStyle(Qt::RoundCap);

    // Track ring
    if (m_loaded) {
        trackPen.setColor(QColor("#E6EAEF"));
        trackPen.setStyle(Qt::SolidLine);
    } else {
        trackPen.setColor(QColor("#EEF0F3"));
        trackPen.setStyle(Qt::SolidLine);
    }
    p.setPen(trackPen);
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(arcRect);

    // Progress arc (or dashed if unloaded)
    State s = state();
    if (s == Unloaded) {
        QPen dashPen(QColor("#C5CCD4"), 1.0, Qt::DashLine);
        dashPen.setDashPattern({2, 2.5});
        p.setPen(dashPen);
        p.drawEllipse(arcRect);
    } else {
        double pct = qBound(0.0, (double)m_qty / m_capacity, 1.0);
        if (pct > 0) {
            QPen arcPen(arcColor(), 2.5, Qt::SolidLine, Qt::RoundCap);
            p.setPen(arcPen);
            // Qt drawArc: angles in 1/16 degree, start from 12 o'clock = 90*16, span CW = negative
            int startAngle = 90 * 16;
            int spanAngle  = -qRound(pct * 360.0 * 16);
            p.drawArc(arcRect, startAngle, spanAngle);
        }
    }

    // Center dot or icon
    QPointF center(w / 2.0, arcRect.center().y());
    if (s == Crit) {
        // Exclamation mark
        QPen ep(arcColor(), 1.8, Qt::SolidLine, Qt::RoundCap);
        p.setPen(ep);
        p.drawLine(QPointF(center.x(), center.y() - 4), QPointF(center.x(), center.y() + 1));
        p.setBrush(arcColor());
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(center.x(), center.y() + 3.5), 1.0, 1.0);
    } else if (s == Unloaded) {
        // Dash
        p.setPen(QPen(QColor("#9AA4B0"), 1.4));
        p.drawLine(QPointF(center.x() - 4, center.y()), QPointF(center.x() + 4, center.y()));
    } else {
        // Small dot
        p.setBrush(arcColor());
        p.setPen(Qt::NoPen);
        p.drawEllipse(center, 2.0, 2.0);
    }

    // Name label at bottom
    QFont f = font();
    f.setPixelSize(9);
    f.setWeight(QFont::Bold);
    p.setFont(f);
    p.setPen(m_loaded ? QColor("#334155") : QColor("#7A8898"));
    QRectF labelRect(0, h - 13, w, 13);
    p.drawText(labelRect, Qt::AlignCenter, m_name);
}
