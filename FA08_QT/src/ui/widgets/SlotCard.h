#pragma once
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include "data/AppData.h"

// Single sample slot card — full-width, 64px tall.
// Draws a 4px left accent bar via paintEvent.
class SlotCard : public QWidget {
    Q_OBJECT
public:
    explicit SlotCard(int index, QWidget *parent = nullptr);

    void setSlot(const SampleSlot &slotData);
    void setSelected(bool selected);
    void setChecked(bool checked);
    bool isChecked() const;

signals:
    void clicked(int index);
    void checkChanged(int index, bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    int        m_index;
    SampleSlot m_slot;
    bool       m_selected = false;
    bool       m_hovered  = false;

    QCheckBox *m_check;
    QLabel    *m_iconLabel;
    QLabel    *m_nameLabel;
    QLabel    *m_statusLabel;

    QColor accentColor() const;
    QColor bgColor()     const;
    void   updateLabels();
};
