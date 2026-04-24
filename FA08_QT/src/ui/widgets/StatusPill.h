#pragma once
#include <QWidget>
#include <QString>

// Renders the .pill component: colored dot + label in a rounded capsule.
// States: "ok", "warn", "err", "run", "idle"
class StatusPill : public QWidget {
    Q_OBJECT
public:
    explicit StatusPill(const QString &state, const QString &label, QWidget *parent = nullptr);
    void setState(const QString &state, const QString &label);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QString m_state;
    QString m_label;

    QColor dotColor() const;
    QColor bgColor() const;
    QColor fgColor() const;
    QColor borderColor() const;
};
