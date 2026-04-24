#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "data/AppData.h"
#include "CircularProgress.h"

class BottomBar : public QWidget {
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = nullptr);

    void setConsumables(const Consumables &c);
    void setDeviceStatus(DeviceStatus status);
    void setScheduledCount(int count);
    void setPendingCardCount(int count);

signals:
    void startClicked();
    void pauseClicked();
    void addCardClicked();

private:
    QPushButton    *m_startBtn;
    QPushButton    *m_pauseBtn;
    QPushButton    *m_cardBtn;
    QLabel         *m_cardCountLabel;
    QLabel         *m_tipsQty;
    QLabel         *m_plateQty;
    CircularProgress *m_dilChip[3];

    DeviceStatus    m_deviceStatus   = DeviceStatus::Idle;
    int             m_scheduledCount = 0;
    int             m_pendingCards   = 0;

    void setupUI();
    void updateButtons();
};
