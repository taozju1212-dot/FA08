#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QList>
#include "data/AppData.h"

// Middle panel: scrollable list of arranged samples with toolbar.
class SampleListPanel : public QWidget {
    Q_OBJECT
public:
    explicit SampleListPanel(QWidget *parent = nullptr);
    void setSlots(const QList<SampleSlot> &slotsList);
    void clearSelection();

signals:
    void editRequested(int slotIndex);
    void deleteRequested(QList<int> slotIndices);

private slots:
    void onEdit();
    void onDelete();
    void onSelectAll();
    void onSelectionChanged();

private:
    QTableWidget *m_table;
    QPushButton  *m_editBtn;
    QPushButton  *m_deleteBtn;
    QPushButton  *m_selectAllBtn;
    QList<SampleSlot> m_slots;  // full 8-slot list (visible = non-empty)

    QList<int> selectedSlotIndices() const;
    void       refreshTable();
};
