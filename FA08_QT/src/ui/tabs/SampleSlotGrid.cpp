#include "SampleSlotGrid.h"
#include "ui_SampleSlotGrid.h"
#include "ui/widgets/SlotCard.h"

SampleSlotGrid::SampleSlotGrid(QWidget *parent)
    : QWidget(parent), ui(new Ui::SampleSlotGrid)
{
    ui->setupUi(this);
    setFixedWidth(200);

    // Header styles
    ui->headerWidget->setStyleSheet(
        "background:#F4F7FA; border-bottom:1px solid #E6ECF2;"
    );
    ui->titleLabel->setStyleSheet(
        "font-size:12px; font-weight:600; color:#334155; background:transparent;"
    );
    ui->selectAllCheck->setStyleSheet(
        "QCheckBox { font-size:11px; color:#5B6B7B; }"
        "QCheckBox::indicator { width:14px; height:14px; }"
    );
    ui->scrollArea->setStyleSheet("QScrollArea { background:#FFFFFF; border:none; }");
    ui->slotContainer->setStyleSheet("background:#FFFFFF;");

    buildCards();

    connect(ui->selectAllCheck, &QCheckBox::toggled,
            this, &SampleSlotGrid::onSelectAllToggled);
}

SampleSlotGrid::~SampleSlotGrid()
{
    delete ui;
}

void SampleSlotGrid::buildCards()
{
    for (int i = 0; i < 8; ++i) {
        m_cards[i] = new SlotCard(i, ui->slotContainer);
        connect(m_cards[i], &SlotCard::clicked,
                this, &SampleSlotGrid::slotClicked);
        connect(m_cards[i], &SlotCard::checkChanged,
                this, &SampleSlotGrid::onCardCheckChanged);
        ui->slotLayout->addWidget(m_cards[i]);
    }
    ui->slotLayout->addStretch();
}

void SampleSlotGrid::setSlots(const QList<SampleSlot> &slotsList)
{
    for (int i = 0; i < qMin(8, (int)slotsList.size()); ++i)
        m_cards[i]->setSlot(slotsList[i]);
}

void SampleSlotGrid::setSelectedSlot(int index)
{
    m_selected = index;
    for (int i = 0; i < 8; ++i)
        m_cards[i]->setSelected(i == index);
}

void SampleSlotGrid::onSelectAllToggled(bool checked)
{
    for (int i = 0; i < 8; ++i)
        m_cards[i]->setChecked(checked);
}

void SampleSlotGrid::onCardCheckChanged(int /*index*/, bool /*checked*/)
{
    bool allChecked = true;
    for (int i = 0; i < 8; ++i)
        if (!m_cards[i]->isChecked()) { allChecked = false; break; }

    ui->selectAllCheck->blockSignals(true);
    ui->selectAllCheck->setChecked(allChecked);
    ui->selectAllCheck->blockSignals(false);
}
