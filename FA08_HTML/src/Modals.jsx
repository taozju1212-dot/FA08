// Modal: Add card dialog
const AddCardModal = ({ cards, neededByCode, onClose, onConfirm }) => {
  const [qtys, setQtys] = React.useState(() => {
    const q = {};
    Object.keys(neededByCode).forEach(code => { q[code] = neededByCode[code]; });
    return q;
  });

  const totalNeeded = Object.values(neededByCode).reduce((a,b)=>a+b, 0);
  const totalAdding = Object.values(qtys).reduce((a,b)=>a+b, 0);

  const change = (code, delta) => {
    setQtys(q => {
      const card = cards.find(c=>c.code===code);
      const maxAllowed = Math.min(neededByCode[code] || 0, card?.remaining || 0);
      const next = Math.max(0, Math.min(maxAllowed, (q[code]||0) + delta));
      return { ...q, [code]: next };
    });
  };

  return (
    <div className="overlay">
      <div className="modal" style={{width: 540}}>
        <div className="modal-hd">
          <div><Icon name="card" size={14}/> 加卡 — 加载试剂卡</div>
          <span className="x" onClick={onClose}>×</span>
        </div>
        <div className="modal-bd">
          <div style={{fontSize:12, color:'var(--ink-600)', marginBottom:10,
            display:'flex', justifyContent:'space-between',
            padding:'8px 10px', background:'var(--blue-050)', borderRadius:4, border:'1px solid var(--blue-100)'}}>
            <span>编排待加卡总数：<b className="mono" style={{color:'var(--blue-700)'}}>{totalNeeded}</b> 张</span>
            <span>本次准备加载：<b className="mono" style={{color:'var(--ok)'}}>{totalAdding}</b> 张</span>
          </div>
          <table className="tbl" style={{border:'1px solid var(--border)'}}>
            <thead><tr>
              <th>项目</th><th>LOT批号</th><th>库存</th><th>需要</th><th style={{width:110}}>加载</th>
            </tr></thead>
            <tbody>
              {Object.keys(neededByCode).map(code => {
                const card = cards.find(c => c.code===code);
                const need = neededByCode[code];
                const qty = qtys[code] || 0;
                const shortage = !card || card.remaining < need;
                return (
                  <tr key={code}>
                    <td style={{fontWeight:700}}>{code}</td>
                    <td className="mono">{card?.lot || '—'}</td>
                    <td className="mono" style={{color: shortage?'var(--err)':'var(--ink-700)'}}>{card?.remaining || 0}</td>
                    <td className="mono" style={{fontWeight:700}}>{need}</td>
                    <td>
                      <div style={{display:'flex', alignItems:'center', gap:4}}>
                        <button className="btn sm" style={{width:26, padding:0}} onClick={()=>change(code,-1)}>−</button>
                        <div className="mono" style={{flex:1, textAlign:'center', fontWeight:700, fontSize:13}}>{qty}</div>
                        <button className="btn sm" style={{width:26, padding:0}} onClick={()=>change(code,1)}>+</button>
                      </div>
                    </td>
                  </tr>
                );
              })}
            </tbody>
          </table>
        </div>
        <div className="modal-ft">
          <button className="btn" onClick={onClose}>取消</button>
          <button className="btn primary" disabled={totalAdding===0} onClick={() => onConfirm(qtys)}>
            <Icon name="check" size={11}/> 确认加载 {totalAdding} 张
          </button>
        </div>
      </div>
    </div>
  );
};

// Alerts modal
const AlertsModal = ({ alerts, onClose, onHandle, onResetSystem, onEstop, onClearAll }) => (
  <div className="overlay">
    <div className="modal" style={{width: 620}}>
      <div className="modal-hd" style={{background: alerts.some(a=>!a.handled) ? '#C0392B' : 'var(--blue-700)'}}>
        <div><Icon name="alert" size={14}/> 报警管理</div>
        <span className="x" onClick={onClose}>×</span>
      </div>
      <div className="modal-bd" style={{padding:0}}>
        {alerts.length === 0 ? (
          <div style={{padding:40, textAlign:'center', color:'var(--ink-400)'}}>
            <Icon name="check" size={28} color="#C5D3DF"/>
            <div style={{marginTop:6, fontSize:12}}>暂无报警</div>
          </div>
        ) : (
          <table className="tbl">
            <thead><tr>
              <th style={{width:64}}>级别</th><th style={{width:130}}>时间</th><th style={{width:80}}>代码</th>
              <th>描述</th><th style={{width:80}}>状态</th><th style={{width:70}}></th>
            </tr></thead>
            <tbody>
              {alerts.map(a => (
                <tr key={a.id}>
                  <td>
                    <span className={'pill '+(a.level==='warn'?'err': a.level==='info'?'run':'warn')} style={{fontSize:10}}>
                      <span className="dot"/>{a.level==='warn'?'警告': a.level==='info'?'信息':'严重'}
                    </span>
                  </td>
                  <td className="mono" style={{fontSize:11}}>{a.time}</td>
                  <td className="mono" style={{fontSize:11}}>{a.code}</td>
                  <td style={{fontSize:12}}>{a.msg}</td>
                  <td>
                    {a.handled
                      ? <span className="pill ok"><span className="dot"/>已处理</span>
                      : <span className="pill err"><span className="dot"/>未处理</span>}
                  </td>
                  <td style={{textAlign:'right'}}>
                    {!a.handled && <button className="btn sm" onClick={()=>onHandle(a.id)}>处理</button>}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        )}
      </div>
      <div className="modal-ft" style={{justifyContent:'space-between'}}>
        <div style={{display:'flex', gap:6}}>
          <button className="btn danger" onClick={onEstop}><Icon name="stop" size={11}/> 急停</button>
          <button className="btn warn" onClick={onResetSystem}><Icon name="reset" size={11}/> 复位</button>
          <button className="btn" onClick={onClearAll}>清除已处理</button>
        </div>
        <button className="btn primary" onClick={onClose}>关闭</button>
      </div>
    </div>
  </div>
);

// Replace consumable modal
const ReplaceModal = ({ type, consumables, onClose, onConfirm }) => {
  const data = consumables[type];
  const [qty, setQty] = React.useState(data.capacity);

  return (
    <div className="overlay">
      <div className="modal" style={{width: 440}}>
        <div className="modal-hd">
          <div><Icon name="refresh" size={14}/> 更换耗材 — {data.name}</div>
          <span className="x" onClick={onClose}>×</span>
        </div>
        <div className="modal-bd">
          <div style={{fontSize:12, color:'var(--ink-600)', marginBottom:12, lineHeight:1.8}}>
            1. 从正面抽出托盘<br/>
            2. 取出旧 {data.name}，放入新的耗材<br/>
            3. 将托盘推回仪器并确认
          </div>
          <div className="field" style={{marginBottom:10}}>
            <label style={{fontSize:11, color:'var(--ink-500)'}}>新批号</label>
            <input className="input" defaultValue={'NEW-'+Date.now().toString().slice(-6)}/>
          </div>
          <div className="field">
            <label style={{fontSize:11, color:'var(--ink-500)'}}>更新库存数量</label>
            <div style={{display:'flex', gap:4, alignItems:'center'}}>
              <button className="btn sm" onClick={()=>setQty(q=>Math.max(0,q-1))}>−</button>
              <input className="input mono" style={{flex:1, textAlign:'center', fontWeight:700}} value={qty} onChange={e=>setQty(parseInt(e.target.value)||0)}/>
              <button className="btn sm" onClick={()=>setQty(q=>Math.min(data.capacity,q+1))}>+</button>
              <button className="btn sm" onClick={()=>setQty(data.capacity)}>满载</button>
            </div>
          </div>
        </div>
        <div className="modal-ft">
          <button className="btn" onClick={onClose}>取消</button>
          <button className="btn primary" onClick={()=>onConfirm(qty)}><Icon name="check" size={11}/> 确认更新</button>
        </div>
      </div>
    </div>
  );
};

// Toast
const Toast = ({ msg, type='info' }) => {
  if (!msg) return null;
  const colors = { info:'#1464A5', ok:'#2E8B57', err:'#C0392B', warn:'#D08419' };
  return (
    <div style={{
      position:'absolute', top:76, left:'50%', transform:'translateX(-50%)',
      background: colors[type], color:'#fff',
      padding:'8px 14px', borderRadius:4,
      fontSize:12, fontWeight:500,
      boxShadow:'0 4px 12px rgba(0,0,0,.2)',
      zIndex: 100,
    }}>{msg}</div>
  );
};

Object.assign(window, { AddCardModal, AlertsModal, ReplaceModal, Toast });
