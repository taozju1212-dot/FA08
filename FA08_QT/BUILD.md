# FA08Qt — 编译运行指南

## 环境要求

| 项目 | 版本 |
|------|------|
| Qt   | 5.14.2（含 QtSvg 模块） |
| 编译器 | MinGW 7.3+ 或 MSVC 2017+ |
| 构建工具 | qmake（Qt 5.14.2 自带） |

---

## 编译步骤

### Windows (MinGW)

```bat
cd E:\Agent\Code\FA08\FA08Qt

:: 生成 Makefile
qmake FA08Qt.pro -spec win32-g++ CONFIG+=release

:: 编译（4线程）
mingw32-make -j4

:: 运行
release\FA08Qt.exe
```

### Windows (MSVC)

```bat
cd E:\Agent\Code\FA08\FA08Qt

qmake FA08Qt.pro -spec win32-msvc CONFIG+=release
nmake

release\FA08Qt.exe
```

### Qt Creator（推荐）

1. 打开 Qt Creator
2. 文件 → 打开项目 → 选择 `FA08Qt.pro`
3. 选择 Qt 5.14.2 套件（Kit）
4. 点击 **构建** → **运行**

---

## 工程结构说明

```
FA08Qt/
├── FA08Qt.pro               # qmake 项目文件
├── resources/
│   ├── resources.qrc        # Qt 资源索引
│   ├── styles/app.qss       # 全局 QSS 样式表
│   └── icons/*.svg          # 28 个矢量图标
└── src/
    ├── main.cpp
    ├── data/AppData.h       # 数据结构 + 种子数据（纯头文件）
    ├── ui/
    │   ├── LoginWindow      # 登录界面
    │   ├── MainWindow       # 主窗口（状态中枢 + 硬件接口）
    │   ├── widgets/
    │   │   ├── SvgIcon      # SVG 图标渲染（QSvgRenderer）
    │   │   ├── StatusPill   # 状态胶囊组件
    │   │   ├── CircularProgress  # 稀释液圆形进度
    │   │   ├── VirtualKeyboard   # 屏幕软键盘
    │   │   ├── TopBar       # 顶栏（Tab + 状态条）
    │   │   └── BottomBar    # 底栏（耗材 + 控制）
    │   └── tabs/
    │       ├── DetectTab    # 检测页（三栏）
    │       ├── SampleSlotGrid    # 8槽位网格
    │       ├── SampleListPanel   # 样本列表
    │       ├── SampleFormPanel   # 样本申请表单
    │       ├── ResultsTab   # 结果页
    │       ├── ConsumablesTab    # 耗材页
    │       └── SettingsTab  # 设置页（占位）
    └── modals/
        ├── AddCardModal     # 加载试剂卡弹窗
        ├── AlertsModal      # 报警列表弹窗
        └── AssayConfigModal # 检测项目参数配置
```

---

## 默认凭证

| 用户名 | 密码   |
|--------|--------|
| admin  | 123456 |

---

## 硬件接口接入

在 `MainWindow` 中已预留以下 `public slots`，接驳底层硬件驱动时直接 `connect` 即可：

```cpp
void onTemperatureUpdate(double celsius);        // 温度传感器
void onDeviceStatusChanged(DeviceStatus status); // 仪器状态
void onSampleSlotStatusChanged(int idx, SampleStatus); // 运动控制反馈
void onBarcodeScanned(const QString &barcode);   // 条码扫描器
void onLisConnected(bool connected);             // LIS 通讯
```

示例（在你的硬件驱动层）：
```cpp
connect(myTempSensor, &TempSensor::valueChanged,
        mainWindow,   &MainWindow::onTemperatureUpdate);

connect(myScanner,    &BarcodeScanner::scanned,
        mainWindow,   &MainWindow::onBarcodeScanned);
```

---

## 功能验证清单

- [ ] 启动后显示 800×600 蓝色渐变登录界面
- [ ] 输入 `admin / 123456` 登录，进入主界面
- [ ] 顶栏时钟每秒刷新，温度显示 37.x°C
- [ ] 切换 4 个 Tab（检测/结果/耗材/设置）
- [ ] 检测 Tab：点击空槽位 → 右侧弹出表单 → 选择项目 → 提交 → 槽位变色
- [ ] 底栏"启动检测" → 运行条纹动画 + 顶栏"运行中" → 样本依次完成
- [ ] 报警铃铛显示红色数字徽章 → 点击弹出弹窗 → 一键已处理
- [ ] 耗材 Tab：圆形进度颜色随剩余量变化（绿/橙/红/虚线未装载）
- [ ] 底栏"加 卡"按钮 → 弹窗 → 选择项目 → 5秒进度条 → 库存+1
