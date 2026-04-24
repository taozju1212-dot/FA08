#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &username);

private slots:
    void onLogin();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLineEdit   *m_userEdit;
    QLineEdit   *m_pwdEdit;
    QPushButton *m_loginBtn;
    QLabel      *m_errLabel;
    QCheckBox   *m_rememberCk;
    QCheckBox   *m_autoCk;
};
