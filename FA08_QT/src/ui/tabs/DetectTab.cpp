#include "DetectTab.h"
#include <QHBoxLayout>
#include <QFrame>

DetectTab::DetectTab(const QList<SampleSlot> &slots,
                     const QList<ReagentCard> &cards,
                     QWidget *parent)
    : QWidget(parent), m_slots(slots), m_cards(cards)
{
    setStyleSheet("background:#EEF2F6;");
    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setContentsMargins(0, 0, 0, 0);
    hl->setSpacing(0);

    m_slotGrid  = new SampleSlotGrid(this);
    m_listPanel = new SampleListPanel(this);
    m_formPanel = new SampleFormPanel(this);

    hl->addWidget(m_slotGrid);

    // Divider
    QFrame *div = new QFrame(this);
    div->setFrameShape(QFrame::VLine);
    div->setStyleSheet("color:#D6DEE6;");
    hl->addWidget(div);

    hl->addWidget(m_listPanel, 1);
    hl->addWidget(m_formPanel);

    connect(m_slotGrid,  &SampleSlotGrid::slotClicked,       this, &DetectTab::onSlotClicked);
    connect(m_listPanel, &SampleListPanel::editRequested,    this, &DetectTab::onEditRequested);
    connect(m_listPanel, &SampleListPanel::deleteRequested,  this, &DetectTab::onDeleteRequested);
    connect(m_formPanel, &SampleFormPanel::submitted,        this, &DetectTab::onFormSubmitted);
    connect(m_formPanel, &SampleFormPanel::cancelled,        this, &DetectTab::onFormCancelled);

    refreshAll();
}

void DetectTab::setSlots(const QList<SampleSlot> &slotsList)
{
    m_slots = slotsList;
    refreshAll();
}

void DetectTab::refreshAll()
{
    m_slotGrid->setSlots(m_slots);
    m_slotGrid->setSelectedSlot(m_selectedSlot);
    m_listPanel->setSlots(m_slots);
    if (!m_formPanel->hasActiveSlot())
        m_formPanel->setVisible(false);
}

int DetectTab::nextSeq() const
{
    int n = 0;
    for (const auto &s : m_slots)
        if (s.status != SampleStatus::Empty) n++;
    return n + 1;
}

void DetectTab::onSlotClicked(int index)
{
    if (index < 0 || index >= m_slots.size()) return;
    const SampleSlot &slot = m_slots[index];

    // Clicking done/error/running slot → just select for view, no form
    if (slot.status == SampleStatus::Running || slot.status == SampleStatus::Done) {
        m_selectedSlot = index;
        refreshAll();
        return;
    }

    m_selectedSlot = index;
    m_formPanel->loadSlot(index, slot, nextSeq());
    refreshAll();
}

void DetectTab::onEditRequested(int slotIndex)
{
    if (slotIndex < 0 || slotIndex >= m_slots.size()) return;
    m_selectedSlot = slotIndex;
    m_formPanel->loadSlot(slotIndex, m_slots[slotIndex], nextSeq());
    refreshAll();
}

void DetectTab::onDeleteRequested(QList<int> indices)
{
    for (int i : indices) {
        if (i >= 0 && i < m_slots.size()) {
            m_slots[i] = SampleSlot();
            m_slots[i].id = QString("S%1").arg(i + 1);
        }
    }
    m_selectedSlot = -1;
    m_formPanel->clearForm();
    emit slotsChanged(m_slots);
    refreshAll();
}

void DetectTab::onFormSubmitted(int slotIndex, SampleSlot slot)
{
    if (slotIndex >= 0 && slotIndex < m_slots.size()) {
        slot.id = m_slots[slotIndex].id;
        m_slots[slotIndex] = slot;
    }
    m_selectedSlot = -1;
    emit slotsChanged(m_slots);
    refreshAll();
}

void DetectTab::onFormCancelled()
{
    m_selectedSlot = -1;
    refreshAll();
}

void DetectTab::onBarcodeScanned(const QString &barcode)
{
    // If a form is open and sampleId is the active field, fill it
    if (m_formPanel->hasActiveSlot()) {
        int idx = m_formPanel->activeSlotIndex();
        if (idx >= 0 && idx < m_slots.size()) {
            m_slots[idx].sampleId = barcode;
            m_formPanel->loadSlot(idx, m_slots[idx], nextSeq());
        }
    }
}
