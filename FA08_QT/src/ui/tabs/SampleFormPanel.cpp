#include "SampleFormPanel.h"
#include "ui/widgets/SvgIcon.h"
#include "modals/AssayConfigModal.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QEvent>

static QString inputStyle(bool focused = false) {
    return QString(
        "QLineEdit { height:30px; border:1px solid %1; border-radius:4px;"
        "  background:#fff; padding:0 10px; font-size:13px; color:#1F2A37; }"
    ).arg(focused ? "#1E78BE" : "#B7C3CE");
}

SampleFormPanel::SampleFormPanel(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(280);
    buildUI();
    clearForm();
}

void SampleFormPanel::buildUI()
{
    setStyleSheet("background:#FFFFFF; border-left:1px solid #E6ECF2;");
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // ── Header ──
    QWidget *hdr = new QWidget(this);
    hdr->setFixedHeight(36);
    hdr->setStyleSheet("background:#F4F7FA; border-bottom:1px solid #E6ECF2;");
    QHBoxLayout *hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(14, 0, 14, 0);
    auto *editIco = new SvgIconLabel("edit", 13, QColor("#1F2A37"), hdr);
    QLabel *hdrTitle = new QLabel("样本申请", hdr);
    hdrTitle->setStyleSheet("font-size:13px; font-weight:700; color:#1F2A37;");
    m_slotLabel = new QLabel("—", hdr);
    m_slotLabel->setStyleSheet("font-size:11px; color:#5B6B7B; margin-left:8px;");
    hl->addWidget(editIco); hl->addWidget(hdrTitle); hl->addWidget(m_slotLabel); hl->addStretch();
    vl->addWidget(hdr);

    // ── Body (scrollable if needed) ──
    QWidget *body = new QWidget(this);
    QVBoxLayout *bl = new QVBoxLayout(body);
    bl->setContentsMargins(14, 8, 14, 8);
    bl->setSpacing(7);

    auto makeLabel = [](const QString &text, bool required = false) -> QLabel* {
        QLabel *l = new QLabel(text + (required ? " *" : ""));
        l->setStyleSheet(QString("font-size:11px; color:%1; font-weight:500;")
            .arg(required ? "#C0392B" : "#5B6B7B"));
        return l;
    };

    // ── Row 1: 流水号 + 样本ID ──
    QGridLayout *row1 = new QGridLayout;
    row1->setSpacing(8);
    row1->setContentsMargins(0,0,0,0);
    // Seq
    QVBoxLayout *seqVL = new QVBoxLayout;
    seqVL->setSpacing(3); seqVL->setContentsMargins(0,0,0,0);
    seqVL->addWidget(makeLabel("流水号"));
    m_seqLabel = new QLabel("001", body);
    m_seqLabel->setStyleSheet(
        "font-size:13px; font-weight:700; color:#1464A5; background:#E8EEF3;"
        "border:1px solid #B7C3CE; border-radius:4px; padding:0 10px; height:30px; min-height:30px;"
    );
    m_seqLabel->setFixedHeight(30);
    seqVL->addWidget(m_seqLabel);
    row1->addLayout(seqVL, 0, 0);
    // Sample ID (2 cols)
    QVBoxLayout *sidVL = new QVBoxLayout;
    sidVL->setSpacing(3); sidVL->setContentsMargins(0,0,0,0);
    sidVL->addWidget(makeLabel("样本ID", true));
    m_sampleIdEdit = new QLineEdit(body);
    m_sampleIdEdit->setReadOnly(true);
    m_sampleIdEdit->setPlaceholderText("点击输入或自动扫码");
    m_sampleIdEdit->setFixedHeight(30);
    m_sampleIdEdit->setStyleSheet(inputStyle(false));
    connect(m_sampleIdEdit, &QLineEdit::selectionChanged, this, [this]{ onFieldClicked("sampleId"); });
    m_sampleIdEdit->installEventFilter(this);
    sidVL->addWidget(m_sampleIdEdit);
    row1->addLayout(sidVL, 0, 1, 1, 2);
    // Name (col 0 row 1)
    QVBoxLayout *nameVL = new QVBoxLayout;
    nameVL->setSpacing(3); nameVL->setContentsMargins(0,0,0,0);
    nameVL->addWidget(makeLabel("姓名", true));
    m_nameEdit = new QLineEdit(body);
    m_nameEdit->setReadOnly(true);
    m_nameEdit->setPlaceholderText("点击输入");
    m_nameEdit->setFixedHeight(30);
    m_nameEdit->setStyleSheet(inputStyle(false));
    m_nameEdit->installEventFilter(this);
    nameVL->addWidget(m_nameEdit);
    row1->addLayout(nameVL, 1, 0);
    // Age (col 1 row 1)
    QVBoxLayout *ageVL = new QVBoxLayout;
    ageVL->setSpacing(3); ageVL->setContentsMargins(0,0,0,0);
    ageVL->addWidget(makeLabel("年龄"));
    QHBoxLayout *agel = new QHBoxLayout; agel->setSpacing(4);
    m_ageEdit = new QLineEdit(body);
    m_ageEdit->setReadOnly(true);
    m_ageEdit->setPlaceholderText("—");
    m_ageEdit->setFixedHeight(30);
    m_ageEdit->setStyleSheet(inputStyle(false));
    m_ageEdit->installEventFilter(this);
    m_ageUnitCombo = new QComboBox(body);
    m_ageUnitCombo->addItems(AppData::ageUnits());
    m_ageUnitCombo->setFixedSize(50, 30);
    m_ageUnitCombo->setStyleSheet(
        "QComboBox{border:1px solid #B7C3CE;border-radius:4px;padding:0 4px;font-size:12px;}"
    );
    connect(m_ageUnitCombo, &QComboBox::currentTextChanged, this, [this](const QString &t){ m_form.ageUnit = t; });
    agel->addWidget(m_ageEdit,1); agel->addWidget(m_ageUnitCombo);
    ageVL->addLayout(agel);
    row1->addLayout(ageVL, 1, 1);
    // Gender (col 2 row 1)
    QVBoxLayout *genVL = new QVBoxLayout;
    genVL->setSpacing(3); genVL->setContentsMargins(0,0,0,0);
    genVL->addWidget(makeLabel("性别"));
    QHBoxLayout *genl = new QHBoxLayout; genl->setSpacing(5);
    QStringList genders = AppData::genders();
    for (int i = 0; i < 2; ++i) {
        m_genderBtns[i] = new QPushButton(genders[i], body);
        m_genderBtns[i]->setFixedHeight(30);
        int ii = i;
        connect(m_genderBtns[i], &QPushButton::clicked, this, [this, ii]{
            m_form.gender = AppData::genders()[ii];
            updateGenderButtons();
        });
        genl->addWidget(m_genderBtns[i]);
    }
    genVL->addLayout(genl);
    row1->addLayout(genVL, 1, 2);
    row1->setColumnStretch(0, 1); row1->setColumnStretch(1, 1); row1->setColumnStretch(2, 1);
    bl->addLayout(row1);

    // ── 样本类型 ──
    bl->addWidget(makeLabel("样本类型"));
    QHBoxLayout *typeRow = new QHBoxLayout; typeRow->setSpacing(5);
    QStringList types = AppData::sampleTypes();
    for (int i = 0; i < 5; ++i) {
        m_typeBtns[i] = new QPushButton(types[i], body);
        m_typeBtns[i]->setFixedHeight(30);
        int ii = i;
        connect(m_typeBtns[i], &QPushButton::clicked, this, [this, ii]{
            m_form.type = AppData::sampleTypes()[ii];
            updateTypeButtons();
        });
        typeRow->addWidget(m_typeBtns[i]);
    }
    bl->addLayout(typeRow);

    // ── 检测项目 ──
    QHBoxLayout *assayHdr = new QHBoxLayout;
    QLabel *assayLbl = makeLabel("检测项目", true);
    QPushButton *cfgBtn = new QPushButton(body);
    cfgBtn->setFixedSize(28, 24);
    cfgBtn->setIcon(SvgIcon::icon("chevron_right", 13, QColor("#1464A5")));
    cfgBtn->setIconSize(QSize(13, 13));
    cfgBtn->setStyleSheet(
        "QPushButton{border:1px solid #D6DEE6;border-radius:4px;background:#EEF4F9;}"
        "QPushButton:hover{background:#DCE8F2;}"
    );
    connect(cfgBtn, &QPushButton::clicked, this, &SampleFormPanel::onShowAssayConfig);
    assayHdr->addWidget(assayLbl); assayHdr->addStretch(); assayHdr->addWidget(cfgBtn);
    bl->addLayout(assayHdr);

    QGridLayout *assayGrid = new QGridLayout;
    assayGrid->setSpacing(6);
    assayGrid->setContentsMargins(0,0,0,0);
    QList<Assay> assays = AppData::assays();
    for (int i = 0; i < 6; ++i) {
        m_assayBtns[i] = new QPushButton(body);
        m_assayBtns[i]->setFixedHeight(46);
        QString code = assays[i].code;
        QString name = assays[i].name;
        int ii = i;
        connect(m_assayBtns[i], &QPushButton::clicked, this, [this, ii]{ onAssayToggled(AppData::assays()[ii].code); });
        assayGrid->addWidget(m_assayBtns[i], i / 2, i % 2);
    }
    bl->addLayout(assayGrid);
    bl->addStretch();

    vl->addWidget(body, 1);

    // ── Footer ──
    QWidget *footer = new QWidget(this);
    footer->setFixedHeight(48);
    footer->setStyleSheet("background:#F4F7FA; border-top:1px solid #E6ECF2;");
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(14, 6, 14, 6); fl->setSpacing(8);
    fl->addStretch();
    m_cancelBtn = new QPushButton("取消", footer);
    m_cancelBtn->setFixedHeight(32);
    m_cancelBtn->setStyleSheet(
        "QPushButton{background:#fff;border:1px solid #B7C3CE;border-radius:5px;padding:0 14px;font-size:13px;}"
        "QPushButton:hover{background:#EEF4F9;}"
    );
    connect(m_cancelBtn, &QPushButton::clicked, this, &SampleFormPanel::onCancel);
    m_submitBtn = new QPushButton(footer);
    m_submitBtn->setText("提交申请");
    m_submitBtn->setIcon(SvgIcon::icon("check", 12, Qt::white));
    m_submitBtn->setIconSize(QSize(12,12));
    m_submitBtn->setFixedHeight(32);
    m_submitBtn->setStyleSheet(
        "QPushButton{background:#1464A5;color:#fff;border:1px solid #0F4E85;border-radius:5px;padding:0 14px;font-size:13px;font-weight:600;}"
        "QPushButton:hover{background:#0F4E85;}"
        "QPushButton:disabled{background:#7FB2DC;border-color:#7FB2DC;}"
    );
    connect(m_submitBtn, &QPushButton::clicked, this, &SampleFormPanel::onSubmit);
    fl->addWidget(m_cancelBtn); fl->addWidget(m_submitBtn);
    vl->addWidget(footer);

    // ── Virtual keyboard (floats at bottom of parent MainWindow) ──
    // It will be parented to the nearest QMainWindow or placed by DetectTab
    m_keyboard = nullptr; // created lazily with parent window
}

bool SampleFormPanel::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == m_sampleIdEdit) { onFieldClicked("sampleId"); return true; }
        if (obj == m_nameEdit)     { onFieldClicked("name");     return true; }
        if (obj == m_ageEdit)      { onFieldClicked("age");      return true; }
    }
    return QWidget::eventFilter(obj, event);
}

void SampleFormPanel::loadSlot(int index, const SampleSlot &slot, int nextSeq)
{
    m_slotIndex = index;
    m_form      = slot;
    m_nextSeq   = nextSeq;
    if (m_form.gender.isEmpty()) m_form.gender = "男";
    if (m_form.type.isEmpty())   m_form.type   = "血清";
    if (m_form.ageUnit.isEmpty()) m_form.ageUnit = "岁";

    m_slotLabel->setText(QString("位置 <b style='color:#1464A5'>%1</b>").arg(slot.id));
    m_seqLabel->setText(QString("%1").arg(nextSeq, 3, 10, QChar('0')));

    refreshForm();
    setVisible(true);
}

void SampleFormPanel::clearForm()
{
    m_slotIndex = -1;
    m_form      = SampleSlot();
    setVisible(false);
    if (m_keyboard) m_keyboard->hide();
}

void SampleFormPanel::refreshForm()
{
    m_sampleIdEdit->setText(m_form.sampleId);
    m_nameEdit->setText(m_form.name);
    m_ageEdit->setText(m_form.age > 0 ? QString::number(m_form.age) : "");
    int ageIdx = AppData::ageUnits().indexOf(m_form.ageUnit);
    if (ageIdx >= 0) m_ageUnitCombo->setCurrentIndex(ageIdx);
    updateGenderButtons();
    updateTypeButtons();
    updateAssayButtons();
    validateForm();
}

void SampleFormPanel::updateGenderButtons()
{
    QStringList g = AppData::genders();
    for (int i = 0; i < 2; ++i) {
        bool on = (g[i] == m_form.gender);
        m_genderBtns[i]->setStyleSheet(on
            ? "QPushButton{border:1px solid #1464A5;background:#DCE8F2;color:#1464A5;border-radius:4px;font-size:13px;font-weight:700;}"
            : "QPushButton{border:1px solid #B7C3CE;background:#fff;color:#1F2A37;border-radius:4px;font-size:13px;}"
              "QPushButton:hover{background:#EEF4F9;}"
        );
    }
}

void SampleFormPanel::updateTypeButtons()
{
    QStringList t = AppData::sampleTypes();
    for (int i = 0; i < 5; ++i) {
        bool on = (t[i] == m_form.type);
        m_typeBtns[i]->setStyleSheet(on
            ? "QPushButton{border:1px solid #1464A5;background:#DCE8F2;color:#1464A5;border-radius:4px;font-size:11px;font-weight:700;}"
            : "QPushButton{border:1px solid #B7C3CE;background:#fff;color:#334155;border-radius:4px;font-size:11px;}"
              "QPushButton:hover{background:#EEF4F9;}"
        );
    }
}

void SampleFormPanel::updateAssayButtons()
{
    QList<Assay> assays = AppData::assays();
    for (int i = 0; i < 6; ++i) {
        bool on = m_form.assays.contains(assays[i].code);
        m_assayBtns[i]->setText(QString("%1\n%2").arg(assays[i].code).arg(assays[i].name));
        m_assayBtns[i]->setStyleSheet(on
            ? "QPushButton{border:1.5px solid #1464A5;background:#EEF4F9;border-radius:5px;font-size:11px;font-weight:700;color:#0B3C66;text-align:left;padding:6px 8px;}"
              "QPushButton:hover{background:#DCE8F2;}"
            : "QPushButton{border:1.5px solid #D6DEE6;background:#fff;border-radius:5px;font-size:11px;color:#1F2A37;text-align:left;padding:6px 8px;}"
              "QPushButton:hover{background:#EEF4F9;border-color:#7FB2DC;}"
        );
    }
}

void SampleFormPanel::validateForm()
{
    bool valid = !m_form.sampleId.isEmpty()
              && !m_form.name.isEmpty()
              && !m_form.assays.isEmpty();
    m_submitBtn->setEnabled(valid);
}

void SampleFormPanel::onFieldClicked(const QString &field)
{
    m_activeField = field;

    // Get or create keyboard
    if (!m_keyboard) {
        // Parent to the main window so it overlays everything
        QWidget *topW = window();
        m_keyboard = new VirtualKeyboard(topW);
        connect(m_keyboard, &VirtualKeyboard::valueChanged,  this, &SampleFormPanel::onKeyboardValue);
        connect(m_keyboard, &VirtualKeyboard::confirmed,     this, &SampleFormPanel::onKeyboardConfirmed);
    }

    QString curVal;
    VirtualKeyboard::Mode mode = VirtualKeyboard::QWERTY;
    if (field == "sampleId") { curVal = m_form.sampleId; }
    else if (field == "name") { curVal = m_form.name; }
    else if (field == "age")  { curVal = m_form.age > 0 ? QString::number(m_form.age) : ""; mode = VirtualKeyboard::Numeric; }

    // Position at bottom of parent window
    QWidget *topW = window();
    int kbW = (mode == VirtualKeyboard::Numeric) ? 260 : 640;
    int kbH = (mode == VirtualKeyboard::Numeric) ? 260 : 260;
    int x   = (mode == VirtualKeyboard::Numeric) ? (topW->width() - kbW - 10) : (topW->width() - kbW) / 2;
    m_keyboard->resize(kbW, kbH);
    m_keyboard->move(x, topW->height() - kbH - 60); // above bottom bar
    m_keyboard->showFor(mode, curVal, field);
}

void SampleFormPanel::onKeyboardValue(const QString &field, const QString &value)
{
    if (field == "sampleId") { m_form.sampleId = value; m_sampleIdEdit->setText(value); }
    else if (field == "name") { m_form.name = value; m_nameEdit->setText(value); }
    else if (field == "age")  { m_form.age = value.toInt(); m_ageEdit->setText(value); }
    validateForm();
}

void SampleFormPanel::onKeyboardConfirmed(const QString &field, const QString &value)
{
    onKeyboardValue(field, value);
    if (m_keyboard) m_keyboard->hide();
}

void SampleFormPanel::onAssayToggled(const QString &code)
{
    if (m_form.assays.contains(code))
        m_form.assays.removeAll(code);
    else
        m_form.assays.append(code);
    updateAssayButtons();
    validateForm();
}

void SampleFormPanel::onShowAssayConfig()
{
    if (m_form.assays.isEmpty()) return;
    AssayConfigModal *dlg = new AssayConfigModal(m_form.assays, m_form.assayCfg, this);
    if (dlg->exec() == QDialog::Accepted)
        m_form.assayCfg = dlg->config();
}

void SampleFormPanel::onSubmit()
{
    if (m_slotIndex < 0) return;
    m_form.status = SampleStatus::Scheduled;
    emit submitted(m_slotIndex, m_form);
    clearForm();
}

void SampleFormPanel::onCancel()
{
    clearForm();
    emit cancelled();
}
