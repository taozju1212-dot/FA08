#include "SettingsTab.h"
#include "ui/widgets/SvgIcon.h"
#include <QVBoxLayout>
#include <QLabel>

SettingsTab::SettingsTab(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background:#EEF2F6;");
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setAlignment(Qt::AlignCenter);

    QLabel *ico = new QLabel(this);
    ico->setAlignment(Qt::AlignCenter);
    ico->setPixmap(SvgIcon::pixmap("settings", 48, QColor("#D6DEE6")));

    QLabel *lbl = new QLabel("系统设置", this);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("font-size:16px; color:#A5B0BC; margin-top:12px;");

    QLabel *sub = new QLabel("功能开发中…", this);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size:12px; color:#B7C3CE;");

    vl->addStretch();
    vl->addWidget(ico);
    vl->addWidget(lbl);
    vl->addWidget(sub);
    vl->addStretch();
}
