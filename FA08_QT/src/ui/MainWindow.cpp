#include "MainWindow.h"
#include "widgets/TopBar.h"
#include "widgets/BottomBar.h"
#include "tabs/DetectTab.h"
#include "tabs/ResultsTab.h"
#include "tabs/ConsumablesTab.h"
#include "tabs/SettingsTab.h"
#include "modals/AddCardModal.h"
#include "modals/AlertsModal.h"
#include <QVBoxLayout>
#include <QFile>
#include <QApplication>
#include <QScreen>
#include <QRandomGenerator>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(800, 600);
    setWindowTitle("FA08 全自动荧光免疫分析仪");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // Seed initial data
    m_slots       = AppData::initialSlots();
    m_consumables = AppData::initialConsumables();
    m_cards       = AppData::initialCards();
    m_alerts      = AppData::initialAlerts();
    m_results     = AppData::seedResults();

    loadStyleSheet();
    setupUI();
    refreshAll();

    // Center on screen
    if (QScreen *scr = QApplication::primaryScreen()) {
        QRect sg = scr->geometry();
        move(sg.center() - rect().center());
    }

    // Simulation timer (1 second tick)
    m_simTimer.setInterval(1000);
    connect(&m_simTimer, &QTimer::timeout, this, &MainWindow::simulateTick);
}

void MainWindow::loadStyleSheet()
{
    QFile f(":/styles/app.qss");
    if (f.open(QIODevice::ReadOnly)) {
        qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
    }
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *vl = new QVBoxLayout(central);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // Top bar
    m_topBar = new TopBar(central);
    connect(m_topBar, &TopBar::tabChanged,       this, &MainWindow::onTabChanged);
    connect(m_topBar, &TopBar::alertsBellClicked, this, &MainWindow::onAlertsBell);
    vl->addWidget(m_topBar);

    // Content stack
    m_stack = new QStackedWidget(central);
    m_detectTab      = new DetectTab(m_slots, m_cards, this);
    m_resultsTab     = new ResultsTab(m_results, this);
    m_consumablesTab = new ConsumablesTab(m_consumables, m_cards, this);
    m_settingsTab    = new SettingsTab(this);

    m_stack->addWidget(m_detectTab);      // index 0
    m_stack->addWidget(m_resultsTab);     // index 1
    m_stack->addWidget(m_consumablesTab); // index 2
    m_stack->addWidget(m_settingsTab);    // index 3
    vl->addWidget(m_stack, 1);

    // Bottom bar
    m_bottomBar = new BottomBar(central);
    connect(m_bottomBar, &BottomBar::startClicked,   this, &MainWindow::onStart);
    connect(m_bottomBar, &BottomBar::pauseClicked,   this, &MainWindow::onPause);
    connect(m_bottomBar, &BottomBar::addCardClicked, this, &MainWindow::onAddCard);
    vl->addWidget(m_bottomBar);

    // Connect DetectTab signals
    connect(m_detectTab, &DetectTab::slotsChanged, this, [this](const QList<SampleSlot> &slotsList) {
        m_slots = slotsList;
        syncBottomBar();
        m_resultsTab->setResults(m_results);
    });
}

int MainWindow::scheduledCount() const
{
    int n = 0;
    for (const auto &s : m_slots)
        if (s.status == SampleStatus::Scheduled) n++;
    return n;
}

int MainWindow::pendingCardCount() const
{
    // Count needed cards based on scheduled assays vs inventory
    QMap<QString, int> needed;
    for (const auto &s : m_slots) {
        if (s.status == SampleStatus::Scheduled || s.status == SampleStatus::Running) {
            for (const QString &code : s.assays)
                needed[code]++;
        }
    }
    // Compare against cards
    int total = 0;
    for (auto it = needed.begin(); it != needed.end(); ++it) {
        bool found = false;
        for (const auto &c : m_cards) {
            if (c.code == it.key()) {
                int deficit = it.value() - c.remaining;
                if (deficit > 0) total += deficit;
                found = true;
                break;
            }
        }
        if (!found) total += it.value();
    }
    return total;
}

void MainWindow::refreshAll()
{
    syncTopBar();
    syncBottomBar();
    m_detectTab->setSlots(m_slots);
    m_resultsTab->setResults(m_results);
    m_consumablesTab->setConsumables(m_consumables);
    m_consumablesTab->setCards(m_cards);
}

void MainWindow::syncTopBar()
{
    m_topBar->setDeviceStatus(m_deviceStatus);
    m_topBar->setTemperature(m_temperature);
    int unhandled = 0;
    for (const auto &a : m_alerts) if (!a.handled) unhandled++;
    m_topBar->setUnhandledAlerts(unhandled);
}

void MainWindow::syncBottomBar()
{
    m_bottomBar->setConsumables(m_consumables);
    m_bottomBar->setDeviceStatus(m_deviceStatus);
    m_bottomBar->setScheduledCount(scheduledCount());
    m_bottomBar->setPendingCardCount(pendingCardCount());
}

// ── Tab switching ──
void MainWindow::onTabChanged(int index)
{
    m_topBar->setActiveTab(index);
    m_stack->setCurrentIndex(index);
}

// ── Start / Pause ──
void MainWindow::onStart()
{
    m_deviceStatus = DeviceStatus::Running;
    m_simRunIndex  = 0;
    m_simTimer.start();
    syncTopBar();
    syncBottomBar();
}

void MainWindow::onPause()
{
    if (m_deviceStatus == DeviceStatus::Running) {
        m_deviceStatus = DeviceStatus::Paused;
        m_simTimer.stop();
        syncTopBar();
        syncBottomBar();
    }
}

// ── Simulation tick ──
void MainWindow::simulateTick()
{
    // Temperature jitter ±0.2°C
    double jitter = (QRandomGenerator::global()->bounded(41) - 20) / 100.0;
    m_temperature = 37.0 + jitter;
    m_topBar->setTemperature(m_temperature);

    // Advance one scheduled slot to running, then done
    bool anyRunning = false;
    bool anyScheduled = false;
    for (auto &s : m_slots) {
        if (s.status == SampleStatus::Running) { anyRunning = true; break; }
        if (s.status == SampleStatus::Scheduled) anyScheduled = true;
    }

    if (!anyRunning && anyScheduled) {
        // Start next scheduled slot
        for (auto &s : m_slots) {
            if (s.status == SampleStatus::Scheduled) {
                s.status = SampleStatus::Running;
                break;
            }
        }
    } else if (anyRunning) {
        // Complete current running slot after 3s (just 1 tick per slot for demo)
        static int tickCount = 0;
        tickCount++;
        if (tickCount >= 3) {
            tickCount = 0;
            for (auto &s : m_slots) {
                if (s.status == SampleStatus::Running) {
                    s.status = SampleStatus::Done;
                    // Consume tips and plates
                    m_consumables.tips.qty  = qMax(0, m_consumables.tips.qty - (int)s.assays.size());
                    m_consumables.plate.qty = qMax(0, m_consumables.plate.qty - 1);
                    break;
                }
            }
        }
    } else if (!anyScheduled && !anyRunning) {
        // All done
        m_deviceStatus = DeviceStatus::Idle;
        m_simTimer.stop();
    }

    refreshAll();
}

// ── Add Card ──
void MainWindow::onAddCard()
{
    // Build needed map
    QMap<QString, int> needed;
    for (const auto &s : m_slots) {
        if (s.status == SampleStatus::Scheduled) {
            for (const QString &code : s.assays) needed[code]++;
        }
    }
    AddCardModal *dlg = new AddCardModal(m_cards, needed, this);
    connect(dlg, &AddCardModal::cardsLoaded, this, [this](const QMap<QString,int> &loaded) {
        for (auto it = loaded.begin(); it != loaded.end(); ++it) {
            for (auto &c : m_cards) {
                if (c.code == it.key()) {
                    c.remaining += it.value();
                    break;
                }
            }
        }
        syncBottomBar();
        m_consumablesTab->setCards(m_cards);
    });
    dlg->exec();
}

// ── Alerts bell ──
void MainWindow::onAlertsBell()
{
    AlertsModal *dlg = new AlertsModal(m_alerts, this);
    connect(dlg, &AlertsModal::alertsChanged, this, [this](const QList<Alert> &alerts) {
        m_alerts = alerts;
        syncTopBar();
    });
    dlg->exec();
}

// ── Hardware interface stubs ──
void MainWindow::onTemperatureUpdate(double celsius)
{
    m_temperature = celsius;
    m_topBar->setTemperature(celsius);
}

void MainWindow::onDeviceStatusChanged(DeviceStatus status)
{
    m_deviceStatus = status;
    syncTopBar();
    syncBottomBar();
}

void MainWindow::onSampleSlotStatusChanged(int slotIndex, SampleStatus status)
{
    if (slotIndex >= 0 && slotIndex < m_slots.size()) {
        m_slots[slotIndex].status = status;
        refreshAll();
    }
}

void MainWindow::onBarcodeScanned(const QString &barcode)
{
    // Forward to active dialog if any, or to detect tab
    m_detectTab->onBarcodeScanned(barcode);
}

void MainWindow::onLisConnected(bool /*connected*/)
{
    // Update LIS indicator in TopBar — hook to hardware layer
}
