#pragma once
#include <QWidget>
#include <QString>

// Circular progress widget for dilution solution levels.
// Shows a ring arc proportional to qty/capacity, with color coding:
//   unloaded → grey dashed, crit (<12%) → red, warn (<25%) → orange, ok → green
class CircularProgress : public QWidget {
    Q_OBJECT
public:
    explicit CircularProgress(QWidget *parent = nullptr);
    void setData(const QString &name, int qty, int capacity, bool loaded);

    QSize sizeHint() const override { return QSize(46, 46); }
    QSize minimumSizeHint() const override { return QSize(38, 38); }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QString m_name;
    int     m_qty      = 0;
    int     m_capacity = 50;
    bool    m_loaded   = false;

    enum State { Unloaded, Crit, Warn, Ok };
    State   state() const;
    QColor  arcColor() const;
};
