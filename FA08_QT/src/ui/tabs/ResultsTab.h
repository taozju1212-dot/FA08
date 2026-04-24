#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QList>
#include "data/AppData.h"

class ResultsTab : public QWidget {
    Q_OBJECT
public:
    explicit ResultsTab(const QList<TestResult> &results, QWidget *parent = nullptr);
    void setResults(const QList<TestResult> &results);

private:
    QTableWidget      *m_table;
    QList<TestResult>  m_results;
    void refreshTable();
};
