// Sample application form
const SampleForm = ({ slot, onSubmit, onCancel, onKeyboard, keyboardTarget, initial, nextSeq }) => {
  const [form, setForm] = React.useState(() => ({
    sampleId: initial?.sampleId || '',
    name:     initial?.name || '',
    gender:   initial?.gender || '男',
    age:      initial?.age || '',
    ageUnit:  initial?.ageUnit || '岁',
    type:     initial?.type || '血清',
    assays:   initial?.assays || [],
    assayCfg: initial?.assayCfg || {}, // { [code]: { repeat:1, dilution:1 } }
  }));
  const [expanded, setExpanded] = React.useState(null); // assay code currently expanded

  const update = (k, v) => setForm(f => ({...f, [k]: v}));
  const toggleAssay = (code) => {
    setForm(f => {
      const on = f.assays.includes(code);
      const assays = on ? f.assays.filter(c => c!==code) : [...f.assays, code];
      const assayCfg = {...f.assayCfg};
      if (!on && !assayCfg[code]) assayCfg[code] = { repeat: 1, dilution: 1 };
      return {...f, assays, assayCfg};
    });
  };
  const updateCfg = (code, key, val) => {
    setForm(f => ({
      ...f,
      assayCfg: {...f.assayCfg, [code]: {...(f.assayCfg[code] || {repeat:1, dilution:1}), [key]: val}}
    }));
  };

  const valid = form.sampleId && form.name && form.assays.length > 0;
  const seq = String(nextSeq || 1).padStart(3, '0');

  React.useEffect(() => {
    window.__sampleFormSetValue = (key, val) => update(key, val);
    return () => { delete window.__sampleFormSetValue; };
  }, []);

  const FieldLabel = ({ children, required }) => (
    <label style={{fontSize:11, color:'var(--ink-500)', fontWeight:500}}>
      {children}{required && <span style={{color:'var(--err)', marginLeft:2}}>*</span>}
    </label>
  );

  const REPEATS = [1, 2, 3];
  const DILUTIONS = [1, 5, 10, 50, 100];

  return (
    <div style={{height:'100%', display:'flex', flexDirection:'column', background:'#fff'}}>
      <div style={{
        padding: '10px 14px',
        borderBottom:'1px solid var(--divider)',
        background:'var(--surface-alt)',
        display:'flex', alignItems:'center', justifyContent:'space-between',
      }}>
        <div style={{fontSize:13, fontWeight:600}}>
          <Icon name="edit" size={13}/> 样本申请
          <span style={{marginLeft:10, fontSize:11, color:'var(--ink-500)', fontWeight:400}}>
            位置：<span className="mono" style={{color:'var(--blue-700)', fontWeight:600}}>{slot?.id || '—'}</span>
            {' · '}编号：<span className="mono" style={{color:'var(--blue-700)', fontWeight:600}}>{seq}</span>
          </span>
        </div>
      </div>

      <div className="scroll" style={{flex:1, padding:'10px 14px'}}>
        {/* Row 1: sample ID + name */}
        <div style={{display:'grid', gridTemplateColumns:'1fr 1fr', gap:10, marginBottom:10}}>
          <div className="field">
            <FieldLabel required>样本ID</FieldLabel>
            <div style={{display:'flex', gap:4}}>
              <input className={'input' + (keyboardTarget==='sampleId' ? ' focus' : '')}
                style={{flex:1}}
                value={form.sampleId}
                readOnly
                onClick={() => onKeyboard('sampleId', form.sampleId)}
                placeholder="点击输入或扫码"/>
              <button className="btn sm" onClick={() => update('sampleId', '2026041810' + Math.floor(Math.random()*900+100))}>
                <Icon name="scan" size={12}/> 扫码
              </button>
            </div>
          </div>
          <div className="field">
            <FieldLabel required>姓名</FieldLabel>
            <input className={'input' + (keyboardTarget==='name' ? ' focus' : '')}
              value={form.name}
              readOnly
              onClick={() => onKeyboard('name', form.name)}
              placeholder="点击输入"/>
          </div>
        </div>

        {/* Row 2: gender / age / type — inline */}
        <div style={{display:'grid', gridTemplateColumns:'auto 1fr auto', gap:10, marginBottom:10, alignItems:'end'}}>
          <div className="field" style={{minWidth:120}}>
            <FieldLabel>性别</FieldLabel>
            <div style={{display:'flex', gap:4}}>
              {GENDERS.map(g => (
                <button key={g}
                  onClick={() => update('gender', g)}
                  style={{
                    width:44, height:30,
                    border:'1px solid ' + (form.gender===g ? 'var(--blue-700)' : 'var(--border-strong)'),
                    background: form.gender===g ? 'var(--blue-100)' : '#fff',
                    color: form.gender===g ? 'var(--blue-700)' : 'var(--ink-800)',
                    borderRadius:4, fontSize:12, fontWeight: form.gender===g ? 600 : 400,
                    cursor:'pointer',
                  }}>{g}</button>
              ))}
            </div>
          </div>
          <div className="field">
            <FieldLabel>年龄</FieldLabel>
            <div style={{display:'flex', gap:4}}>
              <input className={'input mono' + (keyboardTarget==='age' ? ' focus' : '')}
                style={{flex:1}}
                value={form.age}
                readOnly
                onClick={() => onKeyboard('age', String(form.age))}
                placeholder="—"/>
              <select className="input" style={{width:56}} value={form.ageUnit} onChange={e => update('ageUnit', e.target.value)}>
                {AGE_UNITS.map(u => <option key={u}>{u}</option>)}
              </select>
            </div>
          </div>
          <div className="field">
            <FieldLabel>样本类型</FieldLabel>
            <div style={{display:'flex', gap:3}}>
              {SAMPLE_TYPES.map(t => (
                <button key={t}
                  onClick={() => update('type', t)}
                  style={{
                    width:44, height:30,
                    border:'1px solid ' + (form.type===t ? 'var(--blue-700)' : 'var(--border-strong)'),
                    background: form.type===t ? 'var(--blue-100)' : '#fff',
                    color: form.type===t ? 'var(--blue-700)' : 'var(--ink-700)',
                    borderRadius:4, fontSize:11, fontWeight: form.type===t ? 600 : 400,
                    cursor:'pointer', padding:0,
                  }}>{t}</button>
              ))}
            </div>
          </div>
        </div>

        {/* Assays — 3x2 grid */}
        <div className="field">
          <FieldLabel required>检测项目 <span style={{color:'var(--ink-400)', fontWeight:400}}>（已选 {form.assays.length}）</span></FieldLabel>
          <div style={{display:'grid', gridTemplateColumns:'1fr 1fr 1fr', gap:5, marginTop:3}}>
            {ASSAYS.map(a => {
              const on = form.assays.includes(a.code);
              const isExp = expanded === a.code;
              const cfg = form.assayCfg[a.code] || { repeat:1, dilution:1 };
              return (
                <div key={a.code} style={{
                  gridColumn: isExp ? 'span 3' : 'span 1',
                  border:'1px solid ' + (on ? 'var(--blue-700)' : 'var(--border)'),
                  background: on ? 'var(--blue-050, #EEF5FB)' : '#fff',
                  borderRadius:4,
                  transition:'all .15s',
                }}>
                  <div
                    onClick={() => toggleAssay(a.code)}
                    style={{
                      padding:'6px 8px',
                      cursor:'pointer',
                      display:'flex', alignItems:'center', gap:6,
                      minHeight: 44,
                    }}>
                    <span className={'ck' + (on ? ' on' : '')} style={{width:14, height:14, flexShrink:0}}/>
                    <div style={{flex:1, minWidth:0}}>
                      <div style={{fontSize:12, fontWeight:600, color: on?'var(--blue-700)':'var(--ink-800)', lineHeight:1.2}}>{a.code}</div>
                      <div style={{fontSize:10, color:'var(--ink-500)', lineHeight:1.2, overflow:'hidden', textOverflow:'ellipsis', whiteSpace:'nowrap'}}>{a.name}</div>
                    </div>
                    {on && (
                      <button
                        onClick={(e) => { e.stopPropagation(); setExpanded(isExp ? null : a.code); }}
                        style={{
                          border:'none', background:'transparent', cursor:'pointer',
                          padding:2, display:'grid', placeItems:'center',
                          color:'var(--blue-700)',
                        }}
                        title="配置重复次数与稀释倍数">
                        <Icon name={isExp ? 'chevron-up' : 'chevron-down'} size={12} color="currentColor"/>
                      </button>
                    )}
                  </div>

                  {on && isExp && (
                    <div style={{
                      padding:'8px 12px 10px',
                      borderTop:'1px dashed var(--blue-100, #B8D2EB)',
                      display:'flex', gap:20, alignItems:'center', flexWrap:'wrap',
                    }}>
                      <div style={{display:'flex', alignItems:'center', gap:8}}>
                        <span style={{fontSize:11, color:'var(--ink-600)', fontWeight:500}}>重复次数</span>
                        <div style={{display:'flex', gap:2}}>
                          {REPEATS.map(r => (
                            <button key={r}
                              onClick={() => updateCfg(a.code, 'repeat', r)}
                              style={{
                                width:30, height:26,
                                border:'1px solid ' + (cfg.repeat===r ? 'var(--blue-700)' : 'var(--border-strong)'),
                                background: cfg.repeat===r ? 'var(--blue-700)' : '#fff',
                                color: cfg.repeat===r ? '#fff' : 'var(--ink-700)',
                                borderRadius:3, fontSize:11, fontWeight:600,
                                cursor:'pointer', padding:0,
                              }}>{r}</button>
                          ))}
                        </div>
                      </div>
                      <div style={{display:'flex', alignItems:'center', gap:8}}>
                        <span style={{fontSize:11, color:'var(--ink-600)', fontWeight:500}}>稀释倍数</span>
                        <div style={{display:'flex', gap:2}}>
                          {DILUTIONS.map(d => (
                            <button key={d}
                              onClick={() => updateCfg(a.code, 'dilution', d)}
                              style={{
                                minWidth:38, height:26, padding:'0 6px',
                                border:'1px solid ' + (cfg.dilution===d ? 'var(--blue-700)' : 'var(--border-strong)'),
                                background: cfg.dilution===d ? 'var(--blue-700)' : '#fff',
                                color: cfg.dilution===d ? '#fff' : 'var(--ink-700)',
                                borderRadius:3, fontSize:11, fontWeight:600,
                                cursor:'pointer',
                              }}>{d===1 ? '原液' : d+'×'}</button>
                          ))}
                        </div>
                      </div>
                      <div style={{flex:1}}/>
                      <div style={{fontSize:10, color:'var(--ink-500)'}}>
                        参考区间 <span className="mono">{a.ref}</span> {a.unit}
                      </div>
                    </div>
                  )}
                </div>
              );
            })}
          </div>
        </div>
      </div>

      <div style={{
        padding:'8px 14px',
        borderTop:'1px solid var(--divider)',
        background:'var(--surface-alt)',
        display:'flex', justifyContent:'flex-end', gap:8,
      }}>
        <button className="btn" onClick={onCancel}>取消</button>
        <button className="btn primary" disabled={!valid} onClick={() => onSubmit(form)}>
          <Icon name="check" size={12}/> 提交申请
        </button>
      </div>
    </div>
  );
};

window.SampleForm = SampleForm;
