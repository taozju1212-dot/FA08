#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QList>
#include "data/AppData.h"

class AlertsModal : public QDialog {
    Q_OBJECT
public:
    explicit AlertsModal(const QList<Alert> &alerts, QWidget *parent = nullptr);

signals:
    void alertsChanged(const QList<Alert> &alerts);

private slots:
    void onMarkAll();

private:
    QList<Alert>  m_alerts;
    QTableWidget *m_table;
    void refreshTable();
};
