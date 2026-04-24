#include "SampleListPanel.h"
#include "ui/widgets/StatusPill.h"
#include "ui/widgets/SvgIcon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QFont>
#include <QSet>

SampleListPanel::SampleListPanel(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background:#FFFFFF;");
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // ── Toolbar ──
    QWidget *toolbar = new QWidget(this);
    toolbar->setFixedHeight(40);
    toolbar->setStyleSheet("background:#F4F7FA; border-bottom:1px solid #E6ECF2;");
    QHBoxLayout *tl = new QHBoxLayout(toolbar);
    tl->setContentsMargins(12, 0, 12, 0); tl->setSpacing(8);

    QLabel *titleLbl = new QLabel(this);
    titleLbl->setStyleSheet("font-size:13px; font-weight:600; color:#1F2A37;");
    QPixmap ico = SvgIcon::pixmap("flask_small", 13, QColor("#1F2A37"));
    titleLbl->setPixmap(ico);
    tl->addWidget(titleLbl);

    QLabel *nameLbl = new QLabel("样本编排列表", toolbar);
    nameLbl->setStyleSheet("font-size:13px; font-weight:600; color:#1F2A37;");
    tl->addWidget(nameLbl);
    tl->addStretch();

    m_editBtn = new QPushButton(toolbar);
    m_editBtn->setText("编辑");
    m_editBtn->setIcon(SvgIcon::icon("edit", 11, QColor("#1F2A37")));
    m_editBtn->setIconSize(QSize(11,11));
    m_editBtn->setFixedHeight(28);
    m_editBtn->setStyleSheet(
        "QPushButton { background:#fff; border:1px solid #B7C3CE; border-radius:5px;"
        "  padding:0 10px; font-size:12px; color:#1F2A37; }"
        "QPushButton:hover { background:#EEF4F9; border-color:#2E86C1; }"
        "QPushButton:disabled { opacity:0.45; }"
    );
    connect(m_editBtn, &QPushButton::clicked, this, &SampleListPanel::onEdit);
    tl->addWidget(m_editBtn);

    m_deleteBtn = new QPushButton(toolbar);
    m_deleteBtn->setText("删除");
    m_deleteBtn->setIcon(SvgIcon::icon("trash", 11, QColor("#1F2A37")));
    m_deleteBtn->setIconSize(QSize(11,11));
    m_deleteBtn->setFixedHeight(28);
    m_deleteBtn->setStyleSheet(
        "QPushButton { background:#fff; border:1px solid #B7C3CE; border-radius:5px;"
        "  padding:0 10px; font-size:12px; color:#1F2A37; }"
        "QPushButton:hover { background:#FBE6E3; border-color:#C0392B; }"
        "QPushButton:disabled { opacity:0.45; }"
    );
    connect(m_deleteBtn, &QPushButton::clicked, this, &SampleListPanel::onDelete);
    tl->addWidget(m_deleteBtn);

    m_selectAllBtn = new QPushButton("全选", toolbar);
    m_selectAllBtn->setFixedHeight(28);
    m_selectAllBtn->setStyleSheet(
        "QPushButton { background:#1464A5; color:#fff; border:1px solid #0F4E85;"
        "  border-radius:5px; padding:0 10px; font-size:12px; }"
        "QPushButton:hover { background:#0F4E85; }"
    );
    connect(m_selectAllBtn, &QPushButton::clicked, this, &SampleListPanel::onSelectAll);
    tl->addWidget(m_selectAllBtn);
    vl->addWidget(toolbar);

    // ── Table ──
    m_table = new QTableWidget(0, 6, this);
    m_table->setHorizontalHeaderLabels({"流水号","样本ID","类型","位置","检测项目","状态"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_table->verticalHeader()->hide();
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(false);
    m_table->setStyleSheet(
        "QTableWidget { border:none; font-size:12px; }"
        "QTableWidget::item { padding:4px 8px; border-bottom:1px solid #E6ECF2; }"
        "QTableWidget::item:selected { background:#DCE8F2; color:#1F2A37; }"
    );
    // Column widths matching HTML: seq=48 id=148 type=60 pos=58 assay=172 status=96
    m_table->setColumnWidth(0, 48);
    m_table->setColumnWidth(1, 148);
    m_table->setColumnWidth(2, 60);
    m_table->setColumnWidth(3, 58);
    m_table->setColumnWidth(4, 172);
    m_table->setColumnWidth(5, 96);
    m_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    connect(m_table, &QTableWidget::itemSelectionChanged, this, &SampleListPanel::onSelectionChanged);
    vl->addWidget(m_table, 1);

    onSelectionChanged();
}

void SampleListPanel::setSlots(const QList<SampleSlot> &slotsList)
{
    m_slots = slotsList;
    refreshTable();
}

void SampleListPanel::clearSelection()
{
    m_table->clearSelection();
}

void SampleListPanel::refreshTable()
{
    m_table->setRowCount(0);
    int seq = 0;
    for (int i = 0; i < m_slots.size(); ++i) {
        const SampleSlot &s = m_slots[i];
        if (s.status == SampleStatus::Empty) continue;
        seq++;
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setRowHeight(row, 36);

        // Store slot index in row data
        auto setItem = [&](int col, const QString &text, Qt::Alignment align = Qt::AlignCenter) {
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(align | Qt::AlignVCenter);
            item->setData(Qt::UserRole, i); // store original slot index
            m_table->setItem(row, col, item);
        };

        setItem(0, QString("%1").arg(seq, 3, 10, QChar('0')));
        setItem(1, s.sampleId, Qt::AlignLeft);
        setItem(2, s.type);
        setItem(3, s.id);

        // Assays (up to 4 tags)
        QString assayText = s.assays.mid(0, 4).join("  ");
        if (s.assays.size() > 4) assayText += " ···";
        setItem(4, assayText, Qt::AlignLeft);

        // Status
        auto si = AppData::statusInfo(s.status);
        QTableWidgetItem *stItem = new QTableWidgetItem(si.label);
        stItem->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        stItem->setData(Qt::UserRole, i);
        QColor bg, fg;
        if      (si.cssClass == "ok")   { bg=QColor("#E6F3EC"); fg=QColor("#1E6B43"); }
        else if (si.cssClass == "run")  { bg=QColor("#DCE8F2"); fg=QColor("#0F4E85"); }
        else if (si.cssClass == "err")  { bg=QColor("#FBE6E3"); fg=QColor("#8E2A20"); }
        else                            { bg=QColor("#E8EEF3"); fg=QColor("#5B6B7B"); }
        stItem->setBackground(bg);
        stItem->setForeground(fg);
        QFont f = stItem->font(); f.setBold(true); stItem->setFont(f);
        m_table->setItem(row, 5, stItem);
    }
    onSelectionChanged();
}

QList<int> SampleListPanel::selectedSlotIndices() const
{
    QSet<int> rows;
    for (auto *item : m_table->selectedItems())
        rows.insert(item->row());

    QList<int> indices;
    for (int r : rows) {
        QTableWidgetItem *item = m_table->item(r, 0);
        if (item) indices.append(item->data(Qt::UserRole).toInt());
    }
    return indices;
}

void SampleListPanel::onSelectionChanged()
{
    int count = m_table->selectionModel()->selectedRows().count();
    m_editBtn->setEnabled(count == 1);
    m_deleteBtn->setEnabled(count > 0);
    if (count > 0) m_deleteBtn->setText(QString("删除(%1)").arg(count));
    else           m_deleteBtn->setText("删除");
}

void SampleListPanel::onEdit()
{
    QList<int> sel = selectedSlotIndices();
    if (sel.size() == 1) emit editRequested(sel.first());
}

void SampleListPanel::onDelete()
{
    QList<int> sel = selectedSlotIndices();
    if (!sel.isEmpty()) emit deleteRequested(sel);
}

void SampleListPanel::onSelectAll()
{
    if (m_table->selectionModel()->selectedRows().count() == m_table->rowCount())
        m_table->clearSelection();
    else
        m_table->selectAll();
}
