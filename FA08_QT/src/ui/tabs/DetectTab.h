#pragma once
#include <QWidget>
#include <QList>
#include "data/AppData.h"
#include "SampleSlotGrid.h"
#include "SampleListPanel.h"
#include "SampleFormPanel.h"

class DetectTab : public QWidget {
    Q_OBJECT
public:
    explicit DetectTab(const QList<SampleSlot> &slotsList,
                       const QList<ReagentCard> &cards,
                       QWidget *parent = nullptr);

    void setSlots(const QList<SampleSlot> &slotsList);
    void onBarcodeScanned(const QString &barcode);

signals:
    void slotsChanged(const QList<SampleSlot> &slotsList);

private slots:
    void onSlotClicked(int index);
    void onEditRequested(int slotIndex);
    void onDeleteRequested(QList<int> indices);
    void onFormSubmitted(int slotIndex, SampleSlot slot);
    void onFormCancelled();

private:
    QList<SampleSlot>  m_slots;
    QList<ReagentCard> m_cards;
    int                m_selectedSlot = -1;

    SampleSlotGrid  *m_slotGrid;
    SampleListPanel *m_listPanel;
    SampleFormPanel *m_formPanel;

    int  nextSeq() const;
    void refreshAll();
};
