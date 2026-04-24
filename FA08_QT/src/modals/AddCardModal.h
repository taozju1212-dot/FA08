#pragma once
#include <QDialog>
#include <QList>
#include <QMap>
#include <QProgressBar>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include "data/AppData.h"

class AddCardModal : public QDialog {
    Q_OBJECT
public:
    explicit AddCardModal(const QList<ReagentCard> &cards,
                          const QMap<QString, int>  &neededByCode,
                          QWidget *parent = nullptr);

signals:
    void cardsLoaded(const QMap<QString, int> &loaded);

private slots:
    void onLoadCard();
    void onProgressTick();

private:
    QList<ReagentCard>   m_cards;
    QMap<QString, int>   m_needed;
    QString              m_selectedCode;
    QString              m_loadingCode;
    int                  m_progress = 0;
    int                  m_doneCount = 0;

    QWidget     *m_listWidget;
    QProgressBar *m_progressBar;
    QLabel      *m_progressLabel;
    QPushButton *m_loadBtn;
    QTimer       m_timer;

    QMap<QString, QWidget*> m_cardWidgets;

    void buildUI();
    void buildCardList();
    void selectCard(const QString &code);
    void updateLoadButton();
    QWidget *makeCardWidget(const QString &code, int need);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
