#include "AlertsModal.h"
#include "ui/widgets/SvgIcon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>

AlertsModal::AlertsModal(const QList<Alert> &alerts, QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::FramelessWindowHint), m_alerts(alerts)
{
    setFixedSize(560, 400);
    setStyleSheet(
        "QDialog { background:#FFFFFF; border-radius:10px; border:1px solid #D6DEE6; }"
    );

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // ── Header ──
    QWidget *hdr = new QWidget(this);
    hdr->setFixedHeight(44);
    hdr->setStyleSheet("background:#1464A5; border-radius:10px 10px 0 0;");
    QHBoxLayout *hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(14, 0, 14, 0); hl->setSpacing(8);
    QLabel *icoLbl = new QLabel(hdr);
    icoLbl->setPixmap(SvgIcon::pixmap("bell", 14, Qt::white));
    QLabel *titleLbl = new QLabel("系统报警", hdr);
    titleLbl->setStyleSheet("font-size:14px; font-weight:600; color:#fff;");
    hl->addWidget(icoLbl); hl->addWidget(titleLbl); hl->addStretch();
    QPushButton *closeBtn = new QPushButton("×", hdr);
    closeBtn->setFixedSize(28, 28);
    closeBtn->setStyleSheet(
        "QPushButton{background:transparent;border:none;color:rgba(255,255,255,0.9);font-size:18px;}"
        "QPushButton:hover{color:#fff;}"
    );
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    hl->addWidget(closeBtn);
    vl->addWidget(hdr);

    // ── Table ──
    m_table = new QTableWidget(0, 5, this);
    m_table->setHorizontalHeaderLabels({"级别","时间","代码","内容","状态"});
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_table->setColumnWidth(0, 50);
    m_table->setColumnWidth(1, 130);
    m_table->setColumnWidth(2, 70);
    m_table->setColumnWidth(4, 60);
    m_table->verticalHeader()->hide();
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setShowGrid(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setStyleSheet(
        "QTableWidget{border:none;font-size:12px;background:#fff;}"
        "QTableWidget::item{padding:4px 8px;border-bottom:1px solid #E6ECF2;}"
    );
    vl->addWidget(m_table, 1);

    // ── Footer ──
    QWidget *footer = new QWidget(this);
    footer->setFixedHeight(46);
    footer->setStyleSheet("background:#F4F7FA; border-top:1px solid #E6ECF2; border-radius:0 0 10px 10px;");
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(14, 6, 14, 6); fl->setSpacing(8);
    fl->addStretch();
    QPushButton *markAllBtn = new QPushButton("全部已处理", footer);
    markAllBtn->setFixedHeight(32);
    markAllBtn->setStyleSheet(
        "QPushButton{background:#1464A5;color:#fff;border:1px solid #0F4E85;border-radius:5px;padding:0 16px;font-size:13px;}"
        "QPushButton:hover{background:#0F4E85;}"
    );
    connect(markAllBtn, &QPushButton::clicked, this, &AlertsModal::onMarkAll);
    QPushButton *okBtn = new QPushButton("关闭", footer);
    okBtn->setFixedHeight(32);
    okBtn->setStyleSheet(
        "QPushButton{background:#fff;border:1px solid #B7C3CE;border-radius:5px;padding:0 16px;font-size:13px;}"
        "QPushButton:hover{background:#EEF4F9;}"
    );
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    fl->addWidget(markAllBtn); fl->addWidget(okBtn);
    vl->addWidget(footer);

    refreshTable();
}

void AlertsModal::refreshTable()
{
    m_table->setRowCount(0);
    for (auto &a : m_alerts) {
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setRowHeight(row, 34);

        // Level
        QString lvlText;
        QColor  lvlColor;
        if (a.level == AlertLevel::Error)      { lvlText = "错误"; lvlColor = QColor("#C0392B"); }
        else if (a.level == AlertLevel::Warn)  { lvlText = "警告"; lvlColor = QColor("#D08419"); }
        else                                   { lvlText = "信息"; lvlColor = QColor("#1464A5"); }

        QTableWidgetItem *lvl = new QTableWidgetItem(lvlText);
        lvl->setForeground(lvlColor);
        QFont lf = lvl->font(); lf.setBold(true); lvl->setFont(lf);
        lvl->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        m_table->setItem(row, 0, lvl);

        auto cell = [&](int col, const QString &txt) {
            QTableWidgetItem *it = new QTableWidgetItem(txt);
            it->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            m_table->setItem(row, col, it);
        };
        cell(1, a.time);
        cell(2, a.code);
        cell(3, a.msg);

        QTableWidgetItem *status = new QTableWidgetItem(a.handled ? "已处理" : "未处理");
        status->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        status->setForeground(a.handled ? QColor("#1E6B43") : QColor("#D08419"));
        m_table->setItem(row, 4, status);

        if (!a.handled)
            m_table->setRowHeight(row, 36);
    }
}

void AlertsModal::onMarkAll()
{
    for (auto &a : m_alerts) a.handled = true;
    emit alertsChanged(m_alerts);
    refreshTable();
}
