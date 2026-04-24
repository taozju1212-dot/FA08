#include "LoginWindow.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QLinearGradient>
#include <QKeyEvent>
#include <QApplication>
#include <QScreen>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);
    setWindowTitle("FA08 全自动荧光免疫分析仪");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // ── Panel widget (white card) ──
    QWidget *panel = new QWidget(this);
    panel->setObjectName("loginPanel");
    panel->setFixedSize(340, 340);
    panel->setStyleSheet(
        "#loginPanel {"
        "  background: #FFFFFF;"
        "  border-radius: 10px;"
        "}"
    );

    // ── Panel layout ──
    QVBoxLayout *vl = new QVBoxLayout(panel);
    vl->setContentsMargins(32, 28, 32, 24);
    vl->setSpacing(0);

    // Title
    QLabel *title = new QLabel("全自动荧光免疫分析仪", panel);
    title->setAlignment(Qt::AlignCenter);
    QFont tf; tf.setPixelSize(18); tf.setWeight(QFont::DemiBold); tf.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    title->setFont(tf);
    title->setStyleSheet("color:#0F1C2B; margin-bottom:4px;");
    vl->addWidget(title);
    vl->addSpacing(10);

    QLabel *sub = new QLabel("用户登录", panel);
    QFont sf; sf.setPixelSize(14); sf.setWeight(QFont::DemiBold);
    sub->setFont(sf);
    sub->setStyleSheet("color:#0F1C2B; margin-bottom:14px;");
    vl->addWidget(sub);
    vl->addSpacing(14);

    // Username
    QLabel *userLbl = new QLabel("用户名", panel);
    userLbl->setStyleSheet("font-size:11px; color:#5B6B7B; font-weight:500; margin-bottom:2px;");
    vl->addWidget(userLbl);
    m_userEdit = new QLineEdit("admin", panel);
    m_userEdit->setFixedHeight(32);
    m_userEdit->setStyleSheet(
        "QLineEdit { border:1px solid #B7C3CE; border-radius:4px; padding:0 10px;"
        "  font-size:13px; color:#1F2A37; background:#fff; }"
        "QLineEdit:focus { border-color:#1E78BE; }"
    );
    vl->addWidget(m_userEdit);
    vl->addSpacing(10);

    // Password
    QLabel *pwdLbl = new QLabel("密码", panel);
    pwdLbl->setStyleSheet("font-size:11px; color:#5B6B7B; font-weight:500; margin-bottom:2px;");
    vl->addWidget(pwdLbl);
    m_pwdEdit = new QLineEdit("123456", panel);
    m_pwdEdit->setEchoMode(QLineEdit::Password);
    m_pwdEdit->setFixedHeight(32);
    m_pwdEdit->setStyleSheet(
        "QLineEdit { border:1px solid #B7C3CE; border-radius:4px; padding:0 10px;"
        "  font-size:13px; color:#1F2A37; background:#fff; }"
        "QLineEdit:focus { border-color:#1E78BE; }"
    );
    vl->addWidget(m_pwdEdit);
    vl->addSpacing(14);

    // Checkboxes
    QHBoxLayout *ckRow = new QHBoxLayout;
    ckRow->setSpacing(16);
    m_rememberCk = new QCheckBox("记住密码", panel);
    m_rememberCk->setChecked(true);
    m_rememberCk->setStyleSheet("font-size:12px; color:#334155;");
    m_autoCk = new QCheckBox("自动登录", panel);
    m_autoCk->setStyleSheet("font-size:12px; color:#334155;");
    ckRow->addWidget(m_rememberCk);
    ckRow->addWidget(m_autoCk);
    ckRow->addStretch();
    vl->addLayout(ckRow);
    vl->addSpacing(10);

    // Error label (hidden by default)
    m_errLabel = new QLabel(panel);
    m_errLabel->setStyleSheet("color:#C0392B; font-size:12px;");
    m_errLabel->hide();
    vl->addWidget(m_errLabel);

    // Login button
    m_loginBtn = new QPushButton("登 录", panel);
    m_loginBtn->setFixedHeight(44);
    m_loginBtn->setStyleSheet(
        "QPushButton {"
        "  background:#1464A5; color:#fff; border:1px solid #0F4E85;"
        "  border-radius:6px; font-size:14px; font-weight:600;"
        "}"
        "QPushButton:hover { background:#0F4E85; }"
        "QPushButton:pressed { background:#0B3C66; }"
    );
    vl->addWidget(m_loginBtn);

    // Center the panel
    panel->move((800 - 340) / 2, (600 - 340) / 2 - 10);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(m_pwdEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLogin);

    // Center on screen
    if (QScreen *scr = QApplication::primaryScreen()) {
        QRect sg = scr->geometry();
        move(sg.center() - rect().center());
    }
}

void LoginWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Gradient background matching HTML: 135deg, #0B3C66 → #1464A5 → #2E86C1
    QLinearGradient grad(0, 0, width(), height());
    grad.setColorAt(0.0, QColor("#0B3C66"));
    grad.setColorAt(0.6, QColor("#1464A5"));
    grad.setColorAt(1.0, QColor("#2E86C1"));
    p.fillRect(rect(), grad);

    // Decorative grid (white lines, 8% opacity)
    p.setPen(QPen(QColor(255, 255, 255, 20), 0.6));
    int step = 40;
    for (int x = 0; x <= width(); x += step)
        p.drawLine(x, 0, 0, x);          // diagonal from left
    for (int y = 0; y <= height(); y += step)
        p.drawLine(0, y, y, 0);
    for (int x = 0; x <= width() + height(); x += step) {
        p.drawLine(x, 0, 0, x);
    }
}

void LoginWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        onLogin();
    else
        QWidget::keyPressEvent(event);
}

void LoginWindow::onLogin()
{
    QString user = m_userEdit->text().trimmed();
    QString pwd  = m_pwdEdit->text();

    if (user.isEmpty() || pwd.isEmpty()) {
        m_errLabel->setText("请输入用户名和密码");
        m_errLabel->show();
        return;
    }
    // Default credentials: admin / 123456
    if (user != "admin" || pwd != "123456") {
        m_errLabel->setText("用户名或密码错误");
        m_errLabel->show();
        return;
    }

    m_errLabel->hide();
    emit loginSuccess(user);

    MainWindow *mw = new MainWindow();
    mw->show();
    this->close();
}
