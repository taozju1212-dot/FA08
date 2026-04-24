#include "DilutionCard.h"
#include "ui/widgets/SvgIcon.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

static const QString kBtnNormal =
    "QPushButton { background:#F4F7FA; border:1px solid #D6DEE6; border-radius:4px;"
    "  padding:0 10px; font-size:11px; color:#1464A5; }"
    "QPushButton:hover { background:#DCE8F2; border-color:#7FB2DC; }";

static const QString kBtnActive =
    "QPushButton { background:#1464A5; border:1px solid #1464A5; border-radius:4px;"
    "  padding:0 10px; font-size:11px; color:#FFFFFF; }"
    "QPushButton:hover { background:#0F4E8A; }";

static const QString kBtnPrimary =
    "QPushButton { background:#1464A5; border:1px solid #1464A5; border-radius:4px;"
    "  padding:0 12px; font-size:11px; color:#FFFFFF; }"
    "QPushButton:hover { background:#0F4E8A; }";

static const QString kBtnSecondary =
    "QPushButton { background:#FFFFFF; border:1px solid #B7C3CE; border-radius:4px;"
    "  padding:0 12px; font-size:11px; color:#1F2A37; }"
    "QPushButton:hover { background:#EEF4F9; }";

// ─────────────────────────────────────────────────────────────────────────────
DilutionCard::DilutionCard(QWidget *parent) : QFrame(parent)
{
    setObjectName("DilutionCard");
    setStyleSheet(
        "DilutionCard { background:#FFFFFF; border:1px solid #D6DEE6; border-radius:8px; }"
    );

    // Compact margins / spacing: target collapsed height ≈ 52 px
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(12, 6, 12, 6);
    vl->setSpacing(5);

    // ── Row 1: icon │ name+lot (one line) │ replace button ─────────────────
    QHBoxLayout *row1 = new QHBoxLayout;
    row1->setSpacing(7);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(16, 16);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    row1->addWidget(m_iconLabel);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setStyleSheet("font-size:12px; color:#1F2A37; background:transparent;");
    m_nameLabel->setTextFormat(Qt::RichText);
    row1->addWidget(m_nameLabel, 1);

    m_replaceBtn = new QPushButton("↺ 更换", this);
    m_replaceBtn->setFixedHeight(24);
    m_replaceBtn->setStyleSheet(kBtnNormal);
    connect(m_replaceBtn, &QPushButton::clicked, this, &DilutionCard::onReplaceClicked);
    row1->addWidget(m_replaceBtn);

    vl->addLayout(row1);

    // ── Inline replace panel (hidden by default) ──────────────────────────
    buildReplacePanel(vl);

    // ── Row 2: progress bar │ qty label ──────────────────────────────────
    QHBoxLayout *row2 = new QHBoxLayout;
    row2->setSpacing(8);

    m_bar = new QProgressBar(this);
    m_bar->setFixedHeight(6);
    m_bar->setTextVisible(false);
    m_bar->setRange(0, 100);
    row2->addWidget(m_bar, 1);

    m_qtyLabel = new QLabel(this);
    m_qtyLabel->setStyleSheet("font-size:11px; color:#5B6B7B; min-width:48px; background:transparent;");
    m_qtyLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row2->addWidget(m_qtyLabel);

    vl->addLayout(row2);
}

// ── Panel ─────────────────────────────────────────────────────────────────────
void DilutionCard::buildReplacePanel(QVBoxLayout *vl)
{
    m_replacePanel = new QWidget(this);
    m_replacePanel->setObjectName("replacePanel");
    m_replacePanel->setStyleSheet(
        "QWidget#replacePanel { background:#EEF4F9; border-radius:4px; border:1px solid #C8DCF0; }"
    );
    m_replacePanel->setVisible(false);

    QVBoxLayout *pvl = new QVBoxLayout(m_replacePanel);
    pvl->setContentsMargins(8, 5, 8, 5);
    pvl->setSpacing(4);

    QLabel *hint = new QLabel("扫描或输入条码（留空确认则执行卸载）", m_replacePanel);
    hint->setStyleSheet("font-size:10px; color:#5B6B7B; background:transparent; border:none;");
    pvl->addWidget(hint);

    QHBoxLayout *inputRow = new QHBoxLayout;
    inputRow->setSpacing(5);

    m_barcodeEdit = new QLineEdit(m_replacePanel);
    m_barcodeEdit->setPlaceholderText("例: 1,SAA,SAA251101,50,20251101,20271101,1");
    m_barcodeEdit->setFixedHeight(26);
    m_barcodeEdit->setStyleSheet(
        "QLineEdit { border:1px solid #B7C3CE; border-radius:3px; padding:0 6px;"
        "  font-size:11px; color:#1F2A37; background:#FFFFFF; }"
        "QLineEdit:focus { border-color:#1464A5; }"
    );
    inputRow->addWidget(m_barcodeEdit, 1);

    QPushButton *confirmBtn = new QPushButton("确认", m_replacePanel);
    confirmBtn->setFixedHeight(26);
    confirmBtn->setStyleSheet(kBtnPrimary);
    connect(confirmBtn, &QPushButton::clicked, this, &DilutionCard::onConfirmClicked);
    inputRow->addWidget(confirmBtn);

    QPushButton *cancelBtn = new QPushButton("取消", m_replacePanel);
    cancelBtn->setFixedHeight(26);
    cancelBtn->setStyleSheet(kBtnSecondary);
    connect(cancelBtn, &QPushButton::clicked, this, &DilutionCard::onCancelClicked);
    inputRow->addWidget(cancelBtn);

    pvl->addLayout(inputRow);
    vl->addWidget(m_replacePanel);
}

// ── Slots ──────────────────────────────────────────────────────────────────────
void DilutionCard::onReplaceClicked()
{
    if (!m_panelOpen) {
        emit panelOpenRequested();   // ConsumablesTab closes all siblings first
        setPanelOpen(true);
    } else {
        setPanelOpen(false);
    }
}

void DilutionCard::onConfirmClicked()
{
    emit replaceRequested(m_data, m_barcodeEdit->text().trimmed());
    m_barcodeEdit->clear();
    setPanelOpen(false);
}

void DilutionCard::onCancelClicked()
{
    m_barcodeEdit->clear();
    setPanelOpen(false);
}

void DilutionCard::closePanel()
{
    if (m_panelOpen) {
        m_barcodeEdit->clear();
        setPanelOpen(false);
    }
}

void DilutionCard::setPanelOpen(bool open)
{
    m_panelOpen = open;
    m_replacePanel->setVisible(open);
    applyReplaceBtnStyle(open);
    if (open) m_barcodeEdit->setFocus();
}

// ── Data ───────────────────────────────────────────────────────────────────────
void DilutionCard::setData(const DilutionSolution &d)
{
    m_data = d;
    int pct = (d.capacity > 0) ? d.qty * 100 / d.capacity : 0;

    QColor iconColor;
    if (!d.loaded)     iconColor = QColor("#A5B0BC");
    else if (pct < 20) iconColor = QColor("#C0392B");
    else if (pct < 40) iconColor = QColor("#D08419");
    else               iconColor = QColor("#2E8B57");

    m_iconLabel->setPixmap(SvgIcon::pixmap("flask", 14, iconColor));

    // One-line label: bold name + slot + lot/expire
    QString lotInfo = d.loaded
        ? QString("批号: %1 · 有效期: %2").arg(d.lotNo, d.expire)
        : QString("批号: — · 有效期: —");
    m_nameLabel->setText(
        QString("<b>%1</b>&nbsp;<span style='color:#7B8FA1;font-size:10px;'>"
                "位置 %2&nbsp;&nbsp;%3</span>")
            .arg(d.name, d.slot, lotInfo)
    );

    m_bar->setValue(d.loaded ? pct : 0);
    applyBarStyle(pct, d.loaded);
    m_qtyLabel->setText(QString("%1 / %2").arg(d.qty).arg(d.capacity));
}

// ── Style helpers ──────────────────────────────────────────────────────────────
void DilutionCard::applyReplaceBtnStyle(bool active)
{
    m_replaceBtn->setStyleSheet(active ? kBtnActive : kBtnNormal);
}

void DilutionCard::applyBarStyle(int pct, bool loaded)
{
    QString css;
    if (!loaded) {
        css = "QProgressBar { border:1px dashed #B7C3CE; border-radius:3px; background:#F4F7FA; }"
              "QProgressBar::chunk { background:transparent; }";
    } else if (pct < 20) {
        css = "QProgressBar { border:none; border-radius:3px; background:#F4D0CC; }"
              "QProgressBar::chunk { background:#C0392B; border-radius:3px; }";
    } else if (pct < 40) {
        css = "QProgressBar { border:none; border-radius:3px; background:#F9E8C8; }"
              "QProgressBar::chunk { background:#D08419; border-radius:3px; }";
    } else {
        css = "QProgressBar { border:none; border-radius:3px; background:#C8E6D4; }"
              "QProgressBar::chunk { background:#2E8B57; border-radius:3px; }";
    }
    m_bar->setStyleSheet(css);
}
