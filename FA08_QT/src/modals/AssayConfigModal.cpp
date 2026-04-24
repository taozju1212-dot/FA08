#include "AssayConfigModal.h"
#include "ui/widgets/SvgIcon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

AssayConfigModal::AssayConfigModal(const QStringList &selectedAssays,
                                   const QMap<QString, AssayCfg> &cfg,
                                   QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::FramelessWindowHint),
      m_assays(selectedAssays), m_cfg(cfg)
{
    setFixedSize(460, qMin(420, 120 + selectedAssays.size() * 56));
    setStyleSheet(
        "QDialog { background:#FFFFFF; border-radius:8px; border:1px solid #D6DEE6; }"
    );

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // ── Header ──
    QWidget *hdr = new QWidget(this);
    hdr->setFixedHeight(42);
    hdr->setStyleSheet("background:#0F4E85; border-radius:8px 8px 0 0;");
    QHBoxLayout *hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(14, 0, 14, 0); hl->setSpacing(8);
    QLabel *titleLbl = new QLabel("检测项目参数配置", hdr);
    titleLbl->setStyleSheet("font-size:13px; font-weight:700; color:#fff;");
    hl->addWidget(titleLbl); hl->addStretch();
    QPushButton *closeBtn = new QPushButton("×", hdr);
    closeBtn->setFixedSize(24, 24);
    closeBtn->setStyleSheet(
        "QPushButton{background:transparent;border:none;color:rgba(255,255,255,0.8);font-size:16px;}"
        "QPushButton:hover{color:#fff;}"
    );
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
    hl->addWidget(closeBtn);
    vl->addWidget(hdr);

    // ── Column header ──
    QWidget *colHdr = new QWidget(this);
    colHdr->setFixedHeight(30);
    colHdr->setStyleSheet("background:#F4F7FA; border-bottom:1px solid #E6ECF2;");
    QHBoxLayout *chl = new QHBoxLayout(colHdr);
    chl->setContentsMargins(14, 0, 14, 0); chl->setSpacing(0);
    QLabel *prjLbl = new QLabel("检测项目", colHdr);
    prjLbl->setStyleSheet("font-size:11px; font-weight:700; color:#334155;");
    QLabel *repLbl = new QLabel("重复次数", colHdr);
    repLbl->setFixedWidth(110);
    repLbl->setAlignment(Qt::AlignCenter);
    repLbl->setStyleSheet("font-size:11px; font-weight:700; color:#334155;");
    QLabel *dilLbl = new QLabel("稀释倍数", colHdr);
    dilLbl->setFixedWidth(130);
    dilLbl->setAlignment(Qt::AlignCenter);
    dilLbl->setStyleSheet("font-size:11px; font-weight:700; color:#334155;");
    chl->addWidget(prjLbl, 1); chl->addWidget(repLbl); chl->addWidget(dilLbl);
    vl->addWidget(colHdr);

    // ── Scrollable rows ──
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea{border:none;}");
    QWidget *rowsW = new QWidget;
    QVBoxLayout *rowsVL = new QVBoxLayout(rowsW);
    rowsVL->setContentsMargins(0, 0, 0, 0);
    rowsVL->setSpacing(0);

    QList<Assay> assayDefs = AppData::assays();
    for (const QString &code : m_assays) {
        // Find assay definition
        QString assayName;
        for (const auto &a : assayDefs)
            if (a.code == code) { assayName = a.name; break; }

        AssayCfg curCfg = m_cfg.contains(code) ? m_cfg[code] : AssayCfg{1, 1.0};

        QWidget *row = new QWidget(rowsW);
        row->setFixedHeight(52);
        row->setStyleSheet("border-bottom:1px solid #E6ECF2;");
        QHBoxLayout *rl = new QHBoxLayout(row);
        rl->setContentsMargins(14, 6, 14, 6); rl->setSpacing(0);

        // Project info
        QWidget *prjW = new QWidget(row);
        QVBoxLayout *pvl = new QVBoxLayout(prjW);
        pvl->setContentsMargins(0, 0, 0, 0); pvl->setSpacing(1);
        QLabel *codeL = new QLabel(code, prjW);
        codeL->setStyleSheet("font-size:13px; font-weight:700; color:#0F4E85;");
        QLabel *nameL = new QLabel(assayName, prjW);
        nameL->setStyleSheet("font-size:10px; color:#5B6B7B;");
        pvl->addWidget(codeL); pvl->addWidget(nameL);
        rl->addWidget(prjW, 1);

        // Repeat combobox
        QComboBox *repBox = new QComboBox(row);
        repBox->setFixedSize(90, 32);
        repBox->setStyleSheet(
            "QComboBox{border:1px solid #B7C3CE;border-radius:4px;padding:0 6px;"
            "font-size:13px;font-weight:700;background:#fff;}"
            "QComboBox:focus{border-color:#1E78BE;}"
        );
        for (int n = 1; n <= 10; ++n)
            repBox->addItem(QString("%1 次").arg(n), n);
        repBox->setCurrentIndex(curCfg.repeat - 1);
        connect(repBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this, code, repBox](int) {
                m_cfg[code].repeat = repBox->currentData().toInt();
            });

        QWidget *repW = new QWidget(row);
        repW->setFixedWidth(110);
        QHBoxLayout *repl = new QHBoxLayout(repW);
        repl->setAlignment(Qt::AlignCenter);
        repl->addWidget(repBox);
        rl->addWidget(repW);

        // Dilution input
        QLineEdit *dilEdit = new QLineEdit(row);
        dilEdit->setFixedSize(100, 32);
        dilEdit->setAlignment(Qt::AlignCenter);
        dilEdit->setStyleSheet(
            "QLineEdit{border:1px solid #B7C3CE;border-radius:4px;font-size:13px;font-weight:700;}"
            "QLineEdit:focus{border-color:#1E78BE;}"
        );
        dilEdit->setText(curCfg.dilution == 1.0 ? "原液" : QString("%1×").arg((int)curCfg.dilution));
        dilEdit->setReadOnly(true); // would connect to virtual keyboard in full impl
        connect(dilEdit, &QLineEdit::textChanged, this, [this, code, dilEdit](const QString &) {
            QString t = dilEdit->text().remove('×');
            bool ok; double v = t.toDouble(&ok);
            m_cfg[code].dilution = (ok && v > 0) ? v : 1.0;
        });

        QWidget *dilW = new QWidget(row);
        dilW->setFixedWidth(130);
        QHBoxLayout *dill = new QHBoxLayout(dilW);
        dill->setAlignment(Qt::AlignCenter);
        dill->addWidget(dilEdit);
        rl->addWidget(dilW);

        m_rows[code] = {repBox, dilEdit};
        rowsVL->addWidget(row);
    }
    rowsVL->addStretch();
    scroll->setWidget(rowsW);
    vl->addWidget(scroll, 1);

    // ── Footer ──
    QWidget *footer = new QWidget(this);
    footer->setFixedHeight(46);
    footer->setStyleSheet("background:#F4F7FA; border-top:1px solid #E6ECF2; border-radius:0 0 8px 8px;");
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(14, 6, 14, 6); fl->setSpacing(8);
    fl->addStretch();
    QPushButton *okBtn = new QPushButton(footer);
    okBtn->setText("确认");
    okBtn->setIcon(SvgIcon::icon("check", 12, Qt::white));
    okBtn->setIconSize(QSize(12, 12));
    okBtn->setFixedHeight(32);
    okBtn->setStyleSheet(
        "QPushButton{background:#1464A5;color:#fff;border:1px solid #0F4E85;"
        "border-radius:5px;padding:0 20px;font-size:13px;font-weight:600;}"
        "QPushButton:hover{background:#0F4E85;}"
    );
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    fl->addWidget(okBtn);
    vl->addWidget(footer);
}
