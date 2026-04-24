#pragma once
#include <QFrame>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class ConsumableItemCard : public QFrame {
    Q_OBJECT
public:
    explicit ConsumableItemCard(QWidget *parent = nullptr);

    void setItem(const QString &iconName,
                 const QString &displayName,
                 const QString &slotId,
                 int qty, int capacity);

    // Force-close without emitting panelOpenRequested
    void closePanel();

signals:
    void panelOpenRequested();
    void replaceRequested(int newQty);

private slots:
    void onReplaceClicked();
    void onConfirmClicked();
    void onCancelClicked();

private:
    QLabel       *m_iconLabel;
    QLabel       *m_nameLabel;
    QProgressBar *m_bar;
    QLabel       *m_qtyLabel;
    QPushButton  *m_replaceBtn;

    QWidget      *m_replacePanel  = nullptr;
    QLineEdit    *m_qtyEdit       = nullptr;
    QLabel       *m_capacityLabel = nullptr;

    int  m_capacity  = 0;
    bool m_panelOpen = false;

    void buildReplacePanel(QVBoxLayout *vl);
    void setPanelOpen(bool open);
    void applyBarStyle(int pct);
    void applyReplaceBtnStyle(bool active);
};
