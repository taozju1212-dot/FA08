#include "StatusPill.h"
#include <QPainter>
#include <QFontMetrics>

StatusPill::StatusPill(const QString &state, const QString &label, QWidget *parent)
    : QWidget(parent), m_state(state), m_label(label)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedHeight(20);
    QFont f = font();
    f.setPixelSize(11);
    f.setWeight(QFont::DemiBold);
    setFont(f);
    QFontMetrics fm(this->font());
    setFixedWidth(fm.horizontalAdvance(label) + 6 + 4 + 16); // dot + gap + padding
}

void StatusPill::setState(const QString &state, const QString &label)
{
    m_state = state;
    m_label = label;
    QFontMetrics fm(font());
    setFixedWidth(fm.horizontalAdvance(label) + 6 + 4 + 16);
    update();
}

QColor StatusPill::dotColor() const {
    if (m_state == "ok")   return QColor("#1E6B43");
    if (m_state == "warn") return QColor("#8A5A10");
    if (m_state == "err")  return QColor("#8E2A20");
    if (m_state == "run")  return QColor("#0F4E85");
    return QColor("#5B6B7B");
}
QColor StatusPill::bgColor() const {
    if (m_state == "ok")   return QColor("#E6F3EC");
    if (m_state == "warn") return QColor("#FBF1DF");
    if (m_state == "err")  return QColor("#FBE6E3");
    if (m_state == "run")  return QColor("#DCE8F2");
    return QColor("#E8EEF3");
}
QColor StatusPill::fgColor() const { return dotColor(); }
QColor StatusPill::borderColor() const {
    if (m_state == "ok")   return QColor("#BDDDC9");
    if (m_state == "warn") return QColor("#E7CFA1");
    if (m_state == "err")  return QColor("#EFC2BC");
    if (m_state == "run")  return QColor("#B4CCE1");
    return QColor("#D6DEE6");
}

void StatusPill::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    // Background capsule
    p.setPen(borderColor());
    p.setBrush(bgColor());
    p.drawRoundedRect(r.adjusted(0,0,-1,-1), 10, 10);

    // Dot
    p.setBrush(dotColor());
    p.setPen(Qt::NoPen);
    int dotX = 8;
    int dotY = r.height() / 2;
    p.drawEllipse(QPoint(dotX, dotY), 3, 3);

    // Text
    p.setPen(fgColor());
    p.setFont(font());
    QRect textRect(dotX + 3 + 4, 0, r.width() - dotX - 10, r.height());
    p.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_label);
}
