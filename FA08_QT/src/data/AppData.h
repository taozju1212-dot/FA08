#pragma once
#include <QString>
#include <QList>
#include <QDateTime>
#include <QMap>
#include <QStringList>

// ─────────────────────────── Enums ───────────────────────────

enum class SampleStatus { Empty, Scheduled, Running, Done, Error };
enum class DeviceStatus  { Idle, Running, Paused, Error };
enum class AlertLevel    { Info, Warn, Error };

// ─────────────────────────── Structures ───────────────────────────

struct Assay {
    QString code;
    QString name;
    QString unit;
    QString ref;
    QString dilution;
};

struct AssayCfg {
    int     repeat   = 1;
    double  dilution = 1.0;
};

struct SampleSlot {
    QString       id;
    SampleStatus  status   = SampleStatus::Empty;
    QString       sampleId;
    QString       name;
    QString       gender   = "男";
    int           age      = 0;
    QString       ageUnit  = "岁";
    QString       type     = "血清";
    QStringList   assays;
    QMap<QString, AssayCfg> assayCfg;
};

struct DilutionSolution {
    QString slot;
    QString name;
    int     qty      = 0;
    int     capacity = 50;
    QString lotNo;
    QString expire;
    bool    loaded   = true;
};

struct Consumables {
    struct Tips  { QString name="吸头";   QString slot="C2"; int qty=52; int capacity=80; QString lotNo="TIP-2603-A"; QString expire="2026-11"; } tips;
    struct Plate { QString name="稀释板"; QString slot="C1"; int qty=36; int capacity=80; QString lotNo="PLT-2603-B"; QString expire="2026-09"; } plate;
    QList<DilutionSolution> dilutions;
};

struct ReagentCard {
    QString code;
    QString lot;
    QString expire;
    int     remaining = 0;
};

struct Alert {
    int         id;
    AlertLevel  level;
    QString     time;
    QString     code;
    QString     msg;
    bool        handled = false;
};

struct ResultItem {
    QString code;
    double  value;
    QString unit;
    QString ref;
    QString concl;
};

struct TestResult {
    QString          id;
    QString          sampleId;
    QString          name;
    QString          gender;
    int              age;
    QString          type;
    QString          slot;
    QString          time;
    QList<ResultItem> items;
};

// ─────────────────────────── Static Data ───────────────────────────

namespace AppData {

inline QList<Assay> assays() {
    return {
        { "TnI",     "肌钙蛋白I",   "ng/mL", "0 ~ 0.04", "D1" },
        { "Myo",     "肌红蛋白",    "ng/mL", "0 ~ 72",   "D1" },
        { "CKMB",    "肌酸激酶MB",  "ng/mL", "0 ~ 5.0",  "D1" },
        { "NTproBNP","NT-proBNP",   "pg/mL", "0 ~ 125",  "D2" },
        { "Ddimer",  "D-二聚体",    "mg/L",  "0 ~ 0.5",  "D2" },
        { "CRP",     "C反应蛋白",   "mg/L",  "0 ~ 10",   "D3" },
    };
}

inline QStringList sampleTypes() { return { "全血","血清","血浆","尿液","质控品" }; }
inline QStringList genders()     { return { "男","女" }; }
inline QStringList ageUnits()    { return { "岁","月","天" }; }

inline QList<SampleSlot> initialSlots() {
    QList<SampleSlot> s;

    SampleSlot s1; s1.id="S1"; s1.status=SampleStatus::Scheduled;
    s1.sampleId="20260418001"; s1.name="张伟"; s1.gender="男"; s1.age=56; s1.ageUnit="岁"; s1.type="血清"; s1.assays<<"TnI"<<"Myo"<<"CKMB";
    s.append(s1);

    SampleSlot s2; s2.id="S2"; s2.status=SampleStatus::Scheduled;
    s2.sampleId="20260418002"; s2.name="李娜"; s2.gender="女"; s2.age=42; s2.ageUnit="岁"; s2.type="血清"; s2.assays<<"NTproBNP";
    s.append(s2);

    SampleSlot s3; s3.id="S3"; s3.status=SampleStatus::Done;
    s3.sampleId="20260418003"; s3.name="王强"; s3.gender="男"; s3.age=63; s3.ageUnit="岁"; s3.type="血浆"; s3.assays<<"Ddimer"<<"CRP";
    s.append(s3);

    SampleSlot s4; s4.id="S4"; s4.status=SampleStatus::Empty; s.append(s4);

    SampleSlot s5; s5.id="S5"; s5.status=SampleStatus::Scheduled;
    s5.sampleId="20260418004"; s5.name="陈敏"; s5.gender="女"; s5.age=29; s5.ageUnit="岁"; s5.type="血清"; s5.assays<<"CRP";
    s.append(s5);

    SampleSlot s6; s6.id="S6"; s6.status=SampleStatus::Empty; s.append(s6);

    SampleSlot s7; s7.id="S7"; s7.status=SampleStatus::Error;
    s7.sampleId="20260418005"; s7.name="赵磊"; s7.gender="男"; s7.age=71; s7.ageUnit="岁"; s7.type="全血"; s7.assays<<"CRP";
    s.append(s7);

    SampleSlot s8; s8.id="S8"; s8.status=SampleStatus::Empty; s.append(s8);

    return s;
}

inline Consumables initialConsumables() {
    Consumables c;
    c.dilutions = {
        { "D1", "SAA",   18, 50, "SAA-2602-11", "2026-08", true  },
        { "D2", "PCT",    8, 50, "PCT-2602-03", "2026-07", true  },
        { "D3", "hsCRP",  0, 50, "—",           "—",       false },
    };
    return c;
}

inline QList<ReagentCard> initialCards() {
    return {
        { "TnI",     "A2603-01", "2026-09", 22 },
        { "Myo",     "A2603-02", "2026-09", 18 },
        { "CKMB",    "A2603-05", "2026-10", 14 },
        { "NTproBNP","B2602-11", "2026-08",  9 },
        { "Ddimer",  "B2602-12", "2026-08", 11 },
        { "CRP",     "C2603-20", "2026-11", 30 },
        { "PCT",     "C2603-21", "2026-11",  7 },
        { "hsCRP",   "C2603-22", "2026-11", 16 },
    };
}

inline QList<Alert> initialAlerts() {
    return {
        { 1, AlertLevel::Warn,  "2026-04-18 08:42", "W-0312", "稀释液D2余量低 (< 10)", false },
        { 2, AlertLevel::Info,  "2026-04-18 08:46", "I-0015", "系统自检通过",           true  },
    };
}

inline QList<TestResult> seedResults() {
    QList<TestResult> r;

    TestResult r1; r1.id="R001"; r1.sampleId="20260418100"; r1.name="刘洋"; r1.gender="男"; r1.age=58; r1.type="血清"; r1.slot="S1"; r1.time="2026-04-18 09:12:34";
    r1.items = { {"TnI",0.18,"ng/mL","0 ~ 0.04","阳性"}, {"Myo",112,"ng/mL","0 ~ 72","阳性"}, {"CKMB",8.6,"ng/mL","0 ~ 5.0","阳性"} };
    r.append(r1);

    TestResult r2; r2.id="R002"; r2.sampleId="20260418101"; r2.name="孙悦"; r2.gender="女"; r2.age=34; r2.type="血清"; r2.slot="S2"; r2.time="2026-04-18 09:38:07";
    r2.items = { {"NTproBNP",78,"pg/mL","0 ~ 125","阴性"} };
    r.append(r2);

    TestResult r3; r3.id="R003"; r3.sampleId="20260418102"; r3.name="周浩"; r3.gender="男"; r3.age=67; r3.type="血浆"; r3.slot="S3"; r3.time="2026-04-18 10:02:51";
    r3.items = { {"Ddimer",1.8,"mg/L","0 ~ 0.5","阳性"}, {"CRP",42,"mg/L","0 ~ 10","阳性"} };
    r.append(r3);

    TestResult r4; r4.id="R004"; r4.sampleId="20260418103"; r4.name="钱静"; r4.gender="女"; r4.age=45; r4.type="血清"; r4.slot="S5"; r4.time="2026-04-18 10:41:22";
    r4.items = { {"hsCRP",2.1,"mg/L","0 ~ 3.0","阴性"} };
    r.append(r4);

    TestResult r5; r5.id="R005"; r5.sampleId="20260418104"; r5.name="吴迪"; r5.gender="男"; r5.age=52; r5.type="血清"; r5.slot="S6"; r5.time="2026-04-18 11:15:09";
    r5.items = { {"PCT",0.36,"ng/mL","0 ~ 0.05","阳性"}, {"CRP",68,"mg/L","0 ~ 10","阳性"} };
    r.append(r5);

    return r;
}

// Status display info
struct StatusInfo { QString label; QString cssClass; };
inline StatusInfo statusInfo(SampleStatus s) {
    switch(s) {
        case SampleStatus::Empty:     return {"空位",   "idle"};
        case SampleStatus::Scheduled: return {"已编排", "run"};
        case SampleStatus::Running:   return {"检测中", "run"};
        case SampleStatus::Done:      return {"已完成", "ok"};
        case SampleStatus::Error:     return {"异常",   "err"};
    }
    return {"未知","idle"};
}

} // namespace AppData
