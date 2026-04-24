#pragma once
#include <QDialog>
#include <QList>
#include <QMap>
#include <QComboBox>
#include <QLineEdit>
#include "data/AppData.h"

// Dialog for configuring repeat count and dilution factor per selected assay.
class AssayConfigModal : public QDialog {
    Q_OBJECT
public:
    explicit AssayConfigModal(const QStringList &selectedAssays,
                              const QMap<QString, AssayCfg> &cfg,
                              QWidget *parent = nullptr);

    QMap<QString, AssayCfg> config() const { return m_cfg; }

private:
    QStringList              m_assays;
    QMap<QString, AssayCfg>  m_cfg;

    struct Row { QComboBox *repeatBox; QLineEdit *dilEdit; };
    QMap<QString, Row> m_rows;
};
