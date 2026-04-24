#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QList>
#include "data/AppData.h"

class SvgIconLabel;

class TopBar : public QWidget {
    Q_OBJECT
public:
    explicit TopBar(QWidget *parent = nullptr);

    void setActiveTab(int index);
    void setDeviceStatus(DeviceStatus status);
    void setTemperature(double celsius);
    void setUnhandledAlerts(int count);

signals:
    void tabChanged(int index);
    void alertsBellClicked();

protected:
    void paintEvent(QPaintEvent *) override;

private slots:
    void updateClock();

private:
    QPushButton *m_tabs[4];
    QLabel      *m_clockLabel;
    QLabel      *m_tempLabel;
    QLabel      *m_alertBadge;
    QLabel      *m_statusDot;
    QLabel      *m_statusLabel;
    QWidget     *m_runStrip;
    QTimer       m_clockTimer;
    int          m_activeTab = 0;
    DeviceStatus m_deviceStatus = DeviceStatus::Idle;

    void setupUI();
    void updateTabStyles();
    void updateStatusDot();
};
