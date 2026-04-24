#include "ResultsTab.h"
#include "ui/widgets/SvgIcon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QPushButton>

ResultsTab::ResultsTab(const QList<TestResult> &results, QWidget *parent)
    : QWidget(parent), m_results(results)
{
    setStyleSheet("background:#EEF2F6;");
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // ── Toolbar ──
    QWidget *toolbar = new QWidget(this);
    toolbar->setFixedHeight(44);
    toolbar->setStyleSheet("background:#F4F7FA; border-bottom:1px solid #D6DEE6;");
    QHBoxLayout *tl = new QHBoxLayout(toolbar);
    tl->setContentsMargins(14, 0, 14, 0); tl->setSpacing(8);

    QLabel *ico = new QLabel(toolbar);
    ico->setPixmap(SvgIcon::pixmap("chart", 14, QColor("#1F2A37")));
    QLabel *title = new QLabel("历史检测结果", toolbar);
    title->setStyleSheet("font-size:13px; font-weight:600; color:#1F2A37;");
    tl->addWidget(ico); tl->addWidget(title); tl->addStretch();

    QPushButton *exportBtn = new QPushButton("导出", toolbar);
    exportBtn->setIcon(SvgIcon::icon("export", 12, QColor("#1F2A37")));
    exportBtn->setIconSize(QSize(12,12));
    exportBtn->setFixedHeight(30);
    exportBtn->setStyleSheet(
        "QPushButton{background:#fff;border:1px solid #B7C3CE;border-radius:5px;padding:0 12px;font-size:12px;}"
        "QPushButton:hover{background:#EEF4F9;}"
    );
    tl->addWidget(exportBtn);

    QPushButton *printBtn = new QPushButton("打印", toolbar);
    printBtn->setIcon(SvgIcon::icon("print", 12, QColor("#1F2A37")));
    printBtn->setIconSize(QSize(12,12));
    printBtn->setFixedHeight(30);
    printBtn->setStyleSheet(
        "QPushButton{background:#fff;border:1px solid #B7C3CE;border-radius:5px;padding:0 12px;font-size:12px;}"
        "QPushButton:hover{background:#EEF4F9;}"
    );
    tl->addWidget(printBtn);
    vl->addWidget(toolbar);

    // ── Results table ──
    // Columns: 样本ID | 姓名 | 性别 | 年龄 | 类型 | 检测项目 | 结果 | 参考范围 | 结论 | 完成时间
    m_table = new QTableWidget(0, 9, this);
    m_table->setHorizontalHeaderLabels({
        "样本ID","姓名","性别","年龄","类型","项目","结果","参考范围","结论/时间"
    });
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_table->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    m_table->setColumnWidth(0, 130);
    m_table->verticalHeader()->hide();
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(true);
    m_table->setStyleSheet(
        "QTableWidget { border:none; font-size:12px; background:#fff; alternate-background-color:#F8FAFC; }"
        "QTableWidget::item { padding:5px 10px; border-bottom:1px solid #E6ECF2; }"
        "QTableWidget::item:selected { background:#DCE8F2; color:#1F2A37; }"
    );
    vl->addWidget(m_table, 1);

    refreshTable();
}

void ResultsTab::setResults(const QList<TestResult> &results)
{
    m_results = results;
    refreshTable();
}

void ResultsTab::refreshTable()
{
    m_table->setRowCount(0);
    for (const auto &r : m_results) {
        for (int i = 0; i < r.items.size(); ++i) {
            const ResultItem &item = r.items[i];
            int row = m_table->rowCount();
            m_table->insertRow(row);
            m_table->setRowHeight(row, 34);

            auto cell = [&](int col, const QString &txt, Qt::Alignment align = Qt::AlignVCenter | Qt::AlignLeft) {
                QTableWidgetItem *it = new QTableWidgetItem(txt);
                it->setTextAlignment(align);
                m_table->setItem(row, col, it);
            };

            // Merge sample info only on first item row
            if (i == 0) {
                cell(0, r.sampleId);
                cell(1, r.name);
                cell(2, r.gender, Qt::AlignCenter | Qt::AlignVCenter);
                cell(3, QString::number(r.age), Qt::AlignCenter | Qt::AlignVCenter);
                cell(4, r.type, Qt::AlignCenter | Qt::AlignVCenter);
            } else {
                for (int c = 0; c < 5; ++c)
                    m_table->setItem(row, c, new QTableWidgetItem(""));
            }

            cell(5, item.code, Qt::AlignCenter | Qt::AlignVCenter);

            // Result value (monospace)
            QTableWidgetItem *valItem = new QTableWidgetItem(
                QString("%1 %2").arg(item.value, 0, 'g', 4).arg(item.unit)
            );
            valItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            valItem->setFont(QFont("Consolas,Courier New,monospace", 11));
            m_table->setItem(row, 6, valItem);

            cell(7, item.ref);

            // Conclusion with color
            QTableWidgetItem *conclItem = new QTableWidgetItem(
                item.concl + "  " + (i == 0 ? r.time : "")
            );
            conclItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            if (item.concl == "阳性") {
                conclItem->setForeground(QColor("#C0392B"));
                QFont f = conclItem->font(); f.setBold(true); conclItem->setFont(f);
            } else {
                conclItem->setForeground(QColor("#1E6B43"));
            }
            m_table->setItem(row, 8, conclItem);
        }
    }
}
