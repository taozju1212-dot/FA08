#include "BottomBar.h"
#include "SvgIcon.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

BottomBar::BottomBar(QWidget *parent) : QWidget(parent)
{
    setObjectName("bottomBar");
    setFixedHeight(56);
    setupUI();
}

void BottomBar::setupUI()
{
    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setContentsMargins(10, 8, 10, 8);
    hl->setSpacing(6);

    // ── Tips chip ──
    QWidget *tipsW = new QWidget(this);
    tipsW->setStyleSheet(
        "QWidget { background:#fff; border:1px solid #D6DEE6; border-radius:4px; }"
    );
    tipsW->setFixedSize(100, 38);
    QHBoxLayout *tl = new QHBoxLayout(tipsW);
    tl->setContentsMargins(8, 0, 8, 0); tl->setSpacing(5);
    tl->addWidget(new SvgIconLabel("tip", 14, QColor("#2E8B57"), tipsW));
    QLabel *tipsLbl = new QLabel("吸头", tipsW);
    tipsLbl->setStyleSheet("font-size:10px; color:#5B6B7B; background:transparent; border:none;");
    m_tipsQty = new QLabel("52/80", tipsW);
    m_tipsQty->setStyleSheet("font-size:13px; font-weight:700; color:#2E8B57; font-family:'Consolas'; background:transparent; border:none;");
    tl->addWidget(tipsLbl); tl->addWidget(m_tipsQty);
    hl->addWidget(tipsW);

    // ── Plate chip ──
    QWidget *plateW = new QWidget(this);
    plateW->setStyleSheet(
        "QWidget { background:#fff; border:1px solid #D6DEE6; border-radius:4px; }"
    );
    plateW->setFixedSize(110, 38);
    QHBoxLayout *pl = new QHBoxLayout(plateW);
    pl->setContentsMargins(8, 0, 8, 0); pl->setSpacing(5);
    pl->addWidget(new SvgIconLabel("plate", 14, QColor("#2E8B57"), plateW));
    QLabel *plateLbl = new QLabel("稀释板", plateW);
    plateLbl->setStyleSheet("font-size:10px; color:#5B6B7B; background:transparent; border:none;");
    m_plateQty = new QLabel("36/80", plateW);
    m_plateQty->setStyleSheet("font-size:13px; font-weight:700; color:#2E8B57; font-family:'Consolas'; background:transparent; border:none;");
    pl->addWidget(plateLbl); pl->addWidget(m_plateQty);
    hl->addWidget(plateW);

    // ── Dilution circular chips (3 slots) ──
    for (int i = 0; i < 3; ++i) {
        m_dilChip[i] = new CircularProgress(this);
        hl->addWidget(m_dilChip[i]);
    }
    // Set initial data
    Consumables c = AppData::initialConsumables();
    setConsumables(c);

    // Separator
    QFrame *sep = new QFrame(this);
    sep->setFrameShape(QFrame::VLine);
    sep->setFixedHeight(28);
    sep->setStyleSheet("color:#D6DEE6;");
    hl->addSpacing(6);
    hl->addWidget(sep);
    hl->addSpacing(6);

    // ── Card queue button ──
    m_cardBtn = new QPushButton(this);
    m_cardBtn->setFixedHeight(40);
    m_cardBtn->setMinimumWidth(120);
    m_cardBtn->setCursor(Qt::PointingHandCursor);
    m_cardBtn->setStyleSheet(
        "QPushButton { background:#fff; border:1.5px solid #B7C3CE; border-radius:5px;"
        "  padding:0 14px; text-align:left; }"
        "QPushButton:hover { background:#FBF1DF; border-color:#D08419; }"
    );
    connect(m_cardBtn, &QPushButton::clicked, this, &BottomBar::addCardClicked);

    // Custom card button content via child layout
    QHBoxLayout *cardL = new QHBoxLayout(m_cardBtn);
    cardL->setContentsMargins(10, 0, 10, 0); cardL->setSpacing(8);
    SvgIconLabel *cardIco = new SvgIconLabel("card", 18, QColor("#1464A5"), m_cardBtn);
    QLabel *cardTxt = new QLabel("待加卡", m_cardBtn);
    cardTxt->setStyleSheet("font-size:12px; color:#334155; font-weight:500; background:transparent;");
    m_cardCountLabel = new QLabel("0", m_cardBtn);
    QFont cf; cf.setPixelSize(18); cf.setWeight(QFont::Black);
    m_cardCountLabel->setFont(cf);
    m_cardCountLabel->setStyleSheet("color:#1F2A37; background:transparent;");
    QLabel *addCardLbl = new QLabel("加 卡", m_cardBtn);
    addCardLbl->setStyleSheet(
        "background:#1464A5; color:#fff; border-radius:4px;"
        "font-size:13px; font-weight:700; padding:6px 14px;"
    );
    cardL->addWidget(cardIco);
    cardL->addWidget(cardTxt);
    cardL->addWidget(m_cardCountLabel);
    cardL->addWidget(addCardLbl);
    hl->addWidget(m_cardBtn);

    hl->addStretch();

    // ── Device control buttons ──
    m_startBtn = new QPushButton(this);
    m_startBtn->setFixedHeight(40);
    m_startBtn->setMinimumWidth(96);
    m_startBtn->setIcon(SvgIcon::icon("play", 13, Qt::white));
    m_startBtn->setIconSize(QSize(13, 13));
    m_startBtn->setText(" 启动检测");
    m_startBtn->setStyleSheet(
        "QPushButton { background:#2E8B57; color:#fff; border:1px solid #226B42;"
        "  border-radius:6px; font-size:13px; font-weight:500; }"
        "QPushButton:hover { background:#226B42; }"
        "QPushButton:disabled { background:#8FC9A8; border-color:#8FC9A8; }"
    );
    connect(m_startBtn, &QPushButton::clicked, this, &BottomBar::startClicked);
    hl->addWidget(m_startBtn);

    m_pauseBtn = new QPushButton(this);
    m_pauseBtn->setFixedHeight(40);
    m_pauseBtn->setMinimumWidth(68);
    m_pauseBtn->setIcon(SvgIcon::icon("pause", 11, Qt::white));
    m_pauseBtn->setIconSize(QSize(11, 11));
    m_pauseBtn->setText(" 暂停");
    m_pauseBtn->setStyleSheet(
        "QPushButton { background:#D08419; color:#fff; border:1px solid #A9671A;"
        "  border-radius:6px; font-size:13px; font-weight:500; }"
        "QPushButton:hover { background:#A9671A; }"
        "QPushButton:disabled { background:#E8C07A; border-color:#E8C07A; }"
    );
    connect(m_pauseBtn, &QPushButton::clicked, this, &BottomBar::pauseClicked);
    hl->addWidget(m_pauseBtn);

    updateButtons();
}

void BottomBar::setConsumables(const Consumables &c)
{
    bool tipsWarn = c.tips.qty < 20, tipsCrit = c.tips.qty < 10;
    QColor tipsColor = tipsCrit ? QColor("#C0392B") : tipsWarn ? QColor("#D08419") : QColor("#2E8B57");
    m_tipsQty->setText(QString("%1/%2").arg(c.tips.qty).arg(c.tips.capacity));
    m_tipsQty->setStyleSheet(QString("font-size:13px; font-weight:700; color:%1; font-family:'Consolas'; background:transparent; border:none;").arg(tipsColor.name()));

    bool plateWarn = c.plate.qty < 20, plateCrit = c.plate.qty < 10;
    QColor plateColor = plateCrit ? QColor("#C0392B") : plateWarn ? QColor("#D08419") : QColor("#2E8B57");
    m_plateQty->setText(QString("%1/%2").arg(c.plate.qty).arg(c.plate.capacity));
    m_plateQty->setStyleSheet(QString("font-size:13px; font-weight:700; color:%1; font-family:'Consolas'; background:transparent; border:none;").arg(plateColor.name()));

    for (int i = 0; i < qMin(3, (int)c.dilutions.size()); ++i) {
        const DilutionSolution &d = c.dilutions[i];
        m_dilChip[i]->setData(d.name, d.qty, d.capacity, d.loaded);
    }
}

void BottomBar::setDeviceStatus(DeviceStatus status)
{
    m_deviceStatus = status;
    updateButtons();
}

void BottomBar::setScheduledCount(int count)
{
    m_scheduledCount = count;
    updateButtons();
}

void BottomBar::setPendingCardCount(int count)
{
    m_pendingCards = count;
    m_cardCountLabel->setText(QString::number(count));
    if (count > 0) {
        m_cardBtn->setStyleSheet(
            "QPushButton { background:#FBF1DF; border:1.5px solid #D08419; border-radius:5px; padding:0 14px; }"
            "QPushButton:hover { background:#F5E4C0; }"
        );
    } else {
        m_cardBtn->setStyleSheet(
            "QPushButton { background:#fff; border:1.5px solid #B7C3CE; border-radius:5px; padding:0 14px; }"
            "QPushButton:hover { background:#FBF1DF; border-color:#D08419; }"
        );
    }
}

void BottomBar::updateButtons()
{
    bool canStart = (m_deviceStatus == DeviceStatus::Idle || m_deviceStatus == DeviceStatus::Paused)
                    && m_scheduledCount > 0;
    bool canPause = (m_deviceStatus == DeviceStatus::Running);
    m_startBtn->setEnabled(canStart);
    m_pauseBtn->setEnabled(canPause);
}
