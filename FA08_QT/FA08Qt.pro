QT += core gui widgets svg

TARGET   = FA08Qt
TEMPLATE = app
CONFIG  += c++14

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/ui/LoginWindow.cpp \
    src/ui/MainWindow.cpp \
    src/ui/widgets/SvgIcon.cpp \
    src/ui/widgets/StatusPill.cpp \
    src/ui/widgets/CircularProgress.cpp \
    src/ui/widgets/VirtualKeyboard.cpp \
    src/ui/widgets/TopBar.cpp \
    src/ui/widgets/BottomBar.cpp \
    src/ui/widgets/SlotCard.cpp \
    src/ui/widgets/DilutionCard.cpp \
    src/ui/widgets/ConsumableItemCard.cpp \
    src/ui/tabs/DetectTab.cpp \
    src/ui/tabs/SampleSlotGrid.cpp \
    src/ui/tabs/SampleListPanel.cpp \
    src/ui/tabs/SampleFormPanel.cpp \
    src/ui/tabs/ResultsTab.cpp \
    src/ui/tabs/ConsumablesTab.cpp \
    src/ui/tabs/SettingsTab.cpp \
    src/modals/AddCardModal.cpp \
    src/modals/AlertsModal.cpp \
    src/modals/AssayConfigModal.cpp

HEADERS += \
    src/data/AppData.h \
    src/ui/LoginWindow.h \
    src/ui/MainWindow.h \
    src/ui/widgets/SvgIcon.h \
    src/ui/widgets/StatusPill.h \
    src/ui/widgets/CircularProgress.h \
    src/ui/widgets/VirtualKeyboard.h \
    src/ui/widgets/TopBar.h \
    src/ui/widgets/BottomBar.h \
    src/ui/widgets/SlotCard.h \
    src/ui/widgets/DilutionCard.h \
    src/ui/widgets/ConsumableItemCard.h \
    src/ui/tabs/DetectTab.h \
    src/ui/tabs/SampleSlotGrid.h \
    src/ui/tabs/SampleListPanel.h \
    src/ui/tabs/SampleFormPanel.h \
    src/ui/tabs/ResultsTab.h \
    src/ui/tabs/ConsumablesTab.h \
    src/ui/tabs/SettingsTab.h \
    src/modals/AddCardModal.h \
    src/modals/AlertsModal.h \
    src/modals/AssayConfigModal.h

FORMS += \
    src/ui/tabs/SampleSlotGrid.ui \
    src/ui/tabs/ConsumablesTab.ui

RESOURCES += resources/resources.qrc
