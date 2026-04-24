#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QList>
#include <QLabel>
#include "data/AppData.h"
#include "ui/widgets/VirtualKeyboard.h"

// Right panel: sample application form.
// Shows when a slot is selected; emits submitted(slotIndex, slot) on confirm.
class SampleFormPanel : public QWidget {
    Q_OBJECT
public:
    explicit SampleFormPanel(QWidget *parent = nullptr);

    void loadSlot(int index, const SampleSlot &slot, int nextSeq);
    void clearForm();
    bool hasActiveSlot() const { return m_slotIndex >= 0; }
    int  activeSlotIndex() const { return m_slotIndex; }

signals:
    void submitted(int slotIndex, SampleSlot slot);
    void cancelled();

private slots:
    void onSubmit();
    void onCancel();
    void onKeyboardValue(const QString &field, const QString &value);
    void onKeyboardConfirmed(const QString &field, const QString &value);
    void onFieldClicked(const QString &field);
    void onAssayToggled(const QString &code);
    void onShowAssayConfig();

private:
    int        m_slotIndex = -1;
    SampleSlot m_form;
    int        m_nextSeq   = 1;

    QLabel     *m_slotLabel;
    QLabel     *m_seqLabel;
    QLineEdit  *m_sampleIdEdit;
    QLineEdit  *m_nameEdit;
    QLineEdit  *m_ageEdit;
    QComboBox  *m_ageUnitCombo;
    QComboBox  *m_typeCombo;
    QPushButton *m_genderBtns[2];
    QPushButton *m_typeBtns[5];
    QPushButton *m_assayBtns[6];  // one per assay in ASSAYS
    QPushButton *m_submitBtn;
    QPushButton *m_cancelBtn;

    VirtualKeyboard *m_keyboard;
    QString          m_activeField;

    void buildUI();
    void refreshForm();
    void validateForm();
    void highlightField(QLineEdit *edit, bool active);
    void updateGenderButtons();
    void updateTypeButtons();
    void updateAssayButtons();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
