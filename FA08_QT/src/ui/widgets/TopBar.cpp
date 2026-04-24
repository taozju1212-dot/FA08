#include "TopBar.h"
#include "SvgIcon.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QLinearGradient>
#include <QDateTime>
#include <QFrame>
#include <QVariant>
#include <QStyle>

TopBar::TopBar(QWidget *parent) : QWidget(parent)
{
    setObjectName("topBar");
    setFixedHeight(44);
    setupUI();
    m_clockTimer.setInterval(1000);
    connect(&m_clockTimer, &QTimer::timeout, this, &TopBar::updateClock);
    m_clockTimer.start();
    updateClock();
}

void TopBar::setupUI()
{
    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setContentsMargins(0, 0, 0, 0);
    hl->setSpacing(0);

    // ── Tab buttons ──
    static const struct { const char *icon; const char *label; } TABS[4] = {
        { "flask_small", "检测" },
        { "chart",       "结果" },
        { "plate",       "耗材" },
        { "settings",    "设置" },
    };
    for (int i = 0; i < 4; ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setProperty("tabBtn", QVariant(true));
        btn->setFlat(true);
        btn->setFixedHeight(44);
        btn->setMinimumWidth(80);
        // Icon + text
        btn->setIcon(SvgIcon::icon(TABS[i].icon, 13, QColor("rgba(255,255,255,0.88)")));
        btn->setIconSize(QSize(13, 13));
        btn->setText(QString(" ") + TABS[i].label);
        QFont f; f.setPixelSize(13); f.setWeight(QFont::Medium);
        btn->setFont(f);
        int idx = i;
        connect(btn, &QPushButton::clicked, this, [this, idx]{ emit tabChanged(idx); });
        m_tabs[i] = btn;
        hl->addWidget(btn);
    }

    // Vertical divider
    QFrame *div = new QFrame(this);
    div->setFrameShape(QFrame::VLine);
    div->setStyleSheet("color: rgba(255,255,255,0.22); margin: 7px 0;");
    hl->addWidget(div);

    // ── Status strip (right side) ──
    QWidget *strip = new QWidget(this);
    strip->setStyleSheet("background: rgba(0,0,0,0.20);");
    QHBoxLayout *sl = new QHBoxLayout(strip);
    sl->setContentsMargins(0, 0, 0, 0);
    sl->setSpacing(0);

    sl->addStretch();

    // LIS + Print + 4G group
    QWidget *connGroup = new QWidget(strip);
    QHBoxLayout *cgl = new QHBoxLayout(connGroup);
    cgl->setContentsMargins(12, 0, 12, 0);
    cgl->setSpacing(10);
    SvgIconLabel *lisIco = new SvgIconLabel("lis", 13, QColor(255,255,255,184), connGroup);
    QLabel *lisLbl = new QLabel("LIS", connGroup);
    lisLbl->setStyleSheet("font-size:10px; color:rgba(255,255,255,0.72); letter-spacing:0.4px;");
    SvgIconLabel *printIco = new SvgIconLabel("print", 13, QColor(255,255,255,184), connGroup);
    SvgIconLabel *sigIco   = new SvgIconLabel("signal4g", 13, QColor(255,255,255,184), connGroup);
    cgl->addWidget(lisIco); cgl->addWidget(lisLbl);
    cgl->addWidget(printIco);
    cgl->addWidget(sigIco);
    sl->addWidget(connGroup);

    // Separator
    auto makeSep = [&]() {
        QLabel *s = new QLabel("|", strip);
        s->setStyleSheet("color:rgba(255,255,255,0.20); font-size:14px;");
        return s;
    };
    sl->addWidget(makeSep());

    // Clock
    m_clockLabel = new QLabel("00:00:00", strip);
    m_clockLabel->setStyleSheet(
        "font-family:'Consolas','Courier New',monospace;"
        "font-size:12px; color:rgba(255,255,255,0.92); letter-spacing:0.6px;"
        "padding:0 12px;"
    );
    sl->addWidget(m_clockLabel);
    sl->addWidget(makeSep());

    // Temperature
    QWidget *tempW = new QWidget(strip);
    QHBoxLayout *tl = new QHBoxLayout(tempW);
    tl->setContentsMargins(12, 0, 12, 0); tl->setSpacing(4);
    SvgIconLabel *thermIco = new SvgIconLabel("therm", 11, Qt::white, tempW);
    m_tempLabel = new QLabel("37.0°C", tempW);
    m_tempLabel->setStyleSheet(
        "font-family:'Consolas','Courier New',monospace;"
        "font-size:12px; color:#fff;"
    );
    tl->addWidget(thermIco); tl->addWidget(m_tempLabel);
    sl->addWidget(tempW);
    sl->addWidget(makeSep());

    // Alert bell button
    QPushButton *bellBtn = new QPushButton(strip);
    bellBtn->setFlat(true);
    bellBtn->setFixedSize(40, 44);
    bellBtn->setIcon(SvgIcon::icon("bell", 16, Qt::white));
    bellBtn->setIconSize(QSize(16, 16));
    bellBtn->setStyleSheet("background:transparent; border:none;");
    connect(bellBtn, &QPushButton::clicked, this, &TopBar::alertsBellClicked);
    sl->addWidget(bellBtn);

    // Alert badge (overlaid)
    m_alertBadge = new QLabel("1", strip);
    m_alertBadge->setFixedSize(14, 14);
    m_alertBadge->setAlignment(Qt::AlignCenter);
    m_alertBadge->setStyleSheet(
        "background:#E74C3C; color:#fff; border-radius:7px;"
        "font-size:9px; font-weight:800;"
    );
    m_alertBadge->hide();
    // Position relative to bellBtn — done in resizeEvent or fixed offset
    m_alertBadge->move(0, 0);  // will be repositioned

    sl->addWidget(makeSep());

    // Device status
    QWidget *devW = new QWidget(strip);
    QHBoxLayout *dl = new QHBoxLayout(devW);
    dl->setContentsMargins(12, 0, 16, 0); dl->setSpacing(6);
    m_statusDot = new QLabel(devW);
    m_statusDot->setFixedSize(7, 7);
    m_statusDot->setStyleSheet("background:#A5B0BC; border-radius:3px;");
    m_statusLabel = new QLabel("待机", devW);
    QFont df; df.setPixelSize(12); df.setWeight(QFont::Bold);
    m_statusLabel->setFont(df);
    m_statusLabel->setStyleSheet("color:#fff; letter-spacing:0.6px;");
    dl->addWidget(m_statusDot); dl->addWidget(m_statusLabel);
    sl->addWidget(devW);

    hl->addWidget(strip, 1);

    // Running strip (positioned absolutely at bottom of topbar)
    m_runStrip = new QWidget(this);
    m_runStrip->setFixedHeight(3);
    m_runStrip->setGeometry(0, 41, 800, 3);
    m_runStrip->setStyleSheet(
        "background: qlineargradient(spread:repeat, x1:0, y1:0, x2:0.025, y2:0,"
        " stop:0 #2E86C1, stop:0.5 #7FB2DC, stop:1 #2E86C1);"
    );
    m_runStrip->hide();

    updateTabStyles();
}

void TopBar::setActiveTab(int index)
{
    if (m_activeTab == index) return;
    m_activeTab = index;
    updateTabStyles();
}

void TopBar::updateTabStyles()
{
    for (int i = 0; i < 4; ++i) {
        bool active = (i == m_activeTab);
        m_tabs[i]->setProperty("active", active ? "true" : "false");
        // Update icon color
        static const char* icons[] = {"flask_small","chart","plate","settings"};
        QColor ic = active ? QColor("#1464A5") : QColor(255,255,255,224);
        m_tabs[i]->setIcon(SvgIcon::icon(icons[i], 13, ic));
        m_tabs[i]->style()->unpolish(m_tabs[i]);
        m_tabs[i]->style()->polish(m_tabs[i]);
        if (active) {
            m_tabs[i]->setStyleSheet(
                "QPushButton { height:44px; padding:0 16px; border:none;"
                " border-right:1px solid rgba(255,255,255,0.12); border-radius:0;"
                " background:rgba(255,255,255,0.95); color:#1464A5;"
                " font-size:13px; font-weight:700; }"
            );
        } else {
            m_tabs[i]->setStyleSheet(
                "QPushButton { height:44px; padding:0 16px; border:none;"
                " border-right:1px solid rgba(255,255,255,0.12); border-radius:0;"
                " background:transparent; color:rgba(255,255,255,0.88);"
                " font-size:13px; font-weight:500; }"
                "QPushButton:hover { background:rgba(255,255,255,0.12); }"
            );
        }
    }
}

void TopBar::setDeviceStatus(DeviceStatus status)
{
    m_deviceStatus = status;
    updateStatusDot();
    m_runStrip->setVisible(status == DeviceStatus::Running);
}

void TopBar::updateStatusDot()
{
    struct { const char *color; const char *label; } map[] = {
        {"#A5B0BC","待机"},
        {"#4CAF50","运行中"},
        {"#E8A33D","暂停"},
        {"#E74C3C","报警"},
    };
    int idx = (int)m_deviceStatus;
    m_statusDot->setStyleSheet(
        QString("background:%1; border-radius:3px;").arg(map[idx].color)
    );
    m_statusLabel->setText(map[idx].label);
}

void TopBar::setTemperature(double celsius)
{
    m_tempLabel->setText(QString("%1°C").arg(celsius, 0, 'f', 1));
}

void TopBar::setUnhandledAlerts(int count)
{
    if (count > 0) {
        m_alertBadge->setText(QString::number(count));
        m_alertBadge->show();
    } else {
        m_alertBadge->hide();
    }
}

void TopBar::updateClock()
{
    QDateTime now = QDateTime::currentDateTime();
    m_clockLabel->setText(now.toString("HH:mm:ss"));
}

void TopBar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QLinearGradient grad(0, 0, 0, height());
    grad.setColorAt(0, QColor("#0F4E85"));
    grad.setColorAt(1, QColor("#1464A5"));
    p.fillRect(rect(), grad);
}
