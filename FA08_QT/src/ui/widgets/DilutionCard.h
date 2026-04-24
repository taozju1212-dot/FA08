#pragma once
#include <QFrame>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "data/AppData.h"

class DilutionCard : public QFrame {
    Q_OBJECT
public:
    explicit DilutionCard(QWidget *parent = nullptr);
    void setData(const DilutionSolution &d);

    // Force-close the panel without emitting panelOpenRequested
    void closePanel();

signals:
    // Emitted just before this card opens its panel (used for mutual exclusion)
    void panelOpenRequested();
    void replaceRequested(const DilutionSolution &original, const QString &barcode);

private slots:
    void onReplaceClicked();
    void onConfirmClicked();
    void onCancelClicked();

private:
    QLabel       *m_iconLabel;
    QLabel       *m_nameLabel;   // one-line: bold name + slot + lot + expire
    QProgressBar *m_bar;
    QLabel       *m_qtyLabel;
    QPushButton  *m_replaceBtn;

    QWidget      *m_replacePanel = nullptr;
    QLineEdit    *m_barcodeEdit  = nullptr;

    DilutionSolution m_data;
    bool             m_panelOpen = false;

    void buildReplacePanel(QVBoxLayout *vl);
    void setPanelOpen(bool open);
    void applyBarStyle(int pct, bool loaded);
    void applyReplaceBtnStyle(bool active);
};
