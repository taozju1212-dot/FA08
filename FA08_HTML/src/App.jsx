// Root App — wires everything together
const { useState, useEffect, useRef, useMemo } = React;

function App() {
  // Auth
  const [user, setUser] = useState(null);

  // Navigation
  const [tab, setTab] = useState('detect');

  // Detect state
  const [slots, setSlots] = useState(INITIAL_SLOTS);
  const [activeSlotId, setActiveSlotId] = useState(null);
  const [selected, setSelected] = useState([]);
  const [view, setView] = useState('list'); // 'list' | 'form'
  const [editing, setEditing] = useState(null);

  // Keyboard
  const [kb, setKb] = useState(null); // {target, mode, value}

  // Device
  const [device, setDevice] = useState('idle'); // idle | running | paused | error
  const [temp, setTemp] = useState(37.0);
  const [tickerMsg, setTickerMsg] = useState('系统就绪，请编排样本并启动检测');

  // Consumables / cards
  const [consumables, setConsumables] = useState(INITIAL_CONSUMABLES);
  const [cards, setCards] = useState(INITIAL_CARDS);
  const [loadedCards, setLoadedCards] = useState({}); // code -> qty loaded

  // Alerts
  const [alerts, setAlerts] = useState(INITIAL_ALERTS);

  // Modals
  const [modal, setModal] = useState(null); // 'addCard' | 'alerts' | 'replace:tips' | 'replace:plate' | 'dilution:idx' | null

  // Results
  const [results] = useState(() => seedResults());

  // Toast
  const [toast, setToast] = useState(null);
  const showToast = (msg, type='info') => {
    setToast({ msg, type });
    setTimeout(() => setToast(null), 2200);
  };

  // Temperature wiggle
  useEffect(() => {
    const id = setInterval(() => {
      setTemp(t => 37 + (Math.random()-.5)*.4);
    }, 2000);
    return () => clearInterval(id);
  }, []);

  // Running simulation — advance samples through states
  useEffect(() => {
    if (device !== 'running') return;
    const id = setInterval(() => {
      setSlots(prev => {
        const next = [...prev];
        // any running -> 80% chance done
        const runningIdx = next.findIndex(s => s.status === 'running');
        if (runningIdx >= 0 && Math.random() > 0.3) {
          next[runningIdx] = { ...next[runningIdx], status: 'done' };
          setTickerMsg(`样本 ${next[runningIdx].id} (${next[runningIdx].name}) 检测完成`);
          return next;
        }
        // pick a scheduled to start
        const schedIdx = next.findIndex(s => s.status === 'scheduled');
        if (schedIdx >= 0) {
          next[schedIdx] = { ...next[schedIdx], status: 'running' };
          setTickerMsg(`正在检测样本 ${next[schedIdx].id} (${next[schedIdx].name})...`);
          return next;
        }
        // all done -> stop
        if (!next.some(s => s.status === 'scheduled' || s.status === 'running')) {
          setDevice('idle');
          setTickerMsg('所有样本检测完成');
          showToast('所有样本检测完成', 'ok');
        }
        return next;
      });
    }, 2200);
    return () => clearInterval(id);
  }, [device]);

  // Pending cards calculation
  const neededByCode = useMemo(() => {
    const need = {};
    slots.forEach(s => {
      if (s.status === 'scheduled') {
        s.assays?.forEach(a => {
          need[a] = (need[a] || 0) + 1;
        });
      }
    });
    // subtract loaded
    Object.keys(loadedCards).forEach(c => {
      if (need[c]) need[c] = Math.max(0, need[c] - loadedCards[c]);
    });
    return need;
  }, [slots, loadedCards]);
  const pendingCardCount = Object.values(neededByCode).reduce((a,b)=>a+b,0);

  const scheduledCount = slots.filter(s => s.status==='scheduled').length;
  const runningCount   = slots.filter(s => s.status==='running').length;

  // Handlers
  const handleSelectSlot = (id) => {
    const slot = slots.find(s => s.id === id);
    setActiveSlotId(id);
    if (slot.status === 'empty') {
      setEditing(null);
      setView('form');
    } else {
      setView('list');
    }
  };

  const handleSubmitForm = (form) => {
    setSlots(prev => prev.map(s => s.id === activeSlotId ? {
      ...s,
      status: 'scheduled',
      sampleId: form.sampleId, name: form.name, gender: form.gender,
      age: form.age, ageUnit: form.ageUnit, type: form.type, assays: form.assays,
    } : s));
    setView('list');
    setKb(null);
    showToast(`样本 ${activeSlotId} 已编排`, 'ok');
  };

  const handleCancelForm = () => { setView('list'); setKb(null); };

  const handleToggleSelect = (id) => setSelected(s => s.includes(id) ? s.filter(x=>x!==id) : [...s, id]);
  const handleSelectAll = (on) => {
    if (on) setSelected(slots.filter(s => s.status !== 'empty').map(s => s.id));
    else setSelected([]);
  };

  const handleDelete = () => {
    setSlots(prev => prev.map(s => selected.includes(s.id)
      ? { id: s.id, status:'empty' } : s));
    showToast(`已删除 ${selected.length} 个样本`, 'ok');
    setSelected([]);
  };

  const handleEdit = () => {
    if (selected.length !== 1) { showToast('请选择一个样本编辑', 'warn'); return; }
    const slot = slots.find(s => s.id === selected[0]);
    setActiveSlotId(slot.id);
    setEditing(slot);
    setView('form');
  };

  const handleNew = () => {
    const empty = slots.find(s => s.status === 'empty');
    if (!empty) { showToast('所有样本位已满', 'warn'); return; }
    setActiveSlotId(empty.id);
    setEditing(null);
    setView('form');
  };

  // Keyboard input handling
  const onKeyboardInput = (ch) => {
    if (!kb) return;
    if (ch === '__close' || ch === '__enter') { setKb(null); return; }
    if (ch === '__clear') {
      setKb(k => ({...k, value: ''}));
      window.__sampleFormSetValue?.(kb.target, kb.target==='age' ? '' : '');
      return;
    }
    if (ch === '__back') {
      setKb(k => {
        const v = (k.value || '').slice(0, -1);
        window.__sampleFormSetValue?.(k.target, k.target==='age' ? (parseInt(v)||'') : v);
        return {...k, value: v};
      });
      return;
    }
    setKb(k => {
      const v = (k.value || '') + ch;
      window.__sampleFormSetValue?.(k.target, k.target==='age' ? (parseInt(v)||v) : v);
      return {...k, value: v};
    });
  };

  const openKb = (target, value) => {
    const mode = (target === 'age' || target === 'sampleId') ? 'num' : 'abc';
    setKb({ target, mode, value: String(value || '') });
  };

  // Device
  const handleStart = () => {
    if (pendingCardCount > 0) {
      showToast(`仍需加载 ${pendingCardCount} 张试剂卡`, 'warn');
      setModal('addCard');
      return;
    }
    if (scheduledCount === 0) { showToast('无待检测样本', 'warn'); return; }
    setDevice('running');
    setTickerMsg('启动中... 开始样本检测流程');
    showToast('设备已启动', 'ok');
  };
  const handlePause = () => {
    setDevice('paused');
    setTickerMsg('设备已暂停');
  };

  // Add card
  const handleConfirmCards = (qtys) => {
    setLoadedCards(prev => {
      const next = {...prev};
      Object.keys(qtys).forEach(c => { next[c] = (next[c]||0) + qtys[c]; });
      return next;
    });
    setCards(prev => prev.map(c => qtys[c.code] ? {...c, remaining: c.remaining - qtys[c.code]} : c));
    setModal(null);
    showToast(`已加载 ${Object.values(qtys).reduce((a,b)=>a+b,0)} 张试剂卡`, 'ok');
  };

  // Alerts
  const handleAlertHandle = (id) => {
    setAlerts(prev => prev.map(a => a.id===id ? {...a, handled:true} : a));
  };
  const handleResetSystem = () => {
    setDevice('idle');
    setAlerts(prev => prev.map(a => ({...a, handled:true})));
    showToast('系统已复位', 'ok');
  };
  const handleEstop = () => {
    setDevice('idle');
    showToast('急停已触发', 'err');
  };
  const handleClearAll = () => {
    setAlerts(prev => prev.filter(a => !a.handled));
  };

  // Consumable replace
  const handleReplace = (type) => setModal('replace:'+type);
  const handleConfirmReplace = (qty) => {
    const type = modal.split(':')[1];
    setConsumables(prev => ({
      ...prev,
      [type]: { ...prev[type], qty, lotNo: 'NEW-'+Date.now().toString().slice(-6) },
    }));
    setModal(null);
    showToast(`${consumables[type].name}已更换`, 'ok');
  };

  // Dilution
  const handleLoadDilution = (idx, action) => {
    setConsumables(prev => {
      const next = {...prev, dilutions: [...prev.dilutions]};
      if (action === 'load') {
        next.dilutions[idx] = {...next.dilutions[idx], qty: next.dilutions[idx].capacity};
      } else {
        next.dilutions[idx] = {...next.dilutions[idx], qty: 0};
      }
      return next;
    });
    showToast(action==='load' ? '稀释液已装载' : '稀释液已卸载', 'ok');
  };

  if (!user) return <Login onLogin={u => setUser(u.user)} />;

  return (
    <div style={{height:'100%', display:'flex', flexDirection:'column'}}>
      <TopBar
        tab={tab} onTab={setTab}
        device={device} temp={temp} message={tickerMsg}
        alerts={alerts} onAlerts={() => setModal('alerts')}
        user={user}
      />

      {tab === 'detect' && (
        <DetectPage
          slots={slots}
          activeSlotId={activeSlotId}
          onSelectSlot={handleSelectSlot}
          selected={selected}
          onToggleSelect={handleToggleSelect}
          onSelectAll={handleSelectAll}
          view={view} setView={setView}
          onSubmitForm={handleSubmitForm}
          onCancelForm={handleCancelForm}
          onEdit={handleEdit}
          onDelete={handleDelete}
          onNew={handleNew}
          onKeyboard={openKb}
          keyboardTarget={kb?.target}
          editing={editing}
          nextSeq={editing ? slots.filter(s => s.status !== 'empty').findIndex(s => s.id === editing.id) + 1 : slots.filter(s => s.status !== 'empty').length + 1}
        />
      )}

      {tab === 'results' && <ResultsPage results={results}/>}

      {tab === 'consumables' && (
        <ConsumablesPage
          consumables={consumables}
          cards={cards}
          onReplace={handleReplace}
          onLoadDilution={handleLoadDilution}
        />
      )}

      <BottomBar
        consumables={consumables}
        device={device}
        onStart={handleStart}
        onPause={handlePause}
        scheduledCount={scheduledCount}
        runningCount={runningCount}
        pendingCardCount={pendingCardCount}
        onAddCard={() => { if(pendingCardCount>0) setModal('addCard'); else showToast('无需加卡','info'); }}
        onOpenConsumable={(k) => setModal('replace:'+k)}
        onOpenDilutions={(i) => { setTab('consumables'); }}
        message={tickerMsg}
      />

      {kb && (
        <Keyboard
          mode={kb.mode}
          target={kb.target}
          value={kb.value}
          onInput={onKeyboardInput}
          onClose={() => setKb(null)}
        />
      )}

      {modal === 'addCard' && (
        <AddCardModal
          cards={cards}
          neededByCode={Object.fromEntries(Object.entries(neededByCode).filter(([,v])=>v>0))}
          onClose={() => setModal(null)}
          onConfirm={handleConfirmCards}
        />
      )}

      {modal === 'alerts' && (
        <AlertsModal
          alerts={alerts}
          onClose={() => setModal(null)}
          onHandle={handleAlertHandle}
          onResetSystem={handleResetSystem}
          onEstop={handleEstop}
          onClearAll={handleClearAll}
        />
      )}

      {modal && modal.startsWith('replace:') && (
        <ReplaceModal
          type={modal.split(':')[1]}
          consumables={consumables}
          onClose={() => setModal(null)}
          onConfirm={handleConfirmReplace}
        />
      )}

      {toast && <Toast msg={toast.msg} type={toast.type}/>}
    </div>
  );
}

ReactDOM.createRoot(document.getElementById('stage')).render(<App/>);
