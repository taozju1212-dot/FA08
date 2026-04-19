// Consumables management page — deep management (beyond bottom-bar chips)
const ConsumablesPage = ({ consumables, cards, onReplace, onLoadDilution }) => {
  const [tab, setTab] = React.useState('tray');

  const Bar = ({ pct, color }) => (
    <div style={{height:6, background:'var(--surface-sunken)', borderRadius:3, overflow:'hidden'}}>
      <div style={{height:'100%', width:pct+'%', background:color, transition:'width .3s'}}/>
    </div>
  );

  const statusOf = (qty, cap) => {
    const pct = qty/cap*100;
    if (pct < 10) return { label:'耗尽', cls:'err', color:'#C0392B' };
    if (pct < 25) return { label:'低量', cls:'warn', color:'#D08419' };
    return { label:'充足', cls:'ok', color:'#2E8B57' };
  };

  return (
    <div style={{flex:1, display:'flex', flexDirection:'column', padding:8, gap:8, background:'var(--bg)', minHeight:0}}>
      {/* sub-tabs */}
      <div style={{display:'flex', gap:2, borderBottom:'1px solid var(--border)', background:'#fff', borderRadius:'6px 6px 0 0', padding:'4px 4px 0'}}>
        {[
          {k:'tray',   l:'托盘耗材',    i:'tip'},
          {k:'cards',  l:'试剂卡库存',   i:'card'},
          {k:'dilutions', l:'稀释液', i:'flask'},
        ].map(t => (
          <button key={t.k} onClick={() => setTab(t.k)} style={{
            height:34, padding:'0 14px',
            border:'none',
            background: tab===t.k ? 'var(--blue-050)' : 'transparent',
            color: tab===t.k ? 'var(--blue-700)' : 'var(--ink-700)',
            borderBottom: tab===t.k ? '2px solid var(--blue-700)' : '2px solid transparent',
            fontSize:12, fontWeight: tab===t.k?700:500,
            cursor:'pointer', display:'flex', alignItems:'center', gap:6,
          }}>
            <Icon name={t.i} size={12}/> {t.l}
          </button>
        ))}
      </div>

      {tab === 'tray' && (
        <div style={{flex:1, background:'#fff', border:'1px solid var(--border)', borderRadius:6, padding:12, display:'flex', gap:12, minHeight:0}}>
          {/* Schematic tray */}
          <div style={{width:260, flexShrink:0}}>
            <div style={{fontSize:12, fontWeight:600, marginBottom:6}}>托盘布局示意</div>
            <div style={{fontSize:10, color:'var(--ink-400)', textAlign:'center', letterSpacing:1, marginBottom:4}}>▲ 内侧</div>
            <div style={{
              border:'2px solid var(--border-strong)', background:'var(--surface-alt)',
              borderRadius:6, padding:8, display:'grid', gap:6,
              gridTemplateColumns:'1fr 1fr 1fr', gridTemplateRows:'1fr 1fr',
            }}>
              {consumables.dilutions.map(d => {
                const pct = d.qty/d.capacity*100;
                const st = statusOf(d.qty, d.capacity);
                return (
                  <div key={d.slot} style={{
                    background:'#fff', border:'1.5px solid '+st.color,
                    borderRadius:4, padding:'6px 6px', textAlign:'center',
                  }}>
                    <div style={{fontSize:11, fontWeight:700, color:st.color}}>{d.slot}</div>
                    <div style={{fontSize:9, color:'var(--ink-500)'}}>稀释液</div>
                    <div className="mono" style={{fontSize:13, fontWeight:700, marginTop:2}}>{d.qty}<span style={{fontSize:9, fontWeight:400, color:'var(--ink-400)'}}>/{d.capacity}</span></div>
                  </div>
                );
              })}
              <div style={{
                background:'#fff', border:'1.5px solid var(--ok)',
                borderRadius:4, padding:'6px 6px', textAlign:'center', gridColumn:'1 / 2',
              }}>
                <div style={{fontSize:11, fontWeight:700, color:'var(--ok)'}}>TIP</div>
                <div style={{fontSize:9, color:'var(--ink-500)'}}>吸头盒</div>
                <div className="mono" style={{fontSize:13, fontWeight:700, marginTop:2}}>{consumables.tips.qty}<span style={{fontSize:9, fontWeight:400, color:'var(--ink-400)'}}>/{consumables.tips.capacity}</span></div>
              </div>
              <div style={{
                background:'#fff', border:'1.5px solid var(--ok)',
                borderRadius:4, padding:'6px 6px', textAlign:'center', gridColumn:'2 / 4',
              }}>
                <div style={{fontSize:11, fontWeight:700, color:'var(--ok)'}}>深孔板</div>
                <div style={{fontSize:9, color:'var(--ink-500)'}}>样本/稀释液混匀</div>
                <div className="mono" style={{fontSize:13, fontWeight:700, marginTop:2}}>{consumables.plate.qty}<span style={{fontSize:9, fontWeight:400, color:'var(--ink-400)'}}>/{consumables.plate.capacity}</span></div>
              </div>
            </div>
            <div style={{fontSize:10, color:'var(--ink-400)', textAlign:'center', letterSpacing:1, marginTop:4}}>▼ 正面（抽出）</div>
            <div style={{fontSize:10, color:'var(--ink-500)', marginTop:10, lineHeight:1.6, padding:'8px', background:'var(--surface-alt)', borderRadius:4}}>
              <b>使用说明：</b><br/>
              从正面将托盘抽出 → 更换耗材后 → 推入仪器，系统会自动识别。
            </div>
          </div>

          {/* Details */}
          <div style={{flex:1, display:'flex', flexDirection:'column', gap:8, minHeight:0, overflow:'auto'}}>
            {[
              { key:'tips',  icon:'tip',   data: consumables.tips },
              { key:'plate', icon:'plate', data: consumables.plate },
            ].map(c => {
              const st = statusOf(c.data.qty, c.data.capacity);
              return (
                <div key={c.key} style={{border:'1px solid var(--border)', borderRadius:6, padding:10}}>
                  <div style={{display:'flex', alignItems:'center', gap:8, marginBottom:8}}>
                    <Icon name={c.icon} size={18} color={st.color}/>
                    <div style={{flex:1}}>
                      <div style={{fontSize:13, fontWeight:700}}>{c.data.name}</div>
                      <div style={{fontSize:10, color:'var(--ink-500)'}}>批号 {c.data.lotNo} · 有效期 {c.data.expire}</div>
                    </div>
                    <span className={'pill '+st.cls}><span className="dot"/>{st.label}</span>
                    <button className="btn sm primary" onClick={() => onReplace(c.key)}>
                      <Icon name="refresh" size={11}/> 更换
                    </button>
                  </div>
                  <div style={{display:'flex', alignItems:'center', gap:8}}>
                    <div style={{flex:1}}><Bar pct={c.data.qty/c.data.capacity*100} color={st.color}/></div>
                    <div className="mono" style={{fontSize:12, fontWeight:700, minWidth:70, textAlign:'right'}}>
                      {c.data.qty} / {c.data.capacity}
                    </div>
                  </div>
                </div>
              );
            })}
            {consumables.dilutions.map((d, i) => {
              const st = statusOf(d.qty, d.capacity);
              return (
                <div key={d.slot} style={{border:'1px solid var(--border)', borderRadius:6, padding:10}}>
                  <div style={{display:'flex', alignItems:'center', gap:8, marginBottom:8}}>
                    <Icon name="flask" size={18} color={st.color}/>
                    <div style={{flex:1}}>
                      <div style={{fontSize:13, fontWeight:700}}>{d.name} <span style={{fontSize:10, color:'var(--ink-500)', fontWeight:400, marginLeft:4}}>位置 {d.slot}</span></div>
                      <div style={{fontSize:10, color:'var(--ink-500)'}}>批号 {d.lotNo} · 有效期 {d.expire}</div>
                    </div>
                    <span className={'pill '+st.cls}><span className="dot"/>{st.label}</span>
                    <button className="btn sm" onClick={() => onLoadDilution(i, 'load')}>装载</button>
                    <button className="btn sm" onClick={() => onLoadDilution(i, 'unload')}>卸载</button>
                  </div>
                  <div style={{display:'flex', alignItems:'center', gap:8}}>
                    <div style={{flex:1}}><Bar pct={d.qty/d.capacity*100} color={st.color}/></div>
                    <div className="mono" style={{fontSize:12, fontWeight:700, minWidth:70, textAlign:'right'}}>
                      {d.qty} / {d.capacity} ml
                    </div>
                  </div>
                </div>
              );
            })}
          </div>
        </div>
      )}

      {tab === 'cards' && (
        <div style={{flex:1, background:'#fff', border:'1px solid var(--border)', borderRadius:6, overflow:'hidden', display:'flex', flexDirection:'column'}}>
          <div className="scroll" style={{flex:1}}>
            <table className="tbl">
              <thead><tr>
                <th>项目</th><th>LOT批号</th><th>有效期</th><th style={{width:120}}>剩余数量</th><th style={{width:80}}>状态</th><th></th>
              </tr></thead>
              <tbody>
                {cards.map(c => {
                  const pct = Math.min(100, c.remaining/30*100);
                  const st = statusOf(c.remaining, 30);
                  return (
                    <tr key={c.code}>
                      <td style={{fontWeight:700}}>{c.code} <span style={{fontSize:10, color:'var(--ink-500)', fontWeight:400, marginLeft:4}}>{ASSAYS.find(a=>a.code===c.code)?.name}</span></td>
                      <td className="mono">{c.lot}</td>
                      <td className="mono">{c.expire}</td>
                      <td>
                        <div style={{display:'flex', alignItems:'center', gap:8}}>
                          <div style={{flex:1, maxWidth:100}}><Bar pct={pct} color={st.color}/></div>
                          <span className="mono" style={{fontSize:12, fontWeight:700, minWidth:32}}>{c.remaining}</span>
                        </div>
                      </td>
                      <td><span className={'pill '+st.cls}><span className="dot"/>{st.label}</span></td>
                      <td style={{textAlign:'right'}}><button className="btn sm">补充</button></td>
                    </tr>
                  );
                })}
              </tbody>
            </table>
          </div>
        </div>
      )}

      {tab === 'dilutions' && (
        <div style={{flex:1, background:'#fff', border:'1px solid var(--border)', borderRadius:6, padding:12, display:'grid', gridTemplateColumns:'1fr 1fr 1fr', gap:10}}>
          {consumables.dilutions.map((d, i) => {
            const st = statusOf(d.qty, d.capacity);
            return (
              <div key={d.slot} style={{border:'1px solid var(--border)', borderRadius:6, padding:10, display:'flex', flexDirection:'column', gap:6}}>
                <div style={{display:'flex', justifyContent:'space-between', alignItems:'center'}}>
                  <div style={{fontSize:13, fontWeight:700}}>{d.name}</div>
                  <span className={'pill '+st.cls}><span className="dot"/>{st.label}</span>
                </div>
                <div style={{fontSize:10, color:'var(--ink-500)'}}>位置 {d.slot} · 批号 {d.lotNo}</div>
                <div style={{fontSize:10, color:'var(--ink-500)'}}>有效期 {d.expire}</div>
                <div className="mono" style={{fontSize:22, fontWeight:700, color:st.color, textAlign:'center', margin:'8px 0 4px'}}>
                  {d.qty}<span style={{fontSize:12, fontWeight:400, color:'var(--ink-400)'}}> / {d.capacity} ml</span>
                </div>
                <Bar pct={d.qty/d.capacity*100} color={st.color}/>
                <div style={{display:'flex', gap:4, marginTop:6}}>
                  <button className="btn sm" style={{flex:1}} onClick={() => onLoadDilution(i,'load')}><Icon name="scan" size={11}/> 扫码装载</button>
                  <button className="btn sm" style={{flex:1}} onClick={() => onLoadDilution(i,'unload')}>卸载</button>
                </div>
              </div>
            );
          })}
        </div>
      )}
    </div>
  );
};

window.ConsumablesPage = ConsumablesPage;
