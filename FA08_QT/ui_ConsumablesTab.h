/********************************************************************************
** Form generated from reading UI file 'ConsumablesTab.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSUMABLESTAB_H
#define UI_CONSUMABLESTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConsumablesTab
{
public:
    QHBoxLayout *mainLayout;
    QFrame *leftPanel;
    QVBoxLayout *leftLayout;
    QWidget *leftHeader;
    QHBoxLayout *leftHeaderLayout;
    QLabel *leftTitleLabel;
    QSpacerItem *leftHeaderSpacer;
    QTableWidget *calibTable;
    QWidget *leftFooter;
    QHBoxLayout *footerLayout;
    QPushButton *importBtn;
    QPushButton *deleteBtn;
    QPushButton *clearBtn;
    QSpacerItem *footerSpacer;
    QPushButton *prevBtn;
    QLabel *pageLabel;
    QPushButton *nextBtn;
    QScrollArea *rightScrollArea;
    QWidget *rightScrollContents;
    QVBoxLayout *rightLayout;
    QFrame *dilutionGroup;
    QVBoxLayout *dilutionGroupLayout;
    QLabel *dilutionTitle;
    QWidget *dilutionCardsWidget;
    QVBoxLayout *dilutionLayout;
    QFrame *consumableGroup;
    QVBoxLayout *consumableGroupLayout;
    QLabel *consumableTitle;
    QWidget *consumableCardsWidget;
    QVBoxLayout *consumableLayout;
    QSpacerItem *rightBottomSpacer;

    void setupUi(QWidget *ConsumablesTab)
    {
        if (ConsumablesTab->objectName().isEmpty())
            ConsumablesTab->setObjectName(QString::fromUtf8("ConsumablesTab"));
        ConsumablesTab->resize(800, 500);
        mainLayout = new QHBoxLayout(ConsumablesTab);
        mainLayout->setSpacing(10);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setContentsMargins(10, 10, 10, 10);
        leftPanel = new QFrame(ConsumablesTab);
        leftPanel->setObjectName(QString::fromUtf8("leftPanel"));
        leftPanel->setMinimumSize(QSize(355, 0));
        leftPanel->setMaximumSize(QSize(355, 16777215));
        leftPanel->setFrameShape(QFrame::NoFrame);
        leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setSpacing(0);
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));
        leftLayout->setContentsMargins(0, 0, 0, 0);
        leftHeader = new QWidget(leftPanel);
        leftHeader->setObjectName(QString::fromUtf8("leftHeader"));
        leftHeader->setMinimumSize(QSize(0, 40));
        leftHeader->setMaximumSize(QSize(16777215, 40));
        leftHeaderLayout = new QHBoxLayout(leftHeader);
        leftHeaderLayout->setSpacing(8);
        leftHeaderLayout->setObjectName(QString::fromUtf8("leftHeaderLayout"));
        leftHeaderLayout->setContentsMargins(14, 0, 14, 0);
        leftTitleLabel = new QLabel(leftHeader);
        leftTitleLabel->setObjectName(QString::fromUtf8("leftTitleLabel"));

        leftHeaderLayout->addWidget(leftTitleLabel);

        leftHeaderSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        leftHeaderLayout->addItem(leftHeaderSpacer);


        leftLayout->addWidget(leftHeader);

        calibTable = new QTableWidget(leftPanel);
        if (calibTable->columnCount() < 4)
            calibTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        calibTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        calibTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        calibTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        calibTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        calibTable->setObjectName(QString::fromUtf8("calibTable"));
        calibTable->setColumnCount(4);
        calibTable->horizontalHeader()->setStretchLastSection(false);

        leftLayout->addWidget(calibTable);

        leftFooter = new QWidget(leftPanel);
        leftFooter->setObjectName(QString::fromUtf8("leftFooter"));
        leftFooter->setMinimumSize(QSize(0, 44));
        leftFooter->setMaximumSize(QSize(16777215, 44));
        footerLayout = new QHBoxLayout(leftFooter);
        footerLayout->setSpacing(6);
        footerLayout->setObjectName(QString::fromUtf8("footerLayout"));
        footerLayout->setContentsMargins(12, 7, 12, 7);
        importBtn = new QPushButton(leftFooter);
        importBtn->setObjectName(QString::fromUtf8("importBtn"));

        footerLayout->addWidget(importBtn);

        deleteBtn = new QPushButton(leftFooter);
        deleteBtn->setObjectName(QString::fromUtf8("deleteBtn"));

        footerLayout->addWidget(deleteBtn);

        clearBtn = new QPushButton(leftFooter);
        clearBtn->setObjectName(QString::fromUtf8("clearBtn"));

        footerLayout->addWidget(clearBtn);

        footerSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        footerLayout->addItem(footerSpacer);

        prevBtn = new QPushButton(leftFooter);
        prevBtn->setObjectName(QString::fromUtf8("prevBtn"));

        footerLayout->addWidget(prevBtn);

        pageLabel = new QLabel(leftFooter);
        pageLabel->setObjectName(QString::fromUtf8("pageLabel"));
        pageLabel->setAlignment(Qt::AlignCenter);

        footerLayout->addWidget(pageLabel);

        nextBtn = new QPushButton(leftFooter);
        nextBtn->setObjectName(QString::fromUtf8("nextBtn"));

        footerLayout->addWidget(nextBtn);


        leftLayout->addWidget(leftFooter);


        mainLayout->addWidget(leftPanel);

        rightScrollArea = new QScrollArea(ConsumablesTab);
        rightScrollArea->setObjectName(QString::fromUtf8("rightScrollArea"));
        rightScrollArea->setWidgetResizable(true);
        rightScrollArea->setFrameShape(QFrame::NoFrame);
        rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rightScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        rightScrollContents = new QWidget();
        rightScrollContents->setObjectName(QString::fromUtf8("rightScrollContents"));
        rightScrollContents->setGeometry(QRect(0, 0, 435, 590));
        rightLayout = new QVBoxLayout(rightScrollContents);
        rightLayout->setSpacing(10);
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));
        rightLayout->setContentsMargins(0, 0, 0, 0);
        dilutionGroup = new QFrame(rightScrollContents);
        dilutionGroup->setObjectName(QString::fromUtf8("dilutionGroup"));
        dilutionGroup->setFrameShape(QFrame::NoFrame);
        dilutionGroupLayout = new QVBoxLayout(dilutionGroup);
        dilutionGroupLayout->setSpacing(8);
        dilutionGroupLayout->setObjectName(QString::fromUtf8("dilutionGroupLayout"));
        dilutionGroupLayout->setContentsMargins(14, 10, 14, 10);
        dilutionTitle = new QLabel(dilutionGroup);
        dilutionTitle->setObjectName(QString::fromUtf8("dilutionTitle"));

        dilutionGroupLayout->addWidget(dilutionTitle);

        dilutionCardsWidget = new QWidget(dilutionGroup);
        dilutionCardsWidget->setObjectName(QString::fromUtf8("dilutionCardsWidget"));
        dilutionLayout = new QVBoxLayout(dilutionCardsWidget);
        dilutionLayout->setSpacing(8);
        dilutionLayout->setObjectName(QString::fromUtf8("dilutionLayout"));
        dilutionLayout->setContentsMargins(0, 0, 0, 0);

        dilutionGroupLayout->addWidget(dilutionCardsWidget);


        rightLayout->addWidget(dilutionGroup);

        consumableGroup = new QFrame(rightScrollContents);
        consumableGroup->setObjectName(QString::fromUtf8("consumableGroup"));
        consumableGroup->setFrameShape(QFrame::NoFrame);
        consumableGroupLayout = new QVBoxLayout(consumableGroup);
        consumableGroupLayout->setSpacing(8);
        consumableGroupLayout->setObjectName(QString::fromUtf8("consumableGroupLayout"));
        consumableGroupLayout->setContentsMargins(14, 10, 14, 10);
        consumableTitle = new QLabel(consumableGroup);
        consumableTitle->setObjectName(QString::fromUtf8("consumableTitle"));

        consumableGroupLayout->addWidget(consumableTitle);

        consumableCardsWidget = new QWidget(consumableGroup);
        consumableCardsWidget->setObjectName(QString::fromUtf8("consumableCardsWidget"));
        consumableLayout = new QVBoxLayout(consumableCardsWidget);
        consumableLayout->setSpacing(8);
        consumableLayout->setObjectName(QString::fromUtf8("consumableLayout"));
        consumableLayout->setContentsMargins(0, 0, 0, 0);

        consumableGroupLayout->addWidget(consumableCardsWidget);


        rightLayout->addWidget(consumableGroup);

        rightBottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        rightLayout->addItem(rightBottomSpacer);

        rightScrollArea->setWidget(rightScrollContents);

        mainLayout->addWidget(rightScrollArea);


        retranslateUi(ConsumablesTab);

        QMetaObject::connectSlotsByName(ConsumablesTab);
    } // setupUi

    void retranslateUi(QWidget *ConsumablesTab)
    {
        leftTitleLabel->setText(QCoreApplication::translate("ConsumablesTab", "\351\241\271\347\233\256\346\240\207\346\233\262", nullptr));
        QTableWidgetItem *___qtablewidgetitem = calibTable->horizontalHeaderItem(1);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ConsumablesTab", "\351\241\271\347\233\256\345\220\215\347\247\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = calibTable->horizontalHeaderItem(2);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ConsumablesTab", "\346\211\271\346\254\241", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = calibTable->horizontalHeaderItem(3);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ConsumablesTab", "\345\244\261\346\225\210\346\227\245\346\234\237", nullptr));
        importBtn->setText(QCoreApplication::translate("ConsumablesTab", "\345\257\274\345\205\245", nullptr));
        deleteBtn->setText(QCoreApplication::translate("ConsumablesTab", "\345\210\240\351\231\244", nullptr));
        clearBtn->setText(QCoreApplication::translate("ConsumablesTab", "\346\270\205\347\251\272", nullptr));
        prevBtn->setText(QCoreApplication::translate("ConsumablesTab", "\344\270\212\351\241\265", nullptr));
        pageLabel->setText(QCoreApplication::translate("ConsumablesTab", "1 / 1", nullptr));
        nextBtn->setText(QCoreApplication::translate("ConsumablesTab", "\344\270\213\351\241\265", nullptr));
        dilutionTitle->setText(QCoreApplication::translate("ConsumablesTab", "\347\250\200\351\207\212\346\266\262\345\272\223\345\255\230", nullptr));
        consumableTitle->setText(QCoreApplication::translate("ConsumablesTab", "\350\200\227\346\235\220\345\272\223\345\255\230", nullptr));
        (void)ConsumablesTab;
    } // retranslateUi

};

namespace Ui {
    class ConsumablesTab: public Ui_ConsumablesTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSUMABLESTAB_H
