#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>
#include <QList>
#include "data/AppData.h"

class TopBar;
class BottomBar;
class DetectTab;
class ResultsTab;
class ConsumablesTab;
class SettingsTab;
class AddCardModal;
class AlertsModal;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    // ── Hardware interface slots (connect to real hardware layer) ──
public slots:
    void onTemperatureUpdate(double celsius);
    void onDeviceStatusChanged(DeviceStatus status);
    void onSampleSlotStatusChanged(int slotIndex, SampleStatus status);
    void onBarcodeScanned(const QString &barcode);
    void onLisConnected(bool connected);

private slots:
    void onTabChanged(int index);
    void onStart();
    void onPause();
    void onAddCard();
    void onAlertsBell();
    void simulateTick();       // temperature jitter + run simulation

private:
    TopBar          *m_topBar;
    BottomBar       *m_bottomBar;
    QStackedWidget  *m_stack;
    DetectTab       *m_detectTab;
    ResultsTab      *m_resultsTab;
    ConsumablesTab  *m_consumablesTab;
    SettingsTab     *m_settingsTab;

    QTimer           m_simTimer;   // 1s tick for sim + temp

    // Application state
    QList<SampleSlot>    m_slots;
    Consumables          m_consumables;
    QList<ReagentCard>   m_cards;
    QList<Alert>         m_alerts;
    QList<TestResult>    m_results;
    DeviceStatus         m_deviceStatus = DeviceStatus::Idle;
    double               m_temperature  = 37.0;
    int                  m_simRunIndex  = 0;  // current slot being simulated

    void loadStyleSheet();
    void setupUI();
    int  scheduledCount() const;
    int  pendingCardCount() const;
    void refreshAll();
    void syncBottomBar();
    void syncTopBar();
};
