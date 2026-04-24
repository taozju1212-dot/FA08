#include "AddCardModal.h"
#include "ui/widgets/SvgIcon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFrame>
#include <QEvent>
#include <QVariant>

AddCardModal::AddCardModal(const QList<ReagentCard> &cards,
                           const QMap<QString, int>  &neededByCode,
                           QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::FramelessWindowHint),
      m_cards(cards), m_needed(neededByCode)
{
    setFixedSize(460, 500);
    setStyleSheet(
        "QDialog { background:#FFFFFF; border-radius:10px; border:1px solid #D6DEE6; }"
    );
    buildUI();

    m_timer.setInterval(100); // 5s total = 50 ticks of 2% each
    connect(&m_timer, &QTimer::timeout, this, &AddCardModal::onProgressTick);
}

void AddCardModal::buildUI()
{
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
    icoLbl->setPixmap(SvgIcon::pixmap("card", 14, Qt::white));
    QLabel *titleLbl = new QLabel("加载试剂卡", hdr);
    titleLbl->setStyleSheet("font-size:14px; font-weight:600; color:#fff;");
    hl->addWidget(icoLbl); hl->addWidget(titleLbl); hl->addStretch();
    QPushButton *closeBtn = new QPushButton("×", hdr);
    closeBtn->setObjectName("modalClose");
    closeBtn->setFixedSize(28, 28);
    closeBtn->setStyleSheet(
        "QPushButton{background:transparent;border:none;color:rgba(255,255,255,0.9);font-size:18px;}"
        "QPushButton:hover{color:#fff;}"
    );
    connect(closeBtn, &QPushButton::clicked, this, [this]{
        if (m_loadingCode.isEmpty()) accept();
    });
    hl->addWidget(closeBtn);
    vl->addWidget(hdr);

    // ── Body ──
    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(14, 12, 14, 8); bl->setSpacing(10);

    // Total count row
    int total = 0;
    for (auto v : m_needed) total += v;
    QHBoxLayout *totalRow = new QHBoxLayout;
    QLabel *totalLbl = new QLabel("待加卡总数：", body);
    totalLbl->setStyleSheet("font-size:13px; color:#5B6B7B;");
    QLabel *totalNum = new QLabel(body);
    totalNum->setText(QString("<b style='font-size:18px;color:#1464A5;font-family:Consolas;'>%1</b>"
                              "<span style='font-size:12px;color:#5B6B7B;'> 张</span>").arg(total));
    totalNum->setTextFormat(Qt::RichText);
    totalRow->addWidget(totalLbl); totalRow->addWidget(totalNum); totalRow->addStretch();
    bl->addLayout(totalRow);

    // Card list (scrollable)
    QScrollArea *scroll = new QScrollArea(body);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea{border:none;}");
    scroll->setFixedHeight(240);
    m_listWidget = new QWidget;
    buildCardList();
    scroll->setWidget(m_listWidget);
    bl->addWidget(scroll);

    // Hint banner
    QWidget *hint = new QWidget(body);
    hint->setStyleSheet(
        "background:#FFFBEE; border:1px solid #E6D068; border-radius:8px;"
    );
    QHBoxLayout *hintL = new QHBoxLayout(hint);
    hintL->setContentsMargins(12, 8, 12, 8); hintL->setSpacing(6);
    QLabel *hintIco = new QLabel("ℹ", hint);
    hintIco->setStyleSheet("color:#C09820; font-size:14px;");
    QLabel *hintTxt = new QLabel("请扫描试剂卡的条码，确认项目批次后，点击加卡", hint);
    hintTxt->setStyleSheet("font-size:11px; color:#7A5C0A;");
    hintTxt->setWordWrap(true);
    hintL->addWidget(hintIco); hintL->addWidget(hintTxt, 1);
    bl->addWidget(hint);

    // Progress bar (hidden by default)
    m_progressBar = new QProgressBar(body);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setFixedHeight(8);
    m_progressBar->setTextVisible(false);
    m_progressBar->setStyleSheet(
        "QProgressBar{border:1px solid #D6DEE6;border-radius:4px;background:#E8EEF3;}"
        "QProgressBar::chunk{background:#1464A5;border-radius:3px;}"
    );
    m_progressBar->hide();
    m_progressLabel = new QLabel("加卡中，请稍候…", body);
    m_progressLabel->setStyleSheet("font-size:12px; color:#1464A5; font-weight:600;");
    m_progressLabel->setAlignment(Qt::AlignCenter);
    m_progressLabel->hide();
    bl->addWidget(m_progressBar);
    bl->addWidget(m_progressLabel);

    vl->addWidget(body, 1);

    // ── Footer ──
    QWidget *footer = new QWidget(this);
    footer->setFixedHeight(50);
    footer->setStyleSheet("background:#F4F7FA; border-top:1px solid #E6ECF2; border-radius:0 0 10px 10px;");
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(14, 7, 14, 7); fl->setSpacing(8);
    fl->addStretch();

    QPushButton *cancelBtn = new QPushButton("取消", footer);
    cancelBtn->setFixedHeight(34);
    cancelBtn->setStyleSheet(
        "QPushButton{background:#fff;border:1px solid #B7C3CE;border-radius:5px;padding:0 20px;font-size:13px;}"
        "QPushButton:hover{background:#EEF4F9;}"
    );
    connect(cancelBtn, &QPushButton::clicked, this, [this]{
        if (m_loadingCode.isEmpty()) reject();
    });
    fl->addWidget(cancelBtn);

    m_loadBtn = new QPushButton(footer);
    m_loadBtn->setText("加 卡");
    m_loadBtn->setIcon(SvgIcon::icon("card", 14, Qt::white));
    m_loadBtn->setIconSize(QSize(14, 14));
    m_loadBtn->setFixedHeight(34);
    m_loadBtn->setStyleSheet(
        "QPushButton{background:#1464A5;color:#fff;border:1px solid #0F4E85;"
        "border-radius:5px;padding:0 24px;font-size:13px;font-weight:700;}"
        "QPushButton:hover{background:#0F4E85;}"
        "QPushButton:disabled{background:#7FB2DC;border-color:#7FB2DC;}"
    );
    m_loadBtn->setEnabled(false);
    connect(m_loadBtn, &QPushButton::clicked, this, &AddCardModal::onLoadCard);
    fl->addWidget(m_loadBtn);

    vl->addWidget(footer);
}

void AddCardModal::buildCardList()
{
    // Clear existing
    delete m_listWidget->layout();
    qDeleteAll(m_listWidget->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    m_cardWidgets.clear();

    QVBoxLayout *vl = new QVBoxLayout(m_listWidget);
    vl->setContentsMargins(0, 0, 4, 0);
    vl->setSpacing(6);

    for (auto it = m_needed.constBegin(); it != m_needed.constEnd(); ++it) {
        const QString &code = it.key();
        int need = it.value();
        QWidget *w = makeCardWidget(code, need);
        m_cardWidgets[code] = w;
        vl->addWidget(w);
    }
    vl->addStretch();
}

QWidget *AddCardModal::makeCardWidget(const QString &code, int need)
{
    QWidget *w = new QWidget(m_listWidget);
    w->setCursor(Qt::PointingHandCursor);
    bool isSel = (m_selectedCode == code);
    w->setStyleSheet(isSel
        ? "QWidget{background:#EEF4F9;border:2px solid #1464A5;border-radius:10px;}"
        : "QWidget{background:#fff;border:2px solid #D6DEE6;border-radius:10px;}"
          "QWidget:hover{border-color:#7FB2DC;background:#F8FAFC;}"
    );
    w->setFixedHeight(56);

    QHBoxLayout *hl = new QHBoxLayout(w);
    hl->setContentsMargins(14, 0, 14, 0); hl->setSpacing(10);

    // Code label
    QLabel *codeLbl = new QLabel(code, w);
    QFont cf; cf.setPixelSize(16); cf.setWeight(QFont::Black);
    codeLbl->setFont(cf);
    codeLbl->setFixedWidth(70);
    codeLbl->setStyleSheet(isSel ? "color:#0F4E85;" : "color:#1F2A37;");
    hl->addWidget(codeLbl);

    // LOT placeholder (center)
    QLabel *lotLbl = new QLabel(isSel ? "请扫描条码或选择批次" : "—", w);
    lotLbl->setStyleSheet(isSel ? "font-size:11px;color:#5B6B7B;" : "font-size:12px;color:#B7C3CE;");
    lotLbl->setAlignment(Qt::AlignCenter);
    hl->addWidget(lotLbl, 1);

    // Need count
    QLabel *needLbl = new QLabel(w);
    needLbl->setText(QString("待加 <b style='font-size:15px;color:%1;font-family:Consolas;'>%2</b> 张")
        .arg(isSel ? "#1464A5" : "#334155").arg(need));
    needLbl->setTextFormat(Qt::RichText);
    hl->addWidget(needLbl);

    // We use an event filter on the widget to detect clicks
    w->installEventFilter(this);
    w->setProperty("cardCode", QVariant(code));

    return w;
}

bool AddCardModal::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget *w = qobject_cast<QWidget*>(obj);
        if (w && w->property("cardCode").isValid()) {
            QString code = w->property("cardCode").toString();
            if (!m_needed.contains(code)) return false;
            selectCard(code);
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}

void AddCardModal::selectCard(const QString &code)
{
    if (!m_loadingCode.isEmpty()) return; // locked during loading
    m_selectedCode = code;
    updateLoadButton();

    // Rebuild all card widgets to reflect new selection
    buildCardList();
}

void AddCardModal::updateLoadButton()
{
    m_loadBtn->setEnabled(!m_selectedCode.isEmpty() && m_loadingCode.isEmpty());
}

void AddCardModal::onLoadCard()
{
    if (m_selectedCode.isEmpty() || !m_loadingCode.isEmpty()) return;
    m_loadingCode = m_selectedCode;
    m_progress    = 0;

    m_progressBar->setValue(0);
    m_progressBar->show();
    m_progressLabel->setText(QString("正在加载 %1 试剂卡…").arg(m_loadingCode));
    m_progressLabel->show();
    m_loadBtn->setEnabled(false);

    m_timer.start();
}

void AddCardModal::onProgressTick()
{
    m_progress += 2;
    m_progressBar->setValue(m_progress);

    if (m_progress >= 100) {
        m_timer.stop();

        // Emit loaded signal
        QMap<QString, int> loaded;
        loaded[m_loadingCode] = 1;
        emit cardsLoaded(loaded);

        m_doneCount++;
        m_needed[m_loadingCode] = qMax(0, m_needed[m_loadingCode] - 1);
        if (m_needed[m_loadingCode] == 0) m_needed.remove(m_loadingCode);

        m_loadingCode.clear();
        m_selectedCode.clear();
        m_progress = 0;

        m_progressBar->hide();
        m_progressLabel->hide();

        if (m_needed.isEmpty()) {
            // All done
            accept();
        } else {
            buildCardList();
            updateLoadButton();
        }
    }
}
