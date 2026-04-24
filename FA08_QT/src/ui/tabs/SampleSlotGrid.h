#pragma once
#include <QWidget>
#include <QList>
#include "data/AppData.h"

namespace Ui { class SampleSlotGrid; }
class SlotCard;

// Left panel: vertical list of 8 sample slot cards.
// Each card shows a 4px left accent bar, icon, slot-id + name, status text.
class SampleSlotGrid : public QWidget {
    Q_OBJECT
public:
    explicit SampleSlotGrid(QWidget *parent = nullptr);
    ~SampleSlotGrid();

    void setSlots(const QList<SampleSlot> &slotsList);
    void setSelectedSlot(int index);   // -1 = none

signals:
    void slotClicked(int index);

private slots:
    void onSelectAllToggled(bool checked);
    void onCardCheckChanged(int index, bool checked);

private:
    Ui::SampleSlotGrid *ui;
    SlotCard *m_cards[8];
    int       m_selected = -1;

    void buildCards();
};
