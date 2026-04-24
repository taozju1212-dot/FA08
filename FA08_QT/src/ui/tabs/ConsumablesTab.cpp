#include "ConsumablesTab.h"
#include "ui_ConsumablesTab.h"
#include "ui/widgets/DilutionCard.h"
#include "ui/widgets/ConsumableItemCard.h"
#include "ui/widgets/SvgIcon.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QHBoxLayout>

ConsumablesTab::ConsumablesTab(const Consumables &consumables,
                               const QList<ReagentCard> &cards,
                               QWidget *parent)
    : QWidget(parent), ui(new Ui::ConsumablesTab),
      m_consumables(consumables), m_cards(cards)
{
    ui->setupUi(this);
    styleStaticWidgets();
    buildDynamicWidgets();
    refreshCalibTable();
    refreshDilutions();
    refreshConsumables();
}

ConsumablesTab::~ConsumablesTab()
{
    delete ui;
}

// ── Static widget styling ────────────────────────────────────

void ConsumablesTab::styleStaticWidgets()
{
    setStyleSheet("background:#EEF2F6;");

    // Left panel: white card with border
    ui->leftPanel->setStyleSheet(
        "QFrame#leftPanel { background:#FFFFFF; border:1px solid #D6DEE6; border-radius:8px; }"
    );

    // Left header
    ui->leftHeader->setStyleSheet(
        "background:#F4F7FA; border-radius:8px 8px 0 0; border-bottom:1px solid #E6ECF2;"
    );
    ui->leftTitleLabel->setStyleSheet(
        "font-size:13px; font-weight:600; color:#1F2A37; background:transparent;"
    );

    // Calibration table
    ui->calibTable->verticalHeader()->hide();
    ui->calibTable->setShowGrid(false);
    ui->calibTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->calibTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->calibTable->setAlternatingRowColors(false);
    ui->calibTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->calibTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->calibTable->setColumnWidth(0, 36);
    ui->calibTable->setStyleSheet(
        "QTableWidget { border:none; font-size:13px; background:#FFFFFF; }"
        "QTableWidget::item { padding:4px 8px; border-bottom:1px solid #E6ECF2; }"
        "QTableWidget::item:selected { background:#DCE8F2; color:#1F2A37; }"
        "QHeaderView::section { background:#F4F7FA; border:none; border-bottom:1px solid #E6ECF2;"
        "  padding:6px 8px; font-size:12px; color:#5B6B7B; font-weight:600; }"
    );

    // Left footer
    ui->leftFooter->setStyleSheet(
        "background:#F4F7FA; border-top:1px solid #E6ECF2; border-radius:0 0 8px 8px;"
    );
    QString btnStyle =
        "QPushButton { background:#FFFFFF; border:1px solid #B7C3CE; border-radius:5px;"
        "  padding:0 14px; font-size:12px; color:#1F2A37; }"
        "QPushButton:hover { background:#EEF4F9; border-color:#7FB2DC; }"
        "QPushButton:disabled { color:#A5B0BC; }";
    ui->importBtn->setStyleSheet(btnStyle);
    ui->deleteBtn->setStyleSheet(btnStyle);
    ui->clearBtn->setStyleSheet(btnStyle);
    ui->prevBtn->setStyleSheet(btnStyle);
    ui->nextBtn->setStyleSheet(btnStyle);
    ui->pageLabel->setStyleSheet("font-size:12px; color:#5B6B7B; background:transparent;");

    // Right panel section headers
    ui->dilutionGroup->setStyleSheet(
        "QFrame#dilutionGroup { background:#FFFFFF; border:1px solid #D6DEE6; border-radius:8px; }"
    );
    ui->dilutionTitle->setStyleSheet(
        "font-size:13px; font-weight:600; color:#1F2A37; background:transparent;"
    );
    ui->consumableGroup->setStyleSheet(
        "QFrame#consumableGroup { background:#FFFFFF; border:1px solid #D6DEE6; border-radius:8px; }"
    );
    ui->consumableTitle->setStyleSheet(
        "font-size:13px; font-weight:600; color:#1F2A37; background:transparent;"
    );
    ui->dilutionCardsWidget->setStyleSheet("background:transparent;");
    ui->consumableCardsWidget->setStyleSheet("background:transparent;");

    // Right scroll area: transparent background, no border
    ui->rightScrollArea->setStyleSheet(
        "QScrollArea { background:transparent; border:none; }"
        "QScrollArea > QWidget > QWidget { background:transparent; }"
    );
    ui->rightScrollContents->setStyleSheet("background:transparent;");
}

// ── Dynamic widgets ──────────────────────────────────────────

void ConsumablesTab::buildDynamicWidgets()
{
    // 3 dilution cards
    for (int i = 0; i < 3; ++i) {
        m_dilCards[i] = new DilutionCard(ui->dilutionCardsWidget);
        ui->dilutionLayout->addWidget(m_dilCards[i]);
        connect(m_dilCards[i], &DilutionCard::panelOpenRequested,
                this, &ConsumablesTab::onExclusivePanelOpen);
    }

    // Plate card (C1) + Tips card (C2)
    m_plateCard = new ConsumableItemCard(ui->consumableCardsWidget);
    m_tipsCard  = new ConsumableItemCard(ui->consumableCardsWidget);
    ui->consumableLayout->addWidget(m_plateCard);
    ui->consumableLayout->addWidget(m_tipsCard);
    connect(m_plateCard, &ConsumableItemCard::panelOpenRequested,
            this, &ConsumablesTab::onExclusivePanelOpen);
    connect(m_tipsCard, &ConsumableItemCard::panelOpenRequested,
            this, &ConsumablesTab::onExclusivePanelOpen);
}

// ── Mutual exclusion: close all cards except the one that just requested open ──
void ConsumablesTab::onExclusivePanelOpen()
{
    QObject *requestor = sender();
    for (auto *c : m_dilCards)
        if (c != requestor) c->closePanel();
    if (m_plateCard != requestor) m_plateCard->closePanel();
    if (m_tipsCard  != requestor) m_tipsCard->closePanel();
}

// ── Data refresh ─────────────────────────────────────────────

void ConsumablesTab::setConsumables(const Consumables &c)
{
    m_consumables = c;
    refreshDilutions();
    refreshConsumables();
}

void ConsumablesTab::setCards(const QList<ReagentCard> &cards)
{
    m_cards = cards;
    refreshCalibTable();
}

void ConsumablesTab::refreshCalibTable()
{
    ui->calibTable->setRowCount(0);

    for (const auto &c : m_cards) {
        int row = ui->calibTable->rowCount();
        ui->calibTable->insertRow(row);
        ui->calibTable->setRowHeight(row, 40);

        // Column 0: checkbox
        QTableWidgetItem *chkItem = new QTableWidgetItem();
        chkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        chkItem->setCheckState(Qt::Unchecked);
        chkItem->setTextAlignment(Qt::AlignCenter);
        ui->calibTable->setItem(row, 0, chkItem);

        // Column 1: name (bold)
        QTableWidgetItem *nameItem = new QTableWidgetItem(c.code);
        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QFont f = nameItem->font(); f.setBold(true); nameItem->setFont(f);
        ui->calibTable->setItem(row, 1, nameItem);

        // Column 2: lot
        QTableWidgetItem *lotItem = new QTableWidgetItem(c.lot);
        lotItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lotItem->setForeground(QColor("#5B6B7B"));
        ui->calibTable->setItem(row, 2, lotItem);

        // Column 3: expire
        QTableWidgetItem *expItem = new QTableWidgetItem(c.expire);
        expItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        expItem->setForeground(QColor("#5B6B7B"));
        ui->calibTable->setItem(row, 3, expItem);
    }
}

void ConsumablesTab::refreshDilutions()
{
    int n = qMin(3, (int)m_consumables.dilutions.size());
    for (int i = 0; i < n; ++i)
        m_dilCards[i]->setData(m_consumables.dilutions[i]);
}

void ConsumablesTab::refreshConsumables()
{
    m_plateCard->setItem("plate", m_consumables.plate.name,
                         m_consumables.plate.slot,
                         m_consumables.plate.qty, m_consumables.plate.capacity);

    m_tipsCard->setItem("tip", m_consumables.tips.name,
                        m_consumables.tips.slot,
                        m_consumables.tips.qty, m_consumables.tips.capacity);
}
