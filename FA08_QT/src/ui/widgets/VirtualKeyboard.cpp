#include "VirtualKeyboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>

static const QList<QStringList> NUM_ROWS = {
    {"1","2","3"},
    {"4","5","6"},
    {"7","8","9"},
    {"-","0","."},
};
static const QList<QStringList> QW_ROWS = {
    {"1","2","3","4","5","6","7","8","9","0"},
    {"q","w","e","r","t","y","u","i","o","p"},
    {"a","s","d","f","g","h","j","k","l"},
    {"⇧","z","x","c","v","b","n","m","⌫"},
};

VirtualKeyboard::VirtualKeyboard(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setStyleSheet(
        "QWidget#vkbRoot {"
        "  background:#FFFFFF; border:1px solid #B7C3CE;"
        "  border-radius:8px;"
        "}"
    );
    setObjectName("vkbRoot");
    setWindowFlags(Qt::Widget);

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(8, 8, 8, 8);
    vl->setSpacing(6);

    // Preview bar (shows current value + clear/close)
    m_preview = new QWidget(this);
    QHBoxLayout *pl = new QHBoxLayout(m_preview);
    pl->setContentsMargins(6, 2, 6, 6);
    pl->setSpacing(6);

    QLabel *editIco = new QLabel("✏ 输入:", m_preview);
    editIco->setStyleSheet("font-size:11px; color:#5B6B7B;");
    pl->addWidget(editIco);

    QLabel *valLbl = new QLabel("—", m_preview);
    valLbl->setObjectName("vkbValueLabel");
    valLbl->setStyleSheet("font-size:12px; color:#1F2A37; font-family:'Consolas','Courier New',monospace;");
    pl->addWidget(valLbl);
    pl->addStretch();

    QPushButton *clearBtn = new QPushButton("清空", m_preview);
    clearBtn->setFixedHeight(26);
    clearBtn->setStyleSheet(
        "QPushButton { background:#fff; border:1px solid #B7C3CE; border-radius:4px;"
        "  padding:0 8px; font-size:12px; color:#1F2A37; }"
        "QPushButton:hover { background:#EEF4F9; }"
    );
    connect(clearBtn, &QPushButton::clicked, this, [this]{ onKeyPressed("__clear"); });
    pl->addWidget(clearBtn);

    QPushButton *closeBtn = new QPushButton("✕ 关闭", m_preview);
    closeBtn->setFixedHeight(26);
    closeBtn->setStyleSheet(
        "QPushButton { background:#fff; border:1px solid #B7C3CE; border-radius:4px;"
        "  padding:0 8px; font-size:12px; color:#1F2A37; }"
        "QPushButton:hover { background:#EEF4F9; }"
    );
    connect(closeBtn, &QPushButton::clicked, this, [this]{ hide(); emit closed(); });
    pl->addWidget(closeBtn);
    vl->addWidget(m_preview);

    // Numeric pad
    m_numPad = new QWidget(this);
    QVBoxLayout *nl = new QVBoxLayout(m_numPad);
    nl->setContentsMargins(0, 0, 0, 0); nl->setSpacing(4);
    for (const auto &row : NUM_ROWS) nl->addWidget(buildRow(row));
    // Back and Enter for num
    QHBoxLayout *numExtra = new QHBoxLayout;
    numExtra->setSpacing(4);
    QPushButton *numBack = new QPushButton("⌫", m_numPad);
    numBack->setFixedHeight(36);
    numBack->setStyleSheet("QPushButton{border:1px solid #B7C3CE;border-radius:4px;font-size:15px;background:#fff;}QPushButton:hover{background:#EEF4F9;}");
    connect(numBack, &QPushButton::clicked, this, [this]{ onKeyPressed("__back"); });
    numExtra->addWidget(numBack,1);
    QPushButton *numEnter = new QPushButton("确认", m_numPad);
    numEnter->setFixedHeight(36);
    numEnter->setStyleSheet("QPushButton{background:#1464A5;color:#fff;border:1px solid #0F4E85;border-radius:4px;font-size:13px;font-weight:700;}QPushButton:hover{background:#0F4E85;}");
    connect(numEnter, &QPushButton::clicked, this, [this]{ onKeyPressed("__enter"); });
    numExtra->addWidget(numEnter,2);
    nl->addLayout(numExtra);
    vl->addWidget(m_numPad);

    // QWERTY pad
    m_qwPad = new QWidget(this);
    QVBoxLayout *ql = new QVBoxLayout(m_qwPad);
    ql->setContentsMargins(0, 0, 0, 0); ql->setSpacing(4);
    for (const auto &row : QW_ROWS) ql->addWidget(buildRow(row));
    // Space + Confirm row
    QHBoxLayout *spRow = new QHBoxLayout;
    spRow->setSpacing(4);
    QPushButton *spBtn = new QPushButton("空格", m_qwPad);
    spBtn->setFixedHeight(36);
    spBtn->setStyleSheet("QPushButton{border:1px solid #B7C3CE;border-radius:4px;font-size:12px;background:#fff;}QPushButton:hover{background:#EEF4F9;}");
    connect(spBtn, &QPushButton::clicked, this, [this]{ onKeyPressed(" "); });
    spRow->addWidget(spBtn, 3);
    QPushButton *confBtn = new QPushButton("确认", m_qwPad);
    confBtn->setFixedHeight(36);
    confBtn->setStyleSheet("QPushButton{background:#1464A5;color:#fff;border:1px solid #0F4E85;border-radius:4px;font-size:13px;font-weight:700;}QPushButton:hover{background:#0F4E85;}");
    connect(confBtn, &QPushButton::clicked, this, [this]{ onKeyPressed("__enter"); });
    spRow->addWidget(confBtn, 2);
    ql->addLayout(spRow);
    vl->addWidget(m_qwPad);
}

QWidget *VirtualKeyboard::buildRow(const QStringList &keys, int keyH)
{
    QWidget *row = new QWidget(this);
    QHBoxLayout *hl = new QHBoxLayout(row);
    hl->setContentsMargins(0, 0, 0, 0); hl->setSpacing(4);
    for (const QString &k : keys) {
        QPushButton *btn = new QPushButton(k, row);
        btn->setFixedHeight(keyH);
        bool isSpecial = (k == "⇧" || k == "⌫");
        if (isSpecial) btn->setFixedWidth(60);
        btn->setObjectName("vkbKey_" + k);
        btn->setStyleSheet(
            "QPushButton { border:1px solid #B7C3CE; border-radius:4px;"
            "  background:#fff; font-size:14px; font-family:'Consolas','Courier New',monospace;"
            "  color:#1F2A37; }"
            "QPushButton:hover { background:#EEF4F9; }"
            "QPushButton:pressed { background:#DCE8F2; }"
        );
        connect(btn, &QPushButton::clicked, this, [this, k]{ onKeyPressed(k); });
        hl->addWidget(btn, isSpecial ? 0 : 1);
    }
    return row;
}

void VirtualKeyboard::showFor(Mode mode, const QString &currentValue, const QString &fieldKey)
{
    m_mode     = mode;
    m_value    = currentValue;
    m_fieldKey = fieldKey;
    m_shift    = false;

    m_numPad->setVisible(mode == Numeric);
    m_qwPad ->setVisible(mode == QWERTY);

    // Resize widget
    adjustSize();
    updatePreview();
    show();
    raise();
}

void VirtualKeyboard::updatePreview()
{
    QLabel *lbl = findChild<QLabel*>("vkbValueLabel");
    if (lbl) lbl->setText(m_value.isEmpty() ? "—" : m_value);
}

void VirtualKeyboard::setShift(bool on)
{
    m_shift = on;
    // Update all letter key labels
    for (const auto &row : QW_ROWS) {
        for (const QString &k : row) {
            if (k.length() == 1 && k[0].isLetter()) {
                QPushButton *btn = findChild<QPushButton*>("vkbKey_" + k);
                if (btn) btn->setText(on ? k.toUpper() : k.toLower());
            }
        }
    }
    QPushButton *shiftBtn = findChild<QPushButton*>("vkbKey_⇧");
    if (shiftBtn) {
        shiftBtn->setStyleSheet(on
            ? "QPushButton{border:1px solid #1464A5;border-radius:4px;background:#DCE8F2;font-size:14px;}"
            : "QPushButton{border:1px solid #B7C3CE;border-radius:4px;background:#fff;font-size:14px;}"
              "QPushButton:hover{background:#EEF4F9;}"
        );
    }
}

void VirtualKeyboard::onKeyPressed(const QString &key)
{
    if (key == "⇧") { setShift(!m_shift); return; }
    if (key == "__back") {
        if (!m_value.isEmpty()) m_value.chop(1);
    } else if (key == "__clear") {
        m_value.clear();
    } else if (key == "__enter") {
        emit confirmed(m_fieldKey, m_value);
        hide();
        emit closed();
        return;
    } else if (key == "⌫") {
        if (!m_value.isEmpty()) m_value.chop(1);
    } else {
        QString ch = key;
        if (m_shift && ch.length() == 1 && ch[0].isLetter())
            ch = ch.toUpper();
        m_value += ch;
        if (m_shift) setShift(false);  // auto-release shift after one char
    }
    updatePreview();
    emit valueChanged(m_fieldKey, m_value);
}
