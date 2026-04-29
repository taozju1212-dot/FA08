#include "SettingsTab.h"
#include "ui/widgets/SvgIcon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QMouseEvent>
#include <QFrame>
#include <QHeaderView>
#include <QDateTime>
#include <QFont>
#include <QSplitter>
#include <QButtonGroup>
#include <QDialog>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPointer>

// ══════════════════════════════════════════════════════════════════
//  Colors
// ══════════════════════════════════════════════════════════════════
namespace C {
    static const char *BLUE    = "#1464A5";
    static const char *BLUE50  = "#EEF4F9";
    static const char *BORDER  = "#D6DEE6";
    static const char *SURFALT = "#F4F7FA";
    static const char *INK800  = "#1F2A37";
    static const char *INK700  = "#334155";
    static const char *INK500  = "#5B6B7B";
    static const char *INK400  = "#7A8898";
    static const char *INK300  = "#A5B0BC";
    static const char *OFFTRK  = "#BCC1CC";
    static const char *OK      = "#2E8B57";
}

// ══════════════════════════════════════════════════════════════════
//  ToggleSwitch
// ══════════════════════════════════════════════════════════════════
ToggleSwitch::ToggleSwitch(bool on, QWidget *parent)
    : QWidget(parent), m_on(on)
{
    setFixedSize(40, 22);
    setCursor(Qt::PointingHandCursor);
}

void ToggleSwitch::setChecked(bool v)
{
    if (m_on == v) return;
    m_on = v;
    update();
    emit toggled(m_on);
}

void ToggleSwitch::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(m_on ? C::BLUE : C::OFFTRK));
    p.drawRoundedRect(0, 0, 40, 22, 11, 11);
    p.setBrush(QColor("#FFFFFF"));
    int kx = m_on ? 21 : 3;
    p.drawEllipse(kx, 3, 16, 16);
}

void ToggleSwitch::mousePressEvent(QMouseEvent *)
{
    m_on = !m_on;
    update();
    emit toggled(m_on);
}

// ══════════════════════════════════════════════════════════════════
//  Helpers
// ══════════════════════════════════════════════════════════════════
QString SettingsTab::inputStyle()
{
    return QString(
        "QLineEdit,QDateEdit,QTimeEdit,QComboBox{"
        "  height:26px; padding:0 7px; font-size:12px;"
        "  border:1px solid %1; border-radius:4px;"
        "  background:#fff; color:%2; }"
        "QDateEdit::drop-down,QTimeEdit::drop-down,QComboBox::drop-down"
        "{ border:none; width:18px; }"
    ).arg(C::BORDER).arg(C::INK800);
}

QString SettingsTab::smallBtnStyle(bool primary)
{
    if (primary)
        return QString(
            "QPushButton{ height:26px; padding:0 10px; font-size:11px;"
            "  background:%1; color:#fff; border:1px solid #0f4f82;"
            "  border-radius:4px; }"
            "QPushButton:hover{ background:#1056a0; }"
            "QPushButton:disabled{ background:#9cbfe0; border-color:#9cbfe0; }"
        ).arg(C::BLUE);
    return QString(
        "QPushButton{ height:26px; padding:0 10px; font-size:11px;"
        "  background:#fff; color:%1; border:1px solid %2;"
        "  border-radius:4px; }"
        "QPushButton:hover{ background:%3; }"
        "QPushButton:disabled{ color:%4; }"
    ).arg(C::INK700).arg(C::BORDER).arg(C::BLUE50).arg(C::INK300);
}

/* Returns a styled section-title widget (thin blue label + bottom border) */
QWidget *SettingsTab::makeSecTitle(const QString &text, QWidget *parent)
{
    QWidget *w = new QWidget(parent);
    w->setStyleSheet(QString("border-bottom:1.5px solid %1; margin-top:10px; margin-bottom:4px;").arg(C::BLUE));
    QHBoxLayout *hl = new QHBoxLayout(w);
    hl->setContentsMargins(0,6,0,4);
    QLabel *lbl = new QLabel(text, w);
    lbl->setStyleSheet(QString("font-size:12px; font-weight:700; color:%1;"
                               "border:none; margin:0; padding:0;").arg(C::BLUE));
    hl->addWidget(lbl);
    return w;
}

/* Returns a horizontal row: [label lw px] [ctrl] [optional extra] */
QWidget *SettingsTab::makeRow(const QString &label, int lw, QWidget *ctrl,
                               QWidget *parent, QWidget *extra)
{
    QWidget *row = new QWidget(parent);
    row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
    QHBoxLayout *hl = new QHBoxLayout(row);
    hl->setContentsMargins(0, 7, 0, 7);
    hl->setSpacing(10);
    QLabel *lbl = new QLabel(label, row);
    lbl->setFixedWidth(lw);
    lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK500));
    hl->addWidget(lbl);
    if (ctrl) hl->addWidget(ctrl);
    if (extra) hl->addWidget(extra);
    hl->addStretch();
    return row;
}

QPushButton *SettingsTab::makeActionBtn(const QString &text, bool primary, QWidget *parent)
{
    QPushButton *btn = new QPushButton(text, parent);
    btn->setFixedHeight(28);
    btn->setStyleSheet(primary
        ? QString("QPushButton{ padding:0 20px; font-size:12px; background:%1; color:#fff;"
                  "  border:1px solid #0f4f82; border-radius:4px; }"
                  "QPushButton:hover{ background:#1056a0; }").arg(C::BLUE)
        : QString("QPushButton{ padding:0 16px; font-size:12px; background:#fff; color:%1;"
                  "  border:1px solid %2; border-radius:4px; }"
                  "QPushButton:hover{ background:%3; }").arg(C::INK700).arg(C::BORDER).arg(C::BLUE50)
    );
    return btn;
}

ToggleSwitch *SettingsTab::makeToggle(bool on, QWidget *parent)
{
    return new ToggleSwitch(on, parent);
}

QWidget *SettingsTab::wrapScroll(QWidget *content)
{
    QScrollArea *sa = new QScrollArea();
    sa->setWidgetResizable(true);
    sa->setFrameShape(QFrame::NoFrame);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setStyleSheet("QScrollArea{ background:transparent; }");
    sa->setWidget(content);
    return sa;
}

// ── Segment button pair helper (returns container widget) ──
static QWidget *makeSegment(const QString &l0, const QString &l1,
                             QPushButton *&b0, QPushButton *&b1, QWidget *parent)
{
    QWidget *seg = new QWidget(parent);
    QHBoxLayout *hl = new QHBoxLayout(seg);
    hl->setContentsMargins(0,0,0,0);
    hl->setSpacing(0);

    b0 = new QPushButton(l0, seg);
    b1 = new QPushButton(l1, seg);
    for (auto *b : {b0, b1}) {
        b->setFixedHeight(26);
        b->setCheckable(true);
        b->setAutoExclusive(false);
    }
    QString segStyle = QString(
        "QPushButton{ font-size:12px; padding:0 14px;"
        "  border:1px solid %1; background:#fff; color:%2;"
        "  font-weight:400; }"
        "QPushButton:checked{ background:%3; color:#fff; border-color:#0f4f82; font-weight:600; }"
    ).arg(C::BORDER).arg(C::INK700).arg(C::BLUE);
    b0->setStyleSheet(segStyle + "QPushButton{ border-radius:4px 0 0 4px; }");
    b1->setStyleSheet(segStyle + "QPushButton{ border-radius:0 4px 4px 0; border-left:none; }");
    hl->addWidget(b0);
    hl->addWidget(b1);
    return seg;
}

// ── Status/badge label helpers ──
static QLabel *makeStatusLabel(const QString &text, bool isOk, QWidget *parent)
{
    QLabel *lbl = new QLabel(text, parent);
    lbl->setStyleSheet(QString("font-size:11px; color:%1; border:none;")
                       .arg(isOk ? C::OK : C::INK300));
    return lbl;
}

static QLabel *makeBadge(const QString &text, QWidget *parent)
{
    QLabel *lbl = new QLabel(text, parent);
    lbl->setStyleSheet(QString(
        "font-size:11px; color:%1; background:%2;"
        "border:1px solid #c3dafe; border-radius:10px; padding:1px 8px;")
        .arg(C::BLUE).arg(C::BLUE50));
    lbl->setCursor(Qt::PointingHandCursor);
    return lbl;
}

// ══════════════════════════════════════════════════════════════════
//  SettingsTab constructor
// ══════════════════════════════════════════════════════════════════
SettingsTab::SettingsTab(QWidget *parent) : QWidget(parent)
{
    setStyleSheet(QString("background:%1;").arg("#EEF2F6"));

    QHBoxLayout *root = new QHBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(8);

    // ── Left menu ──────────────────────────────────────────────────
    QWidget *menuBox = new QWidget(this);
    menuBox->setFixedWidth(116);
    menuBox->setStyleSheet(QString(
        "QWidget{ background:#fff; border:1px solid %1; border-radius:6px; }").arg(C::BORDER));
    QVBoxLayout *ml = new QVBoxLayout(menuBox);
    ml->setContentsMargins(4, 6, 4, 6);
    ml->setSpacing(2);

    struct MenuItem { const char *label; const char *icon; };
    static const MenuItem ITEMS[8] = {
        {"仪器设置", "flask"},
        {"质控设置", "chart"},
        {"项目设置", "plate"},
        {"打印设置", "print"},
        {"用户设置", "user"},
        {"系统设置", "settings"},
        {"关于仪器", "eye"},
        {"厂家模式", "dot"},
    };

    QString menuInactiveStyle = QString(
        "QPushButton{ background:transparent; color:%1; font-size:12px; font-weight:400;"
        "  border:none; border-radius:4px; text-align:left; padding:0 8px; }"
        "QPushButton:hover{ background:%2; }").arg(C::INK700).arg(C::BLUE50);
    QString menuActiveStyle = QString(
        "QPushButton{ background:%1; color:%2; font-size:12px; font-weight:600;"
        "  border:none; border-radius:4px; text-align:left; padding:0 8px; }"
    ).arg(C::BLUE50).arg(C::BLUE);

    for (int i = 0; i < 8; ++i) {
        QPushButton *btn = new QPushButton(ITEMS[i].label, menuBox);
        btn->setFixedHeight(38);
        btn->setIcon(SvgIcon::icon(ITEMS[i].icon, 12,
                                   i == 0 ? QColor(C::BLUE) : QColor(C::INK700)));
        btn->setIconSize(QSize(12, 12));
        btn->setStyleSheet(i == 0 ? menuActiveStyle : menuInactiveStyle);
        int idx = i;
        connect(btn, &QPushButton::clicked, this, [this, idx]{ onMenuClicked(idx); });
        m_menuBtns[i] = btn;
        ml->addWidget(btn);
    }
    ml->addStretch();
    root->addWidget(menuBox);

    // ── Right panel ────────────────────────────────────────────────
    QWidget *rightBox = new QWidget(this);
    rightBox->setStyleSheet(QString(
        "QWidget#rightBox{ background:#fff; border:1px solid %1; border-radius:6px; }").arg(C::BORDER));
    rightBox->setObjectName("rightBox");
    QVBoxLayout *rl = new QVBoxLayout(rightBox);
    rl->setContentsMargins(0, 0, 0, 0);
    rl->setSpacing(0);

    // Title bar
    QWidget *titleBar = new QWidget(rightBox);
    titleBar->setFixedHeight(36);
    titleBar->setStyleSheet(QString(
        "background:%1; border-bottom:1px solid %2; border-radius:6px 6px 0 0;")
        .arg(C::SURFALT).arg(C::BORDER));
    QHBoxLayout *tl = new QHBoxLayout(titleBar);
    tl->setContentsMargins(14, 0, 14, 0);
    m_panelTitle = new QLabel(ITEMS[0].label, titleBar);
    m_panelTitle->setStyleSheet(QString(
        "font-size:13px; font-weight:600; color:%1; border:none;").arg(C::INK800));
    tl->addWidget(m_panelTitle);
    rl->addWidget(titleBar);

    // Stacked panels
    m_stack = new QStackedWidget(rightBox);
    m_stack->setStyleSheet("background:transparent;");
    m_stack->addWidget(buildFuncPanel());    // 0 仪器设置
    m_stack->addWidget(buildQcPanel());      // 1 质控设置
    m_stack->addWidget(buildProjPanel());    // 2 项目设置
    m_stack->addWidget(buildPrintPanel());   // 3 打印设置
    m_stack->addWidget(buildUsersPanel());   // 4 用户设置
    m_stack->addWidget(buildSysPanel());     // 5 系统设置
    m_stack->addWidget(buildAboutPanel());   // 6 关于仪器
    m_stack->addWidget(buildFactoryPanel()); // 7 厂家模式
    rl->addWidget(m_stack, 1);

    root->addWidget(rightBox, 1);
}

// ══════════════════════════════════════════════════════════════════
//  Menu switching
// ══════════════════════════════════════════════════════════════════
void SettingsTab::onMenuClicked(int idx)
{
    switchMenu(idx);
}

void SettingsTab::switchMenu(int idx)
{
    static const char *LABELS[8] = {
        "仪器设置","质控设置","项目设置","打印设置",
        "用户设置","系统设置","关于仪器","厂家模式"
    };
    static const char *ICONS[8] = {
        "flask","chart","plate","print","user","settings","eye","dot"
    };

    QString menuInactive = QString(
        "QPushButton{ background:transparent; color:%1; font-size:12px; font-weight:400;"
        "  border:none; border-radius:4px; text-align:left; padding:0 8px; }"
        "QPushButton:hover{ background:%2; }").arg(C::INK700).arg(C::BLUE50);
    QString menuActive = QString(
        "QPushButton{ background:%1; color:%2; font-size:12px; font-weight:600;"
        "  border:none; border-radius:4px; text-align:left; padding:0 8px; }"
    ).arg(C::BLUE50).arg(C::BLUE);

    for (int i = 0; i < 8; ++i) {
        bool active = (i == idx);
        m_menuBtns[i]->setStyleSheet(active ? menuActive : menuInactive);
        m_menuBtns[i]->setIcon(SvgIcon::icon(ICONS[i], 12,
                                              active ? QColor(C::BLUE) : QColor(C::INK700)));
    }
    m_panelTitle->setText(LABELS[idx]);
    m_stack->setCurrentIndex(idx);
    m_activeMenu = idx;
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 仪器设置
// ══════════════════════════════════════════════════════════════════
QWidget *SettingsTab::buildFuncPanel()
{
    QWidget *inner = new QWidget;
    inner->setStyleSheet("background:#fff;");
    QVBoxLayout *vl = new QVBoxLayout(inner);
    vl->setContentsMargins(14, 8, 14, 8);
    vl->setSpacing(0);

    struct FuncRow {
        const char *label;
        bool on;
        bool hasIncub;
        bool hasRack;
        int  rackIdx; // 1 or 2
    };
    static const FuncRow ROWS[6] = {
        {"样本扫码",         true,  false, false, 0},
        {"样本液探",         true,  false, false, 0},
        {"样本ID自动生成",   false, false, false, 0},
        {"启用恒温孵育",     true,  true,  false, 0},
        {"样本架01启用",     true,  false, true,  1},
        {"样本架02启用",     false, false, true,  2},
    };

    for (int i = 0; i < 6; ++i) {
        const FuncRow &fr = ROWS[i];
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 10, 0, 10);
        hl->setSpacing(8);

        QLabel *name = new QLabel(fr.label, row);
        name->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK700));
        hl->addWidget(name, 1);

        // Optional badge
        if (fr.hasIncub) {
            m_incubLabel = makeBadge("37℃ 点击修改", row);
            m_incubLabel->setVisible(fr.on);
            hl->addWidget(m_incubLabel);
        }
        if (fr.hasRack) {
            QLabel *&rLbl = (fr.rackIdx == 1) ? m_rack01Label : m_rack02Label;
            rLbl = makeBadge("采血管样本架 点击修改", row);
            rLbl->setVisible(fr.on);
            hl->addWidget(rLbl);
        }

        QLabel *statusLbl = makeStatusLabel(fr.on ? "已启用" : "已关闭", fr.on, row);
        hl->addWidget(statusLbl);

        ToggleSwitch *tog = makeToggle(fr.on, row);
        bool hasIncub = fr.hasIncub;
        bool hasRack  = fr.hasRack;
        int  rackIdx  = fr.rackIdx;
        connect(tog, &ToggleSwitch::toggled, this, [=](bool v) {
            statusLbl->setText(v ? "已启用" : "已关闭");
            statusLbl->setStyleSheet(QString("font-size:11px; color:%1; border:none;")
                                     .arg(v ? C::OK : C::INK300));
            if (hasIncub && m_incubLabel)   m_incubLabel->setVisible(v);
            if (hasRack) {
                QLabel *rLbl = (rackIdx == 1) ? m_rack01Label : m_rack02Label;
                if (rLbl) rLbl->setVisible(v);
            }
        });
        hl->addWidget(tog);
        vl->addWidget(row);
    }
    vl->addStretch();
    return wrapScroll(inner);
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 质控设置
// ══════════════════════════════════════════════════════════════════
QWidget *SettingsTab::buildQcPanel()
{
    QTabWidget *tabs = new QTabWidget;
    tabs->setStyleSheet(QString(
        "QTabWidget::pane{ border:none; background:#fff; }"
        "QTabBar::tab{ height:32px; padding:0 18px; font-size:12px;"
        "  background:transparent; border:none; color:%1; }"
        "QTabBar::tab:selected{ color:%2; border-bottom:2px solid %2; font-weight:600; }"
        "QTabBar::tab:!selected{ border-bottom:2px solid transparent; }"
    ).arg(C::INK500).arg(C::BLUE));

    // ── Tab 0: 质控图谱（暂未开发） ──
    {
        QWidget *chartTab = new QWidget;
        chartTab->setStyleSheet("background:#fff;");
        QVBoxLayout *vl = new QVBoxLayout(chartTab);
        vl->setAlignment(Qt::AlignCenter);
        QLabel *ph = new QLabel("质控图谱（开发中）", chartTab);
        ph->setAlignment(Qt::AlignCenter);
        ph->setStyleSheet(QString("font-size:13px; color:%1;").arg(C::INK300));
        vl->addWidget(ph);
        tabs->addTab(chartTab, "质控图谱");
    }

    // ── Tab 1: 质控规则（暂未开发） ──
    {
        QWidget *rulesTab = new QWidget;
        rulesTab->setStyleSheet("background:#fff;");
        QVBoxLayout *vl = new QVBoxLayout(rulesTab);
        vl->setAlignment(Qt::AlignCenter);
        QLabel *ph = new QLabel("质控规则（开发中）", rulesTab);
        ph->setAlignment(Qt::AlignCenter);
        ph->setStyleSheet(QString("font-size:13px; color:%1;").arg(C::INK300));
        vl->addWidget(ph);
        tabs->addTab(rulesTab, "质控规则");
    }

    // ── Tab 2: 质控品信息 ──
    {
        QWidget *infoTab = new QWidget;
        infoTab->setStyleSheet("background:#fff;");
        QVBoxLayout *vl = new QVBoxLayout(infoTab);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(0);

        // ── 全选 toolbar ──
        QWidget *selBar = new QWidget(infoTab);
        selBar->setFixedHeight(32);
        selBar->setStyleSheet(QString("background:%1; border-bottom:1px solid %2;")
                              .arg(C::SURFALT).arg(C::BORDER));
        QHBoxLayout *sl = new QHBoxLayout(selBar);
        sl->setContentsMargins(8, 0, 8, 0);
        sl->setSpacing(0);
        QCheckBox *allChk = new QCheckBox("全选", selBar);
        allChk->setStyleSheet(QString("font-size:12px; color:%1;").arg(C::INK700));
        sl->addWidget(allChk);
        sl->addStretch();
        vl->addWidget(selBar);

        // ── Table: col0=checkbox, col1-5=data ──
        m_qcTable = new QTableWidget(0, 6, infoTab);
        QStringList qcHdr;
        qcHdr << "" << "质控项目" << "质控品批号" << "水平值" << "靶值" << "质控范围";
        m_qcTable->setHorizontalHeaderLabels(qcHdr);
        m_qcTable->horizontalHeader()->setStretchLastSection(true);
        m_qcTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        m_qcTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
        m_qcTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
        m_qcTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
        m_qcTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
        m_qcTable->setColumnWidth(0, 36);
        m_qcTable->setColumnWidth(1, 90);
        m_qcTable->setColumnWidth(2, 105);
        m_qcTable->setColumnWidth(3, 52);
        m_qcTable->setColumnWidth(4, 80);
        m_qcTable->verticalHeader()->hide();
        m_qcTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_qcTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_qcTable->setShowGrid(false);
        m_qcTable->setStyleSheet(QString(
            "QTableWidget{ border:none; font-size:12px; background:#fff; }"
            "QTableWidget::item{ padding:6px 4px; border-bottom:1px solid #F5F7FA; }"
            "QTableWidget::item:selected{ background:%1; color:%2; }"
            "QHeaderView::section{ background:%3; font-size:11px; font-weight:600;"
            "  color:%4; border:none; padding:6px 4px; border-bottom:1px solid %5; }"
        ).arg(C::BLUE50).arg(C::INK800).arg(C::SURFALT).arg(C::INK500).arg(C::BORDER));

        // Seed data
        struct QcItem { const char *proj; const char *lot; int lvl; double target; const char *unit; };
        static const QcItem QC_DATA[6] = {
            {"肌钙蛋白I","质控品A 2024-001",1,0.05,"ng/mL"},
            {"肌红蛋白","质控品A 2024-001",2,50,"ng/mL"},
            {"肌酸激酶MB","质控品B 2024-002",1,5,"ng/mL"},
            {"NT-proBNP","质控品A 2024-001",1,125,"pg/mL"},
            {"D-二聚体","质控品C 2024-003",1,0.3,"mg/L"},
            {"C反应蛋白","质控品B 2024-002",2,10,"mg/L"},
        };
        m_qcTable->setRowCount(6);
        for (int i = 0; i < 6; ++i) {
            const QcItem &qi = QC_DATA[i];
            // col 0: checkbox item
            QTableWidgetItem *chkItem = new QTableWidgetItem;
            chkItem->setCheckState(Qt::Unchecked);
            chkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            m_qcTable->setItem(i, 0, chkItem);
            // col 1-5: data
            m_qcTable->setItem(i, 1, new QTableWidgetItem(qi.proj));
            m_qcTable->setItem(i, 2, new QTableWidgetItem(qi.lot));
            m_qcTable->setItem(i, 3, new QTableWidgetItem(QString("L%1").arg(qi.lvl)));
            m_qcTable->setItem(i, 4, new QTableWidgetItem(
                QString("%1 %2").arg(qi.target).arg(qi.unit)));
            double sd = qi.target * 0.1;
            m_qcTable->setItem(i, 5, new QTableWidgetItem(
                QString("%1 ~ %2").arg(qi.target - sd, 0, 'f', 3)
                                  .arg(qi.target + sd, 0, 'f', 3)));
        }

        // 全选 logic
        connect(allChk, &QCheckBox::stateChanged, this, [this](int state) {
            Qt::CheckState cs = (state == Qt::Checked) ? Qt::Checked : Qt::Unchecked;
            for (int r = 0; r < m_qcTable->rowCount(); ++r) {
                QTableWidgetItem *it = m_qcTable->item(r, 0);
                if (it) it->setCheckState(cs);
            }
        });
        // Update 全选 checkbox when individual items change
        connect(m_qcTable, &QTableWidget::itemChanged, this, [this, allChk](QTableWidgetItem *it) {
            if (it->column() != 0) return;
            int checked = 0, total = m_qcTable->rowCount();
            for (int r = 0; r < total; ++r) {
                QTableWidgetItem *ci = m_qcTable->item(r, 0);
                if (ci && ci->checkState() == Qt::Checked) ++checked;
            }
            allChk->blockSignals(true);
            if (checked == 0)         allChk->setCheckState(Qt::Unchecked);
            else if (checked == total) allChk->setCheckState(Qt::Checked);
            else                       allChk->setCheckState(Qt::PartiallyChecked);
            allChk->blockSignals(false);
        });

        connect(m_qcTable, &QTableWidget::itemSelectionChanged,
                this, &SettingsTab::onQcSelectionChanged);
        vl->addWidget(m_qcTable, 1);

        // Toolbar
        QWidget *toolbar = new QWidget(infoTab);
        toolbar->setStyleSheet(QString("background:#fff; border-top:1px solid %1;").arg(C::BORDER));
        QHBoxLayout *tl = new QHBoxLayout(toolbar);
        tl->setContentsMargins(6, 6, 6, 6);
        tl->setSpacing(6);

        QPushButton *addBtn = new QPushButton("新增", toolbar);
        addBtn->setStyleSheet(smallBtnStyle(true));
        QPushButton *usbBtn = new QPushButton("U盘导入", toolbar);
        usbBtn->setStyleSheet(smallBtnStyle(false));
        QPushButton *scanBtn = new QPushButton("扫码导入", toolbar);
        scanBtn->setStyleSheet(smallBtnStyle(false));
        m_editQcBtn = new QPushButton("编辑", toolbar);
        m_editQcBtn->setStyleSheet(smallBtnStyle(false));
        m_editQcBtn->setEnabled(false);
        m_delQcBtn = new QPushButton("删除", toolbar);
        m_delQcBtn->setStyleSheet(
            "QPushButton{ height:26px; padding:0 10px; font-size:11px;"
            "  background:#fff; border:1px solid #D6DEE6; border-radius:4px; }"
            "QPushButton:enabled{ color:#C0392B; }"
            "QPushButton:disabled{ color:#A5B0BC; }");
        m_delQcBtn->setEnabled(false);
        tl->addWidget(addBtn);
        tl->addWidget(usbBtn);
        tl->addWidget(scanBtn);
        tl->addWidget(m_editQcBtn);
        tl->addWidget(m_delQcBtn);
        tl->addStretch();

        // Pagination placeholders
        QPushButton *prevBtn = new QPushButton("‹", toolbar);
        prevBtn->setFixedSize(24, 24);
        prevBtn->setStyleSheet(smallBtnStyle(false));
        QLabel *pgLbl = new QLabel("1 / 2", toolbar);
        pgLbl->setStyleSheet(QString("font-size:11px; color:%1;").arg(C::INK500));
        pgLbl->setFixedWidth(50);
        pgLbl->setAlignment(Qt::AlignCenter);
        QPushButton *nextBtn = new QPushButton("›", toolbar);
        nextBtn->setFixedSize(24, 24);
        nextBtn->setStyleSheet(smallBtnStyle(false));
        tl->addWidget(prevBtn);
        tl->addWidget(pgLbl);
        tl->addWidget(nextBtn);
        vl->addWidget(toolbar);

        tabs->addTab(infoTab, "质控品信息");
    }

    tabs->setCurrentIndex(2); // default to info tab
    return tabs;
}

void SettingsTab::onQcSelectionChanged()
{
    bool hasSel = !m_qcTable->selectionModel()->selectedRows().isEmpty();
    if (m_editQcBtn) m_editQcBtn->setEnabled(hasSel);
    if (m_delQcBtn)  m_delQcBtn->setEnabled(hasSel);
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 项目设置
// ══════════════════════════════════════════════════════════════════
// ─── Helper: build one reference-range card ──────────────────────
static QWidget *makeRefCard(const QString &gender, const QString &ageStr,
                             const QString &lo, const QString &hi,
                             const QString &unit, QWidget *cardsInner,
                             QVBoxLayout *cardsVl)
{
    QWidget *card = new QWidget(cardsInner);
    card->setObjectName("refCard");
    card->setStyleSheet(
        "QWidget#refCard{ background:#fff; border:1px solid #D6DEE6;"
        "  border-radius:6px; margin-bottom:6px; }"
        "QWidget#refCard QLabel{ border:none; }"
        "QWidget#refCard QLineEdit{ border:none; background:transparent; }"
    );

    QVBoxLayout *cl = new QVBoxLayout(card);
    cl->setContentsMargins(10, 8, 10, 8);
    cl->setSpacing(6);

    // ── Header row: badges + × ──
    QWidget *hdr = new QWidget(card);
    hdr->setStyleSheet("background:transparent;");
    QHBoxLayout *hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(0, 0, 0, 0);
    hl->setSpacing(6);

    auto badge = [&](const QString &txt, const QString &bg, const QString &fg) {
        QLabel *b = new QLabel(txt, hdr);
        b->setStyleSheet(QString(
            "background:%1; color:%2; border-radius:10px;"
            "font-size:11px; padding:1px 10px; border:1px solid %1;")
            .arg(bg).arg(fg));
        return b;
    };

    // Gender badge(s)
    if (gender == "全部") {
        hl->addWidget(badge("男", "#E3F2FD", "#1464A5"));
        hl->addWidget(badge("女", "#FCE4EC", "#C2185B"));
    } else if (gender == "男") {
        hl->addWidget(badge("男", "#E3F2FD", "#1464A5"));
    } else {
        hl->addWidget(badge("女", "#FCE4EC", "#C2185B"));
    }
    // Age badge
    hl->addWidget(badge(ageStr, "#F3E5F5", "#7B1FA2"));
    hl->addStretch();

    // × button
    QPushButton *delBtn = new QPushButton("×", hdr);
    delBtn->setFixedSize(20, 20);
    delBtn->setStyleSheet(
        "QPushButton{ background:transparent; color:#9CA3AF;"
        "  border:none; font-size:14px; font-weight:600; }"
        "QPushButton:hover{ color:#C0392B; }");
    hl->addWidget(delBtn);
    cl->addWidget(hdr);

    // ── Threshold row ──
    QWidget *threshRow = new QWidget(card);
    threshRow->setStyleSheet("background:transparent;");
    QHBoxLayout *thl = new QHBoxLayout(threshRow);
    thl->setContentsMargins(0, 0, 0, 0);
    thl->setSpacing(0);

    auto threshCell = [&](const QString &txt, bool center) {
        QLabel *l = new QLabel(txt, threshRow);
        l->setAlignment(center ? Qt::AlignCenter : Qt::AlignCenter);
        l->setStyleSheet(QString("font-size:12px; color:%1; font-weight:500;").arg("#334155"));
        return l;
    };
    QString loStr = lo.isEmpty() ? "—" : lo;
    QString hiStr = hi.isEmpty() ? "—" : hi;
    thl->addWidget(threshCell(QString("< %1").arg(loStr), true), 1);
    QFrame *div1 = new QFrame(threshRow);
    div1->setFrameShape(QFrame::VLine);
    div1->setStyleSheet("color:#D6DEE6;");
    thl->addWidget(div1);
    thl->addWidget(threshCell(QString("%1 ~ %2  %3").arg(loStr).arg(hiStr).arg(unit), true), 2);
    QFrame *div2 = new QFrame(threshRow);
    div2->setFrameShape(QFrame::VLine);
    div2->setStyleSheet("color:#D6DEE6;");
    thl->addWidget(div2);
    thl->addWidget(threshCell(QString("> %1").arg(hiStr), true), 1);
    cl->addWidget(threshRow);

    // ── Conclusion labels (editable) ──
    QWidget *conclRow = new QWidget(card);
    conclRow->setStyleSheet("background:transparent;");
    QHBoxLayout *crl = new QHBoxLayout(conclRow);
    crl->setContentsMargins(0, 0, 0, 0);
    crl->setSpacing(4);

    auto conclEdit = [&](const QString &txt, const QString &bg, const QString &fg) {
        QLineEdit *e = new QLineEdit(txt, conclRow);
        e->setAlignment(Qt::AlignCenter);
        e->setStyleSheet(QString(
            "QLineEdit{ background:%1; color:%2; border-radius:4px;"
            "  font-size:12px; padding:4px 0; border:none; }").arg(bg).arg(fg));
        return e;
    };
    crl->addWidget(conclEdit("低于参考范围", "#DBEAFE", "#1464A5"), 1);
    crl->addWidget(conclEdit("正常",         "#D1FAE5", "#065F46"), 2);
    crl->addWidget(conclEdit("高于参考范围", "#FEE2E2", "#991B1B"), 1);
    cl->addWidget(conclRow);

    // Delete button: remove card from layout
    QObject::connect(delBtn, &QPushButton::clicked, delBtn, [card, cardsVl]() {
        cardsVl->removeWidget(card);
        card->deleteLater();
    });
    return card;
}

// ─── Helper: open "新增自定义范围" dialog ──────────────────────────
static void openAddRefDialog(QWidget *parent, QWidget *cardsInner,
                              QVBoxLayout *cardsVl, const QString &unit)
{
    QDialog *dlg = new QDialog(parent);
    dlg->setWindowTitle("新增自定义范围");
    dlg->setFixedWidth(400);
    dlg->setStyleSheet("background:#fff;");

    QVBoxLayout *vl = new QVBoxLayout(dlg);
    vl->setContentsMargins(20, 16, 20, 16);
    vl->setSpacing(12);

    QString iStyle =
        "QLineEdit{ height:28px; padding:0 8px; font-size:12px;"
        "  border:1px solid #D6DEE6; border-radius:4px; background:#fff; color:#1F2A37; }";
    QString rowLblStyle = "font-size:12px; color:#334155; min-width:56px;";

    // ── Gender row ────────────────────────────────────────────────
    QWidget *gRow = new QWidget(dlg);
    QHBoxLayout *ghl = new QHBoxLayout(gRow);
    ghl->setContentsMargins(0,0,0,0); ghl->setSpacing(10);
    QLabel *gLbl = new QLabel("性别", gRow);
    gLbl->setStyleSheet(rowLblStyle);
    ghl->addWidget(gLbl);

    // Segment: 全部 / 男 / 女
    QPushButton *gAll = new QPushButton("全部", gRow);
    QPushButton *gMale = new QPushButton("男",  gRow);
    QPushButton *gFem  = new QPushButton("女",  gRow);
    QString segBase =
        "QPushButton{ font-size:12px; height:26px; padding:0 14px;"
        "  border:1px solid #D6DEE6; background:#fff; color:#334155; }"
        "QPushButton:checked{ background:#1464A5; color:#fff; border-color:#0f4f82; }";
    gAll->setStyleSheet(segBase  + "QPushButton{ border-radius:4px 0 0 4px; }");
    gMale->setStyleSheet(segBase + "QPushButton{ border-radius:0; border-left:none; }");
    gFem->setStyleSheet(segBase  + "QPushButton{ border-radius:0 4px 4px 0; border-left:none; }");
    for (auto *b : {gAll, gMale, gFem}) b->setCheckable(true);
    gAll->setChecked(true);
    auto syncGender = [gAll, gMale, gFem](QPushButton *clicked){
        gAll->setChecked(clicked == gAll);
        gMale->setChecked(clicked == gMale);
        gFem->setChecked(clicked == gFem);
    };
    QObject::connect(gAll,  &QPushButton::clicked, dlg, [syncGender, gAll] { syncGender(gAll);  });
    QObject::connect(gMale, &QPushButton::clicked, dlg, [syncGender, gMale]{ syncGender(gMale); });
    QObject::connect(gFem,  &QPushButton::clicked, dlg, [syncGender, gFem] { syncGender(gFem);  });
    ghl->addWidget(gAll); ghl->addWidget(gMale); ghl->addWidget(gFem);
    ghl->addStretch();
    vl->addWidget(gRow);

    // ── Age row ───────────────────────────────────────────────────
    QWidget *aRow = new QWidget(dlg);
    QHBoxLayout *ahl = new QHBoxLayout(aRow);
    ahl->setContentsMargins(0,0,0,0); ahl->setSpacing(10);
    QLabel *aLbl = new QLabel("年龄", aRow);
    aLbl->setStyleSheet(rowLblStyle);
    ahl->addWidget(aLbl);
    QCheckBox *ageAllChk = new QCheckBox("全年龄", aRow);
    ageAllChk->setChecked(true);
    ageAllChk->setStyleSheet("font-size:12px; color:#334155;");
    ahl->addWidget(ageAllChk);
    ahl->addStretch();
    vl->addWidget(aRow);

    // ── Threshold row ─────────────────────────────────────────────
    QWidget *tRow = new QWidget(dlg);
    QHBoxLayout *thl = new QHBoxLayout(tRow);
    thl->setContentsMargins(0,0,0,0); thl->setSpacing(16);

    QWidget *loW = new QWidget(tRow);
    QHBoxLayout *lohl = new QHBoxLayout(loW);
    lohl->setContentsMargins(0,0,0,0); lohl->setSpacing(6);
    QLabel *loLbl = new QLabel("参考下限", loW);
    loLbl->setStyleSheet(rowLblStyle);
    QLineEdit *loEd = new QLineEdit(loW);
    loEd->setFixedWidth(90); loEd->setStyleSheet(iStyle);
    lohl->addWidget(loLbl); lohl->addWidget(loEd);

    QWidget *hiW = new QWidget(tRow);
    QHBoxLayout *hihl = new QHBoxLayout(hiW);
    hihl->setContentsMargins(0,0,0,0); hihl->setSpacing(6);
    QLabel *hiLbl = new QLabel("参考上限", hiW);
    hiLbl->setStyleSheet(rowLblStyle);
    QLineEdit *hiEd = new QLineEdit(hiW);
    hiEd->setFixedWidth(90); hiEd->setStyleSheet(iStyle);
    hihl->addWidget(hiLbl); hihl->addWidget(hiEd);

    thl->addWidget(loW); thl->addWidget(hiW); thl->addStretch();
    vl->addWidget(tRow);

    // ── Three-zone preview bar ────────────────────────────────────
    QWidget *preview = new QWidget(dlg);
    preview->setFixedHeight(64);
    QHBoxLayout *phl = new QHBoxLayout(preview);
    phl->setContentsMargins(0,0,0,0); phl->setSpacing(0);

    struct Zone { const char *bg; const char *border; const char *hdr; const char *def; const char *color; };
    static const Zone ZONES[3] = {
        {"#DBEAFE","#93C5FD","< —","低于参考范围","#1D4ED8"},
        {"#D1FAE5","#6EE7B7","— ~ —","正常","#065F46"},
        {"#FEE2E2","#FCA5A5","> —","高于参考范围","#991B1B"},
    };
    for (int z = 0; z < 3; ++z) {
        const Zone &zn = ZONES[z];
        QWidget *cell = new QWidget(preview);
        cell->setStyleSheet(QString("background:%1; border:1px solid %2;").arg(zn.bg).arg(zn.border));
        QVBoxLayout *cvl = new QVBoxLayout(cell);
        cvl->setContentsMargins(6,4,6,4); cvl->setSpacing(2);
        cvl->setAlignment(Qt::AlignCenter);
        QLabel *hdr = new QLabel(zn.hdr, cell);
        hdr->setAlignment(Qt::AlignCenter);
        hdr->setStyleSheet(QString("font-size:10px; color:%1; background:transparent; border:none;").arg(zn.color));
        QLineEdit *concl = new QLineEdit(zn.def, cell);
        concl->setAlignment(Qt::AlignCenter);
        concl->setStyleSheet(QString(
            "QLineEdit{ font-size:12px; font-weight:600; color:%1;"
            "  background:transparent; border:none; border-bottom:1px dashed %1; }").arg(zn.color));
        cvl->addWidget(hdr);
        cvl->addWidget(concl);
        phl->addWidget(cell, 1);
    }
    vl->addWidget(preview);

    // Hint text
    QLabel *hint = new QLabel("点击结论文字可直接编辑", dlg);
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet("font-size:11px; color:#7A8898; border:none;");
    vl->addWidget(hint);

    // ── Buttons ───────────────────────────────────────────────────
    QWidget *btnRow = new QWidget(dlg);
    QHBoxLayout *bhl = new QHBoxLayout(btnRow);
    bhl->setContentsMargins(0,0,0,0); bhl->setSpacing(8);
    bhl->addStretch();
    QPushButton *cancelBtn  = new QPushButton("取消",    btnRow);
    QPushButton *confirmBtn = new QPushButton("确认添加", btnRow);
    cancelBtn->setFixedHeight(30);
    confirmBtn->setFixedHeight(30);
    cancelBtn->setStyleSheet(
        "QPushButton{ padding:0 18px; font-size:12px; background:#fff; color:#334155;"
        "  border:1px solid #D6DEE6; border-radius:4px; }"
        "QPushButton:hover{ background:#EEF4F9; }");
    confirmBtn->setStyleSheet(
        "QPushButton{ padding:0 18px; font-size:12px; background:#1464A5; color:#fff;"
        "  border:1px solid #0f4f82; border-radius:4px; }"
        "QPushButton:hover{ background:#1056a0; }");
    bhl->addWidget(cancelBtn); bhl->addWidget(confirmBtn);
    vl->addWidget(btnRow);

    QObject::connect(cancelBtn,  &QPushButton::clicked, dlg, &QDialog::reject);
    QObject::connect(confirmBtn, &QPushButton::clicked, dlg, &QDialog::accept);

    if (dlg->exec() != QDialog::Accepted) return;

    // Build gender string from segment selection
    QString gender = gAll->isChecked() ? "全部" : gMale->isChecked() ? "男" : "女";

    // Build age string
    QString ageStr = ageAllChk->isChecked() ? "全年龄段" : "自定义年龄";

    QString lo = loEd->text().trimmed();
    QString hi = hiEd->text().trimmed();

    QWidget *card = makeRefCard(gender, ageStr, lo, hi, unit, cardsInner, cardsVl);
    // Insert before the last stretch (index = count-1)
    int pos = cardsVl->count() > 0 ? cardsVl->count() - 1 : 0;
    cardsVl->insertWidget(pos, card);
}

QWidget *SettingsTab::buildProjPanel()
{
    QWidget *outer = new QWidget;
    outer->setStyleSheet("background:#fff;");
    QHBoxLayout *hl = new QHBoxLayout(outer);
    hl->setContentsMargins(0, 0, 0, 0);
    hl->setSpacing(0);

    // ── Left: project list ──
    QWidget *projList = new QWidget(outer);
    projList->setFixedWidth(120);
    projList->setStyleSheet(QString(
        "QWidget{ background:%1; border-right:1px solid %2; }").arg(C::SURFALT).arg(C::BORDER));
    QVBoxLayout *pl = new QVBoxLayout(projList);
    pl->setContentsMargins(4, 8, 4, 8);
    pl->setSpacing(2);

    // U盘导入 button at top
    QPushButton *usbImportBtn = new QPushButton("  U盘导入", projList);
    usbImportBtn->setFixedHeight(30);
    usbImportBtn->setIcon(SvgIcon::icon("export", 12, QColor(C::BLUE)));
    usbImportBtn->setIconSize(QSize(12,12));
    usbImportBtn->setStyleSheet(QString(
        "QPushButton{ background:#fff; color:%1; font-size:11px;"
        "  border:1px solid %2; border-radius:4px; text-align:left; padding:0 8px; }"
        "QPushButton:hover{ background:%3; }").arg(C::BLUE).arg(C::BORDER).arg(C::BLUE50));
    pl->addWidget(usbImportBtn);

    struct ProjItem { const char *code; const char *name; const char *num; };
    static const ProjItem PROJS[6] = {
        {"TnI","肌钙蛋白I","001"},{"Myo","肌红蛋白","002"},{"CKMB","肌酸激酶MB","003"},
        {"NTproBNP","NT-proBNP","004"},{"Ddimer","D-二聚体","005"},{"CRP","C反应蛋白","006"},
    };

    QStackedWidget *rightStack = new QStackedWidget(outer);

    QString projBtnActive = QString(
        "QPushButton{ background:%1; color:%2; font-size:12px; font-weight:600;"
        "  border:none; border-radius:4px; text-align:left; padding:0 8px; }"
    ).arg(C::BLUE50).arg(C::BLUE);
    QString projBtnInactive = QString(
        "QPushButton{ background:transparent; color:%1; font-size:12px; font-weight:400;"
        "  border:none; border-radius:4px; text-align:left; padding:0 8px; }"
        "QPushButton:hover{ background:#EBEEF1; }"
    ).arg(C::INK700);

    QPushButton *projBtns[6];
    for (int i = 0; i < 6; ++i) {
        // Multi-line button: num + name
        QPushButton *btn = new QPushButton(projList);
        btn->setFixedHeight(48);
        btn->setStyleSheet(i == 0 ? projBtnActive : projBtnInactive);
        // Use a widget with two labels
        QWidget *btnInner = new QWidget(btn);
        btnInner->setAttribute(Qt::WA_TransparentForMouseEvents);
        QVBoxLayout *bil = new QVBoxLayout(btnInner);
        bil->setContentsMargins(8, 4, 4, 4);
        bil->setSpacing(1);
        QLabel *numLbl = new QLabel(QString("%1").arg(PROJS[i].num), btnInner);
        numLbl->setStyleSheet("font-size:10px; color:#9CA3AF; background:transparent;");
        QLabel *nameLbl = new QLabel(PROJS[i].name, btnInner);
        nameLbl->setStyleSheet(QString("font-size:12px; color:%1; font-weight:600;"
                                        "background:transparent;").arg(i==0?C::BLUE:C::INK700));
        QLabel *codeLbl = new QLabel(PROJS[i].code, btnInner);
        codeLbl->setStyleSheet("font-size:10px; color:#9CA3AF; background:transparent;");
        bil->addWidget(numLbl);
        bil->addWidget(nameLbl);
        bil->addWidget(codeLbl);
        btnInner->setGeometry(0, 0, 112, 48);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        projBtns[i] = btn;
        pl->addWidget(btn);

        // ── Right panel for this project ──
        QWidget *rp = new QWidget;
        rp->setStyleSheet("background:#fff;");
        QVBoxLayout *rvl = new QVBoxLayout(rp);
        rvl->setContentsMargins(0, 0, 0, 0);
        rvl->setSpacing(0);

        QTabWidget *resultTabs = new QTabWidget(rp);
        resultTabs->setStyleSheet(QString(
            "QTabWidget::pane{ border:none; }"
            "QTabBar::tab{ height:30px; padding:0 18px; font-size:12px;"
            "  background:transparent; border:none; color:%1; }"
            "QTabBar::tab:selected{ color:%2; border-bottom:2px solid %2; font-weight:600; }"
            "QTabBar::tab:!selected{ border-bottom:2px solid transparent; }"
        ).arg(C::INK500).arg(C::BLUE));

        for (int r = 1; r <= 3; ++r) {
            // Each result tab: scroll area wrapping content
            QWidget *rt = new QWidget;
            rt->setStyleSheet("background:#fff;");
            QVBoxLayout *rtl = new QVBoxLayout(rt);
            rtl->setContentsMargins(14, 10, 14, 10);
            rtl->setSpacing(0);

            QString unit = (i == 3) ? "pg/mL" : (i >= 4) ? "mg/L" : "ng/mL";

            // ── Row 1: 小数位数 + 浓度单位 + 保存 ──
            {
                QWidget *row = new QWidget(rt);
                row->setStyleSheet("background:transparent;");
                QHBoxLayout *rhl = new QHBoxLayout(row);
                rhl->setContentsMargins(0, 0, 0, 8);
                rhl->setSpacing(12);

                QLabel *decLbl = new QLabel("小数位数", row);
                decLbl->setStyleSheet(QString("font-size:12px; color:%1;").arg(C::INK500));
                QComboBox *decCombo = new QComboBox(row);
                decCombo->setFixedWidth(70);
                decCombo->addItems(QStringList() << "0" << "1" << "2" << "3");
                decCombo->setCurrentIndex(2);
                decCombo->setStyleSheet(inputStyle());

                QLabel *uLbl = new QLabel("浓度单位", row);
                uLbl->setStyleSheet(QString("font-size:12px; color:%1;").arg(C::INK500));
                QLineEdit *unitEd = new QLineEdit(unit, row);
                unitEd->setFixedWidth(90);
                unitEd->setStyleSheet(inputStyle());

                QPushButton *saveBtn = new QPushButton("保存", row);
                saveBtn->setFixedSize(56, 28);
                saveBtn->setStyleSheet(
                    "QPushButton{ background:#1464A5; color:#fff; font-size:12px;"
                    "  border:none; border-radius:4px; }"
                    "QPushButton:hover{ background:#1056a0; }");

                rhl->addWidget(decLbl);
                rhl->addWidget(decCombo);
                rhl->addSpacing(16);
                rhl->addWidget(uLbl);
                rhl->addWidget(unitEd);
                rhl->addStretch();
                rhl->addWidget(saveBtn);
                rtl->addWidget(row);
            }

            // ── Row 2: 换算浓度单位 + 系数K + 系数B ──
            {
                QWidget *row = new QWidget(rt);
                row->setStyleSheet("background:transparent; border-bottom:1px solid #F0F2F5;");
                QHBoxLayout *rhl = new QHBoxLayout(row);
                rhl->setContentsMargins(0, 0, 0, 10);
                rhl->setSpacing(8);

                QLabel *cvLbl = new QLabel("换算浓度单位", row);
                cvLbl->setStyleSheet(QString("font-size:12px; color:%1;").arg(C::INK500));
                QLineEdit *cvUnitEd = new QLineEdit(row);
                cvUnitEd->setPlaceholderText("手工输入");
                cvUnitEd->setFixedWidth(80);
                cvUnitEd->setStyleSheet(inputStyle());

                QLabel *kLbl = new QLabel("系数 K", row);
                kLbl->setStyleSheet(QString("font-size:12px; color:%1;").arg(C::INK500));
                QLineEdit *kEd = new QLineEdit("1", row);
                kEd->setFixedWidth(60);
                kEd->setStyleSheet(inputStyle());

                QLabel *bLbl = new QLabel("系数 B", row);
                bLbl->setStyleSheet(QString("font-size:12px; color:%1;").arg(C::INK500));
                QLineEdit *bEd = new QLineEdit("0", row);
                bEd->setFixedWidth(60);
                bEd->setStyleSheet(inputStyle());

                rhl->addWidget(cvLbl);
                rhl->addWidget(cvUnitEd);
                rhl->addSpacing(8);
                rhl->addWidget(kLbl);
                rhl->addWidget(kEd);
                rhl->addSpacing(8);
                rhl->addWidget(bLbl);
                rhl->addWidget(bEd);
                rhl->addStretch();
                rtl->addWidget(row);
            }

            // ── 参考范围规则 section ──
            {
                QWidget *ruleHdr = new QWidget(rt);
                ruleHdr->setStyleSheet("background:transparent; margin-top:8px;");
                QHBoxLayout *rhl = new QHBoxLayout(ruleHdr);
                rhl->setContentsMargins(0, 8, 0, 6);
                rhl->setSpacing(8);
                QLabel *rLbl = new QLabel("参考范围规则", ruleHdr);
                rLbl->setStyleSheet(QString(
                    "font-size:13px; font-weight:700; color:%1;").arg(C::INK800));
                QLabel *togLbl = new QLabel("自定义规则用于结论计算", ruleHdr);
                togLbl->setStyleSheet(QString("font-size:11px; color:%1;").arg(C::INK400));
                ToggleSwitch *tog = makeToggle(false, ruleHdr);
                QPushButton *addCardBtn = new QPushButton("+ 自定义范围", ruleHdr);
                addCardBtn->setStyleSheet(
                    "QPushButton{ height:26px; padding:0 12px; font-size:12px;"
                    "  background:#1464A5; color:#fff; border:none; border-radius:4px; }"
                    "QPushButton:hover{ background:#1056a0; }");
                rhl->addWidget(rLbl);
                rhl->addStretch();
                rhl->addWidget(togLbl);
                rhl->addWidget(tog);
                rhl->addSpacing(8);
                rhl->addWidget(addCardBtn);
                rtl->addWidget(ruleHdr);

                // Default reference range box
                QWidget *defBox = new QWidget(rt);
                defBox->setStyleSheet(QString(
                    "QWidget{ background:#F8FAFC; border:1px solid %1; border-radius:6px; }"
                    "QWidget QLabel{ border:none; }"
                    "QWidget QLineEdit{ border:1px solid %1; background:#fff;"
                    "  border-radius:4px; height:26px; padding:0 6px; font-size:12px; }"
                ).arg(C::BORDER));
                QVBoxLayout *dbvl = new QVBoxLayout(defBox);
                dbvl->setContentsMargins(10, 8, 10, 8);
                dbvl->setSpacing(6);

                QLabel *defTitle = new QLabel(
                    "<b>默认参考范围</b>  <span style='color:#9CA3AF;font-size:11px;'>"
                    "适用于所有未匹配自定义规则的样本</span>", defBox);
                defTitle->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK700));
                dbvl->addWidget(defTitle);

                QWidget *defRow = new QWidget(defBox);
                defRow->setStyleSheet("background:transparent;");
                QHBoxLayout *drl = new QHBoxLayout(defRow);
                drl->setContentsMargins(0,0,0,0); drl->setSpacing(10);
                QLabel *loLbl = new QLabel("下限", defRow);
                loLbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK500));
                QLineEdit *defLo = new QLineEdit("—", defRow); defLo->setFixedWidth(80);
                QLabel *hiLbl = new QLabel("上限", defRow);
                hiLbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK500));
                QLineEdit *defHi = new QLineEdit("—", defRow); defHi->setFixedWidth(80);
                QLabel *uLbl2 = new QLabel(QString("单位: %1").arg(unit), defRow);
                uLbl2->setStyleSheet(QString("font-size:11px; color:%1; border:none;").arg(C::INK400));
                drl->addWidget(loLbl); drl->addWidget(defLo);
                drl->addSpacing(8);
                drl->addWidget(hiLbl); drl->addWidget(defHi);
                drl->addSpacing(8);
                drl->addWidget(uLbl2);
                drl->addStretch();
                dbvl->addWidget(defRow);
                rtl->addWidget(defBox);

                // Cards scroll area
                QScrollArea *cardsSa = new QScrollArea(rt);
                cardsSa->setWidgetResizable(true);
                cardsSa->setFrameShape(QFrame::NoFrame);
                cardsSa->setStyleSheet("QScrollArea{ background:transparent; }");
                cardsSa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                QWidget *cardsInner = new QWidget;
                cardsInner->setStyleSheet("background:transparent;");
                QVBoxLayout *cardsVl = new QVBoxLayout(cardsInner);
                cardsVl->setContentsMargins(0, 6, 0, 0);
                cardsVl->setSpacing(0);

                // Default card (男, 18~60岁)
                QWidget *defaultCard = makeRefCard("男", "18~60岁", "0", "0.04",
                                                   unit, cardsInner, cardsVl);
                cardsVl->addWidget(defaultCard);
                cardsVl->addStretch();
                cardsSa->setWidget(cardsInner);
                rtl->addWidget(cardsSa, 1);

                // Connect "+ 自定义范围" button
                QString capturedUnit = unit;
                connect(addCardBtn, &QPushButton::clicked, rt,
                    [rt, cardsInner, cardsVl, capturedUnit]() {
                        openAddRefDialog(rt, cardsInner, cardsVl, capturedUnit);
                    });
            }

            resultTabs->addTab(rt, QString("结果%1").arg(r));
        }
        rvl->addWidget(resultTabs, 1);
        rightStack->addWidget(rp);
    }
    pl->addStretch();

    // Delete/clear buttons at bottom of left panel
    QWidget *leftBtns = new QWidget(projList);
    QHBoxLayout *lbl2 = new QHBoxLayout(leftBtns);
    lbl2->setContentsMargins(0,0,0,0);
    lbl2->setSpacing(4);
    QPushButton *clearBtn = new QPushButton("清空", leftBtns);
    clearBtn->setStyleSheet(smallBtnStyle(false));
    QPushButton *delProjBtn = new QPushButton("删除", leftBtns);
    delProjBtn->setStyleSheet(
        "QPushButton{ height:26px; padding:0 8px; font-size:11px;"
        "  background:#fff; color:#C0392B; border:1px solid #D6DEE6; border-radius:4px; }"
        "QPushButton:hover{ background:#FBE6E3; }");
    lbl2->addWidget(clearBtn);
    lbl2->addWidget(delProjBtn);
    pl->addWidget(leftBtns);

    hl->addWidget(projList);
    hl->addWidget(rightStack, 1);

    // Connect project buttons
    for (int i = 0; i < 6; ++i) {
        connect(projBtns[i], &QPushButton::clicked, outer,
            [=]() mutable {
                for (int j = 0; j < 6; ++j)
                    projBtns[j]->setStyleSheet(j == i ? projBtnActive : projBtnInactive);
                rightStack->setCurrentIndex(i);
            });
    }
    return outer;
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 打印设置
// ══════════════════════════════════════════════════════════════════
QWidget *SettingsTab::buildPrintPanel()
{
    QWidget *inner = new QWidget;
    inner->setStyleSheet("background:#fff;");
    QVBoxLayout *vl = new QVBoxLayout(inner);
    vl->setContentsMargins(14, 8, 14, 8);
    vl->setSpacing(0);

    vl->addWidget(makeSecTitle("基本设置", inner));

    // Auto print toggle row
    {
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 10, 0, 10);
        hl->setSpacing(8);
        QLabel *lbl = new QLabel("检测完成后自动打印结果", row);
        lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK700));
        m_autoPrint = makeToggle(false, row);
        QLabel *statusLbl = makeStatusLabel("已关闭", false, row);
        connect(m_autoPrint, &ToggleSwitch::toggled, this, [=](bool v) {
            statusLbl->setText(v ? "已启用" : "已关闭");
            statusLbl->setStyleSheet(QString("font-size:11px; color:%1; border:none;")
                                     .arg(v ? C::OK : C::INK300));
        });
        hl->addWidget(lbl, 1);
        hl->addWidget(statusLbl);
        hl->addWidget(m_autoPrint);
        vl->addWidget(row);
    }

    // Printer type segment
    {
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 10, 0, 10);
        hl->setSpacing(8);
        QLabel *lbl = new QLabel("打印机类型", row);
        lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK700));
        QWidget *seg = makeSegment("内置打印机", "外置打印机",
                                   m_printerInternalBtn, m_printerExternalBtn, row);
        m_printerInternalBtn->setChecked(true);
        connect(m_printerInternalBtn, &QPushButton::clicked, this, [this]{ onPrinterTypeChanged(true); });
        connect(m_printerExternalBtn, &QPushButton::clicked, this, [this]{ onPrinterTypeChanged(false); });
        hl->addWidget(lbl, 1);
        hl->addWidget(seg);
        vl->addWidget(row);
    }

    // ── Internal section ──
    m_printInternalSection = new QWidget(inner);
    {
        QVBoxLayout *sv = new QVBoxLayout(m_printInternalSection);
        sv->setContentsMargins(0, 0, 0, 0);
        sv->setSpacing(0);
        sv->addWidget(makeSecTitle("打印内容", m_printInternalSection));

        struct ContentRow { const char *label; bool on; };
        static const ContentRow CONT[5] = {
            {"样本信息",true},{"检测结果",true},{"参考范围",true},
            {"检验人员",true},{"病人姓名",true}
        };
        for (const ContentRow &cr : CONT) {
            QWidget *row = new QWidget(m_printInternalSection);
            row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
            QHBoxLayout *hl = new QHBoxLayout(row);
            hl->setContentsMargins(0, 10, 0, 10);
            hl->setSpacing(8);
            QLabel *lbl = new QLabel(cr.label, row);
            lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK700));
            ToggleSwitch *tog = makeToggle(cr.on, row);
            QLabel *st = makeStatusLabel(cr.on ? "已启用" : "已关闭", cr.on, row);
            connect(tog, &ToggleSwitch::toggled, this, [=](bool v) {
                st->setText(v ? "已启用" : "已关闭");
                st->setStyleSheet(QString("font-size:11px; color:%1; border:none;")
                                  .arg(v ? C::OK : C::INK300));
            });
            hl->addWidget(lbl, 1);
            hl->addWidget(st);
            hl->addWidget(tog);
            sv->addWidget(row);
        }
    }
    vl->addWidget(m_printInternalSection);

    // ── External section ──
    m_printExternalSection = new QWidget(inner);
    m_printExternalSection->hide();
    {
        QVBoxLayout *sv = new QVBoxLayout(m_printExternalSection);
        sv->setContentsMargins(0, 0, 0, 0);
        sv->setSpacing(0);
        sv->addWidget(makeSecTitle("外置打印机设置", m_printExternalSection));

        QLineEdit *nameEd = new QLineEdit(m_printExternalSection);
        nameEd->setPlaceholderText("输入打印机名称或端口");
        nameEd->setFixedWidth(200);
        nameEd->setStyleSheet(inputStyle());
        sv->addWidget(makeRow("打印机名称", 90, nameEd, m_printExternalSection));

        QLineEdit *tplEd = new QLineEdit(m_printExternalSection);
        tplEd->setPlaceholderText("未导入模板");
        tplEd->setReadOnly(true);
        tplEd->setFixedWidth(160);
        tplEd->setStyleSheet(inputStyle());
        QPushButton *importBtn = new QPushButton("导入模板", m_printExternalSection);
        importBtn->setStyleSheet(smallBtnStyle(false));
        connect(importBtn, &QPushButton::clicked, this, [tplEd]{
            tplEd->setText("template_v1.rpt");
        });
        sv->addWidget(makeRow("打印模板", 90, tplEd, m_printExternalSection, importBtn));
    }
    vl->addWidget(m_printExternalSection);

    // Bottom buttons
    vl->addStretch();
    QWidget *btnRow = new QWidget(inner);
    QHBoxLayout *bl = new QHBoxLayout(btnRow);
    bl->setContentsMargins(0, 8, 0, 4);
    bl->setSpacing(8);
    bl->addStretch();
    bl->addWidget(makeActionBtn("打印测试", false, btnRow));
    bl->addWidget(makeActionBtn("保存设置", true, btnRow));
    vl->addWidget(btnRow);

    return wrapScroll(inner);
}

void SettingsTab::onPrinterTypeChanged(bool isInternal)
{
    if (!m_printerInternalBtn || !m_printerExternalBtn) return;
    m_printerInternalBtn->setChecked(isInternal);
    m_printerExternalBtn->setChecked(!isInternal);
    if (m_printInternalSection) m_printInternalSection->setVisible(isInternal);
    if (m_printExternalSection) m_printExternalSection->setVisible(!isInternal);
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 用户设置
// ══════════════════════════════════════════════════════════════════
QWidget *SettingsTab::buildUsersPanel()
{
    QWidget *outer = new QWidget;
    outer->setStyleSheet("background:#fff;");
    QVBoxLayout *vl = new QVBoxLayout(outer);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    m_userTable = new QTableWidget(0, 4, outer);
    QStringList userHdr;
    userHdr << "序号" << "用户名" << "用户组" << "最后登录时间";
    m_userTable->setHorizontalHeaderLabels(userHdr);
    m_userTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_userTable->setColumnWidth(0, 52);
    m_userTable->setColumnWidth(2, 100);
    m_userTable->setColumnWidth(3, 160);
    m_userTable->verticalHeader()->hide();
    m_userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_userTable->setShowGrid(false);
    m_userTable->setStyleSheet(QString(
        "QTableWidget{ border:none; font-size:13px; background:#fff; }"
        "QTableWidget::item{ padding:10px 8px; border-bottom:1px solid #F0F2F5; }"
        "QTableWidget::item:selected{ background:%1; color:%2; }"
        "QHeaderView::section{ background:%3; font-size:12px; font-weight:600;"
        "  color:%4; border:none; padding:8px; border-bottom:1px solid %5; }"
    ).arg(C::BLUE50).arg(C::INK800).arg(C::SURFALT).arg(C::INK500).arg(C::BORDER));

    struct UserRow { const char *name; const char *group; const char *lastLogin; };
    static const UserRow USERS[3] = {
        {"admin","管理员","2026-04-23 08:12"},
        {"operator1","操作员","2026-04-22 16:45"},
        {"engineer","工程师","2026-04-20 09:30"},
    };
    m_userTable->setRowCount(3);
    for (int i = 0; i < 3; ++i) {
        m_userTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        m_userTable->setItem(i, 1, new QTableWidgetItem(USERS[i].name));
        m_userTable->setItem(i, 2, new QTableWidgetItem(USERS[i].group));
        m_userTable->setItem(i, 3, new QTableWidgetItem(USERS[i].lastLogin));
    }
    connect(m_userTable, &QTableWidget::itemSelectionChanged,
            this, &SettingsTab::onUserSelectionChanged);
    vl->addWidget(m_userTable, 1);

    // Toolbar
    QWidget *toolbar = new QWidget(outer);
    toolbar->setStyleSheet(QString("background:#fff; border-top:1px solid %1;").arg(C::BORDER));
    QHBoxLayout *tl = new QHBoxLayout(toolbar);
    tl->setContentsMargins(8, 6, 8, 6);
    tl->setSpacing(6);

    QLabel *countLbl = new QLabel(QString("最多8个用户 · 当前%1个").arg(3), toolbar);
    countLbl->setStyleSheet(QString("font-size:11px; color:%1;").arg(C::INK400));
    tl->addWidget(countLbl);
    tl->addStretch();

    QPushButton *addBtn = new QPushButton("新增", toolbar);
    addBtn->setStyleSheet(smallBtnStyle(false));
    m_editUserBtn = new QPushButton("修改", toolbar);
    m_editUserBtn->setStyleSheet(smallBtnStyle(false));
    m_editUserBtn->setEnabled(false);
    m_delUserBtn = new QPushButton("删除", toolbar);
    m_delUserBtn->setStyleSheet(
        "QPushButton{ height:26px; padding:0 8px; font-size:11px;"
        "  background:#fff; border:1px solid #D6DEE6; border-radius:4px; }"
        "QPushButton:enabled{ color:#C0392B; }"
        "QPushButton:disabled{ color:#A5B0BC; }");
    m_delUserBtn->setEnabled(false);
    QPushButton *permBtn = new QPushButton("权限分配", toolbar);
    permBtn->setStyleSheet(smallBtnStyle(true));

    tl->addWidget(addBtn);
    tl->addWidget(m_editUserBtn);
    tl->addWidget(m_delUserBtn);
    tl->addWidget(permBtn);
    vl->addWidget(toolbar);
    return outer;
}

void SettingsTab::onUserSelectionChanged()
{
    bool hasSel = !m_userTable->selectedItems().isEmpty();
    if (m_editUserBtn) m_editUserBtn->setEnabled(hasSel);
    if (m_delUserBtn)  m_delUserBtn->setEnabled(hasSel);
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 系统设置
// ══════════════════════════════════════════════════════════════════
QWidget *SettingsTab::buildSysPanel()
{
    QWidget *inner = new QWidget;
    inner->setStyleSheet("background:#fff;");
    QVBoxLayout *vl = new QVBoxLayout(inner);
    vl->setContentsMargins(14, 4, 14, 8);
    vl->setSpacing(0);

    // ── 日期时间 ──
    vl->addWidget(makeSecTitle("日期时间", inner));

    QDateEdit *dateEd = new QDateEdit(QDate::currentDate(), inner);
    dateEd->setFixedWidth(140);
    dateEd->setStyleSheet(inputStyle());
    dateEd->setCalendarPopup(true);
    vl->addWidget(makeRow("日期", 80, dateEd, inner));

    QTimeEdit *timeEd = new QTimeEdit(QTime::currentTime(), inner);
    timeEd->setFixedWidth(110);
    timeEd->setStyleSheet(inputStyle());
    vl->addWidget(makeRow("时间", 80, timeEd, inner));

    // ── 屏幕亮度 ──
    vl->addWidget(makeSecTitle("屏幕亮度", inner));
    {
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 7, 0, 7);
        hl->setSpacing(10);
        QLabel *lbl = new QLabel("亮度", row);
        lbl->setFixedWidth(80);
        lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK500));
        QSlider *slider = new QSlider(Qt::Horizontal, row);
        slider->setRange(10, 100);
        slider->setValue(80);
        slider->setFixedWidth(150);
        slider->setStyleSheet(
            "QSlider::groove:horizontal{ height:4px; background:#D6DEE6; border-radius:2px; }"
            "QSlider::handle:horizontal{ width:14px; height:14px; margin:-5px 0;"
            "  background:#1464A5; border-radius:7px; }"
            "QSlider::sub-page:horizontal{ background:#1464A5; border-radius:2px; }");
        m_brightLabel = new QLabel("80%", row);
        m_brightLabel->setStyleSheet(QString("font-size:12px; color:%1; border:none; min-width:36px;")
                                     .arg(C::INK500));
        connect(slider, &QSlider::valueChanged, this, &SettingsTab::onBrightnessChanged);
        hl->addWidget(lbl);
        hl->addWidget(slider);
        hl->addWidget(m_brightLabel);
        hl->addStretch();
        vl->addWidget(row);
    }

    // ── 通讯设置(LIS) ──
    vl->addWidget(makeSecTitle("通讯设置（LIS）", inner));

    ToggleSwitch *lisTog = makeToggle(false, inner);
    vl->addWidget(makeRow("启用LIS", 90, lisTog, inner));

    // Interface type segment
    {
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 7, 0, 7);
        hl->setSpacing(10);
        QLabel *lbl = new QLabel("接口类型", row);
        lbl->setFixedWidth(90);
        lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK500));
        QWidget *seg = makeSegment("网口", "串口", m_netModeBtn, m_serialModeBtn, row);
        m_netModeBtn->setChecked(true);
        connect(m_netModeBtn, &QPushButton::clicked, this, [this]{ onCommModeChanged(true); });
        connect(m_serialModeBtn, &QPushButton::clicked, this, [this]{ onCommModeChanged(false); });
        hl->addWidget(lbl);
        hl->addWidget(seg);
        hl->addStretch();
        vl->addWidget(row);
    }

    // Network section
    m_networkSection = new QWidget(inner);
    {
        QVBoxLayout *ns = new QVBoxLayout(m_networkSection);
        ns->setContentsMargins(0, 0, 0, 0);
        ns->setSpacing(0);
        QLineEdit *ipEd = new QLineEdit("192.168.1.100", m_networkSection);
        ipEd->setFixedWidth(150);
        ipEd->setStyleSheet(inputStyle());
        ns->addWidget(makeRow("服务器 IP", 90, ipEd, m_networkSection));
        QLineEdit *portEd = new QLineEdit("8080", m_networkSection);
        portEd->setFixedWidth(90);
        portEd->setStyleSheet(inputStyle());
        ns->addWidget(makeRow("端口号", 90, portEd, m_networkSection));
        QLineEdit *localIpEd = new QLineEdit("192.168.1.200", m_networkSection);
        localIpEd->setFixedWidth(150);
        localIpEd->setStyleSheet(inputStyle());
        ns->addWidget(makeRow("本机 IP", 90, localIpEd, m_networkSection));
    }
    vl->addWidget(m_networkSection);

    // Serial section
    m_serialSection = new QWidget(inner);
    m_serialSection->hide();
    {
        QVBoxLayout *ss = new QVBoxLayout(m_serialSection);
        ss->setContentsMargins(0, 0, 0, 0);
        ss->setSpacing(0);
        QComboBox *comCombo = new QComboBox(m_serialSection);
        comCombo->setFixedWidth(100);
        comCombo->addItems(QStringList() << "COM1" << "COM2" << "COM3" << "COM4");
        comCombo->setStyleSheet(inputStyle());
        ss->addWidget(makeRow("串口号", 90, comCombo, m_serialSection));
    }
    vl->addWidget(m_serialSection);

    // Upload mode
    {
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 7, 0, 7);
        hl->setSpacing(16);
        QLabel *lbl = new QLabel("上传方式", row);
        lbl->setFixedWidth(90);
        lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK500));
        QRadioButton *one = new QRadioButton("单向LIS", row);
        QRadioButton *two = new QRadioButton("双向LIS", row);
        one->setChecked(true);
        for (auto *rb : {one, two})
            rb->setStyleSheet(QString("font-size:12px; color:%1;").arg(C::INK700));
        hl->addWidget(lbl);
        hl->addWidget(one);
        hl->addWidget(two);
        hl->addStretch();
        vl->addWidget(row);
    }

    // Abnormal upload
    {
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 7, 0, 7);
        hl->setSpacing(10);
        QLabel *lbl = new QLabel("异常结果上传", row);
        lbl->setFixedWidth(90);
        lbl->setStyleSheet(QString("font-size:12px; color:%1; border:none;").arg(C::INK500));
        ToggleSwitch *abnTog = makeToggle(false, row);
        QLabel *hint = new QLabel("仅上传超出参考范围的结果", row);
        hint->setStyleSheet(QString("font-size:11px; color:%1; border:none;").arg(C::INK400));
        hl->addWidget(lbl);
        hl->addWidget(abnTog);
        hl->addWidget(hint);
        hl->addStretch();
        vl->addWidget(row);
    }

    // Bottom buttons
    vl->addStretch();
    QWidget *btnRow = new QWidget(inner);
    QHBoxLayout *bl = new QHBoxLayout(btnRow);
    bl->setContentsMargins(0, 8, 0, 4);
    bl->setSpacing(8);
    bl->addStretch();
    bl->addWidget(makeActionBtn("测试连接", false, btnRow));
    bl->addWidget(makeActionBtn("保存设置", true, btnRow));
    vl->addWidget(btnRow);

    return wrapScroll(inner);
}

void SettingsTab::onCommModeChanged(bool isNetwork)
{
    if (!m_netModeBtn || !m_serialModeBtn) return;
    m_netModeBtn->setChecked(isNetwork);
    m_serialModeBtn->setChecked(!isNetwork);
    if (m_networkSection) m_networkSection->setVisible(isNetwork);
    if (m_serialSection)  m_serialSection->setVisible(!isNetwork);
}

void SettingsTab::onBrightnessChanged(int val)
{
    if (m_brightLabel) m_brightLabel->setText(QString("%1%").arg(val));
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 关于仪器
// ══════════════════════════════════════════════════════════════════
QWidget *SettingsTab::buildAboutPanel()
{
    QWidget *inner = new QWidget;
    inner->setStyleSheet("background:#fff;");
    QVBoxLayout *vl = new QVBoxLayout(inner);
    vl->setContentsMargins(14, 8, 14, 8);
    vl->setSpacing(0);

    // ── Device info rows ──
    struct InfoRow { const char *label; const char *value; };
    static const InfoRow INFO[6] = {
        {"仪器名称","干式荧光免疫分析仪"},
        {"仪器型号","FIC-FA08"},
        {"软件发布版本","v1.0"},
        {"软件完整版本","v1.0.0.0"},
        {"仪器序列号","—"},
        {"ICCID","89860...4321"},
    };
    for (const InfoRow &ir : INFO) {
        QWidget *row = new QWidget(inner);
        row->setStyleSheet("border-bottom:1px solid #F0F2F5;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 8, 0, 8);
        hl->setSpacing(10);
        QLabel *key = new QLabel(ir.label, row);
        key->setFixedWidth(110);
        key->setStyleSheet(QString(
            "font-size:12px; color:%1; border:1px solid #dde5f7;"
            "background:#f0f4ff; border-radius:4px; padding:2px 6px;").arg(C::INK500));
        QLabel *val = new QLabel(ir.value, row);
        bool bold = QString(ir.label) == "仪器名称";
        val->setStyleSheet(QString("font-size:12px; color:%1; font-weight:%2; border:none;")
                           .arg(C::INK800).arg(bold ? "600" : "400"));
        hl->addWidget(key);
        hl->addWidget(val);
        hl->addStretch();
        vl->addWidget(row);
    }

    // ── Buttons: 软件更新 + 日志导出 ──
    QWidget *btnRow = new QWidget(inner);
    btnRow->setStyleSheet("background:transparent;");
    QHBoxLayout *bl = new QHBoxLayout(btnRow);
    bl->setContentsMargins(0, 12, 0, 4);
    bl->setSpacing(8);
    QPushButton *updateBtn = makeActionBtn("软件更新", true, btnRow);
    QPushButton *logBtn    = makeActionBtn("日志导出", false, btnRow);
    bl->addWidget(updateBtn);
    bl->addWidget(logBtn);
    bl->addStretch();
    vl->addWidget(btnRow);

    // ── Software versions section (hidden by default) ──
    QWidget *softSection = new QWidget(inner);
    softSection->setVisible(false);
    softSection->setStyleSheet("background:transparent;");
    QVBoxLayout *svl = new QVBoxLayout(softSection);
    svl->setContentsMargins(0, 8, 0, 0);
    svl->setSpacing(6);

    svl->addWidget(makeSecTitle("软件版本", softSection));

    // Info banner
    QLabel *banner = new QLabel(
        "请将需要更新的程序包放到U盘 update 文件夹下，点击对应行的[更新]按钮。\n"
        "等待发送完成，仪器校验成功后会自动重启完成更新。", softSection);
    banner->setWordWrap(true);
    banner->setStyleSheet(
        "font-size:11px; color:#5a4500; background:#fffbe6;"
        "border:1px solid #ffe58f; border-radius:4px; padding:8px 10px;");
    svl->addWidget(banner);

    QTableWidget *softTable = new QTableWidget(5, 3, softSection);
    QStringList softHdr;
    softHdr << "软件名称" << "当前版本" << "操作";
    softTable->setHorizontalHeaderLabels(softHdr);
    softTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    softTable->setColumnWidth(1, 130);
    softTable->setColumnWidth(2, 110);
    softTable->verticalHeader()->hide();
    softTable->setShowGrid(false);
    softTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    softTable->setFixedHeight(155);
    softTable->setStyleSheet(QString(
        "QTableWidget{ border:1px solid %1; font-size:12px; background:#fff; }"
        "QHeaderView::section{ background:%2; font-size:11px; font-weight:600;"
        "  color:%3; border:none; padding:5px 8px; border-bottom:1px solid %4; }"
        "QTableWidget::item{ padding:5px 8px; border-bottom:1px solid #F5F7FA; color:%5; }"
    ).arg(C::BORDER).arg(C::SURFALT).arg(C::INK500).arg(C::BORDER).arg(C::INK800));

    struct SoftRow { const char *name; const char *ver; bool hasBtn; };
    static const SoftRow SOFT[5] = {
        {"用户程序","v1.0.0.0",true},
        {"控制板程序","v2.1.3",true},
        {"移液器程序","v1.0.0",false},
        {"通道程序版本号","v1.0.0",false},
        {"通用参数版本","v1.0",false},
    };
    for (int i = 0; i < 5; ++i) {
        softTable->setItem(i, 0, new QTableWidgetItem(SOFT[i].name));
        softTable->setItem(i, 1, new QTableWidgetItem(SOFT[i].ver));
        if (SOFT[i].hasBtn) {
            QPushButton *updBtn = new QPushButton("更新", softTable);
            updBtn->setFixedHeight(22);
            updBtn->setStyleSheet(
                "QPushButton{ padding:0 8px; font-size:10px; background:#1464A5;"
                "  color:#fff; border:none; border-radius:3px; }"
                "QPushButton:hover{ background:#1056a0; }");
            QWidget *cell = new QWidget(softTable);
            QHBoxLayout *cl = new QHBoxLayout(cell);
            cl->setContentsMargins(4, 2, 4, 2);
            cl->addWidget(updBtn);
            cl->addStretch();
            softTable->setCellWidget(i, 2, cell);
        }
    }
    svl->addWidget(softTable);
    vl->addWidget(softSection);
    vl->addStretch();

    // Toggle software section on 软件更新 click
    connect(updateBtn, &QPushButton::clicked, inner, [softSection, updateBtn]() {
        bool shown = !softSection->isVisible();
        softSection->setVisible(shown);
        updateBtn->setText(shown ? "收起更新" : "软件更新");
    });

    return wrapScroll(inner);
}

// ══════════════════════════════════════════════════════════════════
//  Panel: 厂家模式
// ══════════════════════════════════════════════════════════════════
QWidget *SettingsTab::buildFactoryPanel()
{
    QWidget *w = new QWidget;
    w->setStyleSheet("background:#fff;");
    QVBoxLayout *vl = new QVBoxLayout(w);
    vl->setAlignment(Qt::AlignCenter);

    QLabel *ico = new QLabel(w);
    ico->setAlignment(Qt::AlignCenter);
    ico->setPixmap(SvgIcon::pixmap("settings", 36, QColor("#A5B0BC")));

    QLabel *lbl = new QLabel("厂家模式（预留）", w);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet(QString("font-size:13px; color:%1; margin-top:8px;").arg(C::INK500));

    vl->addStretch();
    vl->addWidget(ico);
    vl->addWidget(lbl);
    vl->addStretch();
    return w;
}
