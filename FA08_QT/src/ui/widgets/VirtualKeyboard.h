#pragma once
#include <QWidget>
#include <QString>
#include <QList>

// On-screen keyboard supporting numeric and QWERTY modes.
// Floats as a child widget at the bottom of the parent.
// Use show(mode, currentValue) to display, hide() to close.
class VirtualKeyboard : public QWidget {
    Q_OBJECT
public:
    enum Mode { Numeric, QWERTY };
    explicit VirtualKeyboard(QWidget *parent = nullptr);

    void showFor(Mode mode, const QString &currentValue, const QString &fieldKey);
    QString currentValue() const { return m_value; }
    QString fieldKey()     const { return m_fieldKey; }

signals:
    void valueChanged(const QString &fieldKey, const QString &value);
    void confirmed(const QString &fieldKey, const QString &value);
    void closed();

private slots:
    void onKeyPressed(const QString &key);

private:
    Mode    m_mode   = QWERTY;
    QString m_value;
    QString m_fieldKey;
    bool    m_shift  = false;

    QWidget *m_numPad   = nullptr;
    QWidget *m_qwPad    = nullptr;
    QWidget *m_preview  = nullptr;  // shows current value

    void buildNumPad();
    void buildQwertyPad();
    void updatePreview();
    void setShift(bool on);
    QWidget *buildRow(const QStringList &keys, int keyH = 36);
};
