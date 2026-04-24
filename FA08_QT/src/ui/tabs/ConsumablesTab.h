#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QList>
#include "data/AppData.h"

namespace Ui { class ConsumablesTab; }
class DilutionCard;
class ConsumableItemCard;

class ConsumablesTab : public QWidget {
    Q_OBJECT
public:
    explicit ConsumablesTab(const Consumables &consumables,
                            const QList<ReagentCard> &cards,
                            QWidget *parent = nullptr);
    ~ConsumablesTab();

    void setConsumables(const Consumables &consumables);
    void setCards(const QList<ReagentCard> &cards);

private slots:
    // Close all other panels when one card requests to open its panel
    void onExclusivePanelOpen();

private:
    Ui::ConsumablesTab  *ui;
    Consumables          m_consumables;
    QList<ReagentCard>   m_cards;

    DilutionCard        *m_dilCards[3];
    ConsumableItemCard  *m_tipsCard;
    ConsumableItemCard  *m_plateCard;

    void buildDynamicWidgets();
    void styleStaticWidgets();
    void refreshCalibTable();
    void refreshDilutions();
    void refreshConsumables();
};
