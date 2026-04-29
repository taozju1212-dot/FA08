#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QTableWidget>
#include <QRadioButton>
#include <QCheckBox>
#include <QTabWidget>
#include <QScrollArea>

// ─── Toggle switch ───────────────────────────────────────────────
class ToggleSwitch : public QWidget {
    Q_OBJECT
public:
    explicit ToggleSwitch(bool on = false, QWidget *parent = nullptr);
    bool isChecked() const { return m_on; }
    void setChecked(bool v);
    QSize sizeHint() const override        { return QSize(40, 22); }
    QSize minimumSizeHint() const override { return QSize(40, 22); }
signals:
    void toggled(bool);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
private:
    bool m_on;
};

// ─── Settings tab ────────────────────────────────────────────────
class SettingsTab : public QWidget {
    Q_OBJECT
public:
    explicit SettingsTab(QWidget *parent = nullptr);

private slots:
    void onMenuClicked(int idx);
    void onCommModeChanged(bool isNetwork);
    void onPrinterTypeChanged(bool isInternal);
    void onBrightnessChanged(int val);
    void onUserSelectionChanged();
    void onQcSelectionChanged();

private:
    QLabel         *m_panelTitle      = nullptr;
    QStackedWidget *m_stack           = nullptr;
    QPushButton    *m_menuBtns[8];

    // 系统设置 widgets
    QWidget        *m_networkSection  = nullptr;
    QWidget        *m_serialSection   = nullptr;
    QPushButton    *m_netModeBtn      = nullptr;
    QPushButton    *m_serialModeBtn   = nullptr;
    QLabel         *m_brightLabel     = nullptr;

    // 仪器设置 widgets
    QLabel         *m_incubLabel      = nullptr;
    QLabel         *m_rack01Label     = nullptr;
    QLabel         *m_rack02Label     = nullptr;

    // 打印设置 widgets
    ToggleSwitch   *m_autoPrint            = nullptr;
    QWidget        *m_printInternalSection = nullptr;
    QWidget        *m_printExternalSection = nullptr;
    QPushButton    *m_printerInternalBtn   = nullptr;
    QPushButton    *m_printerExternalBtn   = nullptr;

    // 用户设置 widgets
    QTableWidget   *m_userTable  = nullptr;
    QPushButton    *m_editUserBtn = nullptr;
    QPushButton    *m_delUserBtn  = nullptr;

    // 质控设置 widgets
    QTableWidget   *m_qcTable   = nullptr;
    QPushButton    *m_editQcBtn = nullptr;
    QPushButton    *m_delQcBtn  = nullptr;

    int m_activeMenu = 0;

    void switchMenu(int idx);

    QWidget    *buildSysPanel();
    QWidget    *buildFuncPanel();
    QWidget    *buildQcPanel();
    QWidget    *buildProjPanel();
    QWidget    *buildPrintPanel();
    QWidget    *buildUsersPanel();
    QWidget    *buildAboutPanel();
    QWidget    *buildFactoryPanel();

    // Layout helpers
    static QWidget     *wrapScroll(QWidget *content);
    static QWidget     *makeSecTitle(const QString &text, QWidget *parent);
    static QWidget     *makeRow(const QString &label, int lw, QWidget *ctrl, QWidget *parent, QWidget *extra = nullptr);
    static QPushButton *makeActionBtn(const QString &text, bool primary, QWidget *parent);
    static ToggleSwitch *makeToggle(bool on, QWidget *parent);
    static QString      inputStyle();
    static QString      smallBtnStyle(bool primary = false);
};
