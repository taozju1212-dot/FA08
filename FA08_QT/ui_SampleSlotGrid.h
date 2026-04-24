/********************************************************************************
** Form generated from reading UI file 'SampleSlotGrid.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMPLESLOTGRID_H
#define UI_SAMPLESLOTGRID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SampleSlotGrid
{
public:
    QVBoxLayout *outerLayout;
    QWidget *headerWidget;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QSpacerItem *headerSpacer;
    QCheckBox *selectAllCheck;
    QScrollArea *scrollArea;
    QWidget *slotContainer;
    QVBoxLayout *slotLayout;

    void setupUi(QWidget *SampleSlotGrid)
    {
        if (SampleSlotGrid->objectName().isEmpty())
            SampleSlotGrid->setObjectName(QString::fromUtf8("SampleSlotGrid"));
        SampleSlotGrid->resize(200, 500);
        outerLayout = new QVBoxLayout(SampleSlotGrid);
        outerLayout->setSpacing(0);
        outerLayout->setObjectName(QString::fromUtf8("outerLayout"));
        outerLayout->setContentsMargins(0, 0, 0, 0);
        headerWidget = new QWidget(SampleSlotGrid);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setMinimumSize(QSize(0, 36));
        headerWidget->setMaximumSize(QSize(16777215, 36));
        headerLayout = new QHBoxLayout(headerWidget);
        headerLayout->setSpacing(6);
        headerLayout->setObjectName(QString::fromUtf8("headerLayout"));
        headerLayout->setContentsMargins(10, 0, 10, 0);
        titleLabel = new QLabel(headerWidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));

        headerLayout->addWidget(titleLabel);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        headerLayout->addItem(headerSpacer);

        selectAllCheck = new QCheckBox(headerWidget);
        selectAllCheck->setObjectName(QString::fromUtf8("selectAllCheck"));

        headerLayout->addWidget(selectAllCheck);


        outerLayout->addWidget(headerWidget);

        scrollArea = new QScrollArea(SampleSlotGrid);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        slotContainer = new QWidget();
        slotContainer->setObjectName(QString::fromUtf8("slotContainer"));
        slotContainer->setGeometry(QRect(0, 0, 198, 460));
        slotLayout = new QVBoxLayout(slotContainer);
        slotLayout->setSpacing(0);
        slotLayout->setObjectName(QString::fromUtf8("slotLayout"));
        slotLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea->setWidget(slotContainer);

        outerLayout->addWidget(scrollArea);


        retranslateUi(SampleSlotGrid);

        QMetaObject::connectSlotsByName(SampleSlotGrid);
    } // setupUi

    void retranslateUi(QWidget *SampleSlotGrid)
    {
        titleLabel->setText(QCoreApplication::translate("SampleSlotGrid", "\346\240\267\346\234\254\344\275\215", nullptr));
        selectAllCheck->setText(QCoreApplication::translate("SampleSlotGrid", "\345\205\250\351\200\211", nullptr));
        (void)SampleSlotGrid;
    } // retranslateUi

};

namespace Ui {
    class SampleSlotGrid: public Ui_SampleSlotGrid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLESLOTGRID_H
