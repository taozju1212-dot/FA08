#include "SlotCard.h"
#include "ui/widgets/SvgIcon.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>

SlotCard::SlotCard(int index, QWidget *parent)
    : QWidget(parent), m_index(index)
{
    setFixedHeight(64);
    setCursor(Qt::PointingHandCursor);

    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setContentsMargins(10, 0, 14, 0);
    hl->setSpacing(8);

    // Checkbox
    m_check = new QCheckBox(this);
    m_check->setFixedSize(16, 16);
    m_check->setStyleSheet("QCheckBox::indicator { width:14px; height:14px; }");
    connect(m_check, &QCheckBox::toggled, this, [this](bool checked){
        emit checkChanged(m_index, checked);
    });
    hl->addWidget(m_check);

    // Icon
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(20, 20);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    hl->addWidget(m_iconLabel);

    // Name + slot id
    m_nameLabel = new QLabel(this);
    m_nameLabel->setStyleSheet("font-size:12px; font-weight:600; color:#1F2A37; background:transparent;");
    hl->addWidget(m_nameLabel, 1);

    // Status text (right-aligned)
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("font-size:11px; color:#5B6B7B; background:transparent;");
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hl->addWidget(m_statusLabel);

    // Init with empty slot
    SampleSlot empty;
    empty.id     = QString("S%1").arg(index + 1);
    empty.status = SampleStatus::Empty;
    setSlot(empty);
}

// ── Colors ──────────────────────────────────────────────────

QColor SlotCard::accentColor() const
{
    switch (m_slot.status) {
        case SampleStatus::Scheduled: return QColor("#1464A5");
        case SampleStatus::Running:   return QColor("#1E78BE");
        case SampleStatus::Done:      return QColor("#2E8B57");
        case SampleStatus::Error:     return QColor("#C0392B");
        default:                      return QColor("#D6DEE6");
    }
}

QColor SlotCard::bgColor() const
{
    if (m_selected) return QColor("#EEF4F9");
    if (m_hovered)  return QColor("#F8FAFC");
    return QColor("#FFFFFF");
}

// ── Paint ───────────────────────────────────────────────────

void SlotCard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    // Background
    p.fillRect(rect(), bgColor());

    // Left accent bar (4 px)
    p.fillRect(0, 0, 4, height(), accentColor());

    // Bottom separator
    p.setPen(QColor("#E6ECF2"));
    p.drawLine(4, height() - 1, width(), height() - 1);
}

// ── Events ──────────────────────────────────────────────────

void SlotCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Let checkbox absorb its own area
        if (!m_check->geometry().contains(event->pos()))
            emit clicked(m_index);
    }
    QWidget::mousePressEvent(event);
}

void SlotCard::enterEvent(QEvent *event)
{
    m_hovered = true;
    update();
    QWidget::enterEvent(event);
}

void SlotCard::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();
    QWidget::leaveEvent(event);
}

// ── Public API ───────────────────────────────────────────────

void SlotCard::setSlot(const SampleSlot &slotData)
{
    m_slot = slotData;
    updateLabels();
    update();
}

void SlotCard::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void SlotCard::setChecked(bool checked)
{
    m_check->blockSignals(true);
    m_check->setChecked(checked);
    m_check->blockSignals(false);
}

bool SlotCard::isChecked() const
{
    return m_check->isChecked();
}

// ── Private ──────────────────────────────────────────────────

void SlotCard::updateLabels()
{
    QString slotId = m_slot.id.isEmpty()
        ? QString("S%1").arg(m_index + 1)
        : m_slot.id;

    // Icon color
    QColor iconColor;
    switch (m_slot.status) {
        case SampleStatus::Empty:     iconColor = QColor("#B7C3CE"); break;
        case SampleStatus::Scheduled: iconColor = QColor("#1464A5"); break;
        case SampleStatus::Running:   iconColor = QColor("#1E78BE"); break;
        case SampleStatus::Done:      iconColor = QColor("#2E8B57"); break;
        case SampleStatus::Error:     iconColor = QColor("#C0392B"); break;
    }
    m_iconLabel->setPixmap(SvgIcon::pixmap("tip", 18, iconColor));

    // Name label
    if (m_slot.status == SampleStatus::Empty) {
        m_nameLabel->setText(QString("%1  <span style='color:#B7C3CE;'>+</span>").arg(slotId));
        m_nameLabel->setTextFormat(Qt::RichText);
        m_nameLabel->setStyleSheet("font-size:12px; font-weight:600; color:#A5B0BC; background:transparent;");
    } else {
        m_nameLabel->setText(QString("%1  %2").arg(slotId, m_slot.name));
        m_nameLabel->setTextFormat(Qt::PlainText);
        m_nameLabel->setStyleSheet("font-size:12px; font-weight:600; color:#1F2A37; background:transparent;");
    }

    // Status label
    auto si = AppData::statusInfo(m_slot.status);
    QString stColor;
    switch (m_slot.status) {
        case SampleStatus::Empty:     stColor = "#A5B0BC"; break;
        case SampleStatus::Scheduled: stColor = "#1464A5"; break;
        case SampleStatus::Running:   stColor = "#1E78BE"; break;
        case SampleStatus::Done:      stColor = "#2E8B57"; break;
        case SampleStatus::Error:     stColor = "#C0392B"; break;
    }
    m_statusLabel->setText(si.label);
    m_statusLabel->setStyleSheet(
        QString("font-size:11px; font-weight:600; color:%1; background:transparent;").arg(stColor)
    );
}
