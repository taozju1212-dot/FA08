// Results page: filter + sample-grouped list + curve detail
const ResultsPage = ({ results }) => {
  const [search, setSearch] = React.useState('');
  const [expanded, setExpanded] = React.useState(results[0]?.id || null);
  const [selectedIds, setSelectedIds] = React.useState([]);
  const [dateFrom, setDateFrom] = React.useState('2026-04-18');
  const [curveItem, setCurveItem] = React.useState(null);

  const filtered = results.filter(r =>
    !search || r.name.includes(search) || r.sampleId.includes(search)
  );

  const toggleSel = (id) => setSelectedIds(s => s.includes(id)?s.filter(x=>x!==id):[...s,id]);

  return (
    <div style={{flex:1, display:'flex', flexDirection:'column', padding:8, gap:8, minHeight:0, background:'var(--bg)'}}>
      {/* Filter bar */}
      <div style={{
        background:'#fff', border:'1px solid var(--border)', borderRadius:6,
        padding:'8px 10px', display:'flex', alignItems:'center', gap:8, flexWrap:'wrap',
      }}>
        <div style={{display:'flex', alignItems:'center', gap:6, fontSize:12}}>
          <span style={{color:'var(--ink-500)'}}>日期</span>
          <input type="date" className="input" style={{width:132, height:28, fontSize:12}}
            value={dateFrom} onChange={e=>setDateFrom(e.target.value)}/>
          <span style={{color:'var(--ink-400)'}}>—</span>
          <input type="date" className="input" style={{width:132, height:28, fontSize:12}} defaultValue="2026-04-18"/>
        </div>
        <div className="vsep" style={{height:20}}/>
        <div style={{display:'flex', alignItems:'center', gap:4, flex:1, minWidth:160}}>
          <Icon name="search" size={14} color="#7A8898"/>
          <input className="input" style={{flex:1, height:28, fontSize:12}}
            placeholder="搜索姓名 / 样本ID / 项目..."
            value={search} onChange={e=>setSearch(e.target.value)}/>
        </div>
        <button className="btn sm"><Icon name="search" size={11}/> 高级</button>
        <div className="vsep" style={{height:20}}/>
        <button className="btn sm" disabled={selectedIds.length===0}><Icon name="print" size={11}/> 打印</button>
        <button className="btn sm" disabled={selectedIds.length===0}><Icon name="export" size={11}/> 导出</button>
        <button className="btn sm" disabled={selectedIds.length===0}><Icon name="trash" size={11}/> 删除</button>
      </div>

      <div style={{flex:1, display:'flex', gap:8, minHeight:0}}>
        {/* Result list */}
        <div style={{flex: curveItem ? 1.2 : 1, background:'#fff', border:'1px solid var(--border)', borderRadius:6, display:'flex', flexDirection:'column', minWidth:0, overflow:'hidden'}}>
          <div style={{
            padding:'6px 10px', borderBottom:'1px solid var(--divider)',
            background:'var(--surface-alt)', fontSize:12, fontWeight:600,
            display:'flex', justifyContent:'space-between', alignItems:'center',
          }}>
            <span>检测结果 <span style={{color:'var(--ink-500)', fontWeight:400}}>共 {filtered.length} 条</span></span>
            <span style={{fontSize:11, color:'var(--ink-500)', fontWeight:400}}>已选 {selectedIds.length}</span>
          </div>
          <div className="scroll" style={{flex:1}}>
            {filtered.map(r => {
              const open = expanded === r.id;
              const sel = selectedIds.includes(r.id);
              const hasPos = r.items.some(i => i.concl === '阳性');
              return (
                <div key={r.id} style={{borderBottom:'1px solid var(--divider)'}}>
                  <div style={{
                    display:'flex', alignItems:'center', gap:8,
                    padding:'8px 10px', cursor:'pointer',
                    background: sel ? 'var(--blue-050)' : '#fff',
                  }} onClick={() => setExpanded(open ? null : r.id)}>
                    <span className={'ck' + (sel?' on':'')} onClick={(e)=>{e.stopPropagation(); toggleSel(r.id);}}/>
                    <Icon name={open?'chevron-down':'chevron-right'} size={12} color="#7A8898"/>
                    <span className="mono" style={{fontSize:11, color:'var(--blue-700)', fontWeight:700, minWidth:90}}>{r.sampleId}</span>
                    <span style={{fontSize:12, fontWeight:600, minWidth:48}}>{r.name}</span>
                    <span style={{fontSize:11, color:'var(--ink-500)'}}>{r.gender}·{r.age}岁·{r.type}</span>
                    <span style={{flex:1}}/>
                    <span className="pill" style={{fontSize:10}}>{r.items.length} 项</span>
                    {hasPos && <span className="pill err" style={{fontSize:10}}><span className="dot"/>阳性</span>}
                    <span style={{fontSize:11, color:'var(--ink-500)'}} className="mono">{r.time.slice(11)}</span>
                  </div>
                  {open && (
                    <div style={{padding:'0 10px 10px 36px', background:'var(--surface-alt)'}}>
                      <table className="tbl" style={{background:'#fff', border:'1px solid var(--border)', borderRadius:4}}>
                        <thead>
                          <tr>
                            <th style={{width:64}}>项目</th>
                            <th style={{width:90}}>结果</th>
                            <th style={{width:90}}>参考范围</th>
                            <th style={{width:60}}>结论</th>
                            <th style={{width:46}}>CV</th>
                            <th></th>
                          </tr>
                        </thead>
                        <tbody>
                          {r.items.map(it => (
                            <tr key={it.code}>
                              <td style={{fontWeight:600}}>{it.code}</td>
                              <td className="num" style={{color: it.concl==='阳性'?'var(--err)':'var(--ink-800)', fontWeight:700}}>
                                {it.value} <span style={{fontSize:10, color:'var(--ink-500)', fontWeight:400}}>{it.unit}</span>
                              </td>
                              <td className="num" style={{fontSize:11, color:'var(--ink-500)'}}>{it.ref}</td>
                              <td>
                                <span className={'pill '+(it.concl==='阳性'?'err':'ok')} style={{fontSize:10}}>
                                  <span className="dot"/>{it.concl}
                                </span>
                              </td>
                              <td className="num" style={{fontSize:11, color:'var(--ink-500)'}}>{it.cv}</td>
                              <td style={{textAlign:'right'}}>
                                <button className="btn sm" onClick={() => setCurveItem({ result:r, item:it })}>
                                  <Icon name="chart" size={11}/> 曲线
                                </button>
                              </td>
                            </tr>
                          ))}
                        </tbody>
                      </table>
                    </div>
                  )}
                </div>
              );
            })}
          </div>
        </div>

        {/* Curve detail */}
        {curveItem && (
          <div style={{width:300, background:'#fff', border:'1px solid var(--border)', borderRadius:6, display:'flex', flexDirection:'column'}}>
            <div style={{
              padding:'6px 10px', borderBottom:'1px solid var(--divider)',
              background:'var(--surface-alt)', fontSize:12, fontWeight:600,
              display:'flex', justifyContent:'space-between', alignItems:'center',
            }}>
              <span><Icon name="chart" size={12}/> 荧光曲线 · {curveItem.item.code}</span>
              <span onClick={()=>setCurveItem(null)} style={{cursor:'pointer', color:'var(--ink-500)'}}>
                <Icon name="close" size={12}/>
              </span>
            </div>
            <div style={{padding:10, flex:1, overflow:'auto'}}>
              <div style={{fontSize:11, color:'var(--ink-500)', marginBottom:4}}>{curveItem.result.name} · {curveItem.result.sampleId}</div>
              <svg viewBox="0 0 280 160" style={{width:'100%', height:160, border:'1px solid var(--divider)', background:'var(--surface-alt)', borderRadius:4}}>
                {/* grid */}
                {[0,1,2,3,4].map(i => <line key={'h'+i} x1="20" y1={20+i*30} x2="270" y2={20+i*30} stroke="#E6ECF2" strokeWidth=".5"/>)}
                {[0,1,2,3,4,5].map(i => <line key={'v'+i} x1={20+i*50} y1="20" x2={20+i*50} y2="140" stroke="#E6ECF2" strokeWidth=".5"/>)}
                {/* axis */}
                <line x1="20" y1="140" x2="270" y2="140" stroke="#B7C3CE"/>
                <line x1="20" y1="20"  x2="20"  y2="140" stroke="#B7C3CE"/>
                {/* curve */}
                <path d="M20 130 Q 60 120 90 95 T 160 50 T 240 35 L 270 32" stroke="#1464A5" strokeWidth="1.8" fill="none"/>
                {/* points */}
                {[[20,130],[70,112],[120,80],[170,52],[220,38],[265,33]].map(([x,y],i) => (
                  <circle key={i} cx={x} cy={y} r="2.5" fill="#fff" stroke="#1464A5" strokeWidth="1.5"/>
                ))}
                {/* labels */}
                <text x="22" y="16" fontSize="9" fill="#5B6B7B">强度</text>
                <text x="250" y="154" fontSize="9" fill="#5B6B7B">时间(s)</text>
              </svg>
              <div style={{marginTop:8, fontSize:11, display:'grid', gridTemplateColumns:'1fr 1fr', gap:4, color:'var(--ink-700)'}}>
                <div><span style={{color:'var(--ink-500)'}}>T/C：</span><span className="mono">{(Math.random()*2+0.8).toFixed(3)}</span></div>
                <div><span style={{color:'var(--ink-500)'}}>T值：</span><span className="mono">{Math.floor(Math.random()*5000+1000)}</span></div>
                <div><span style={{color:'var(--ink-500)'}}>C值：</span><span className="mono">{Math.floor(Math.random()*5000+2000)}</span></div>
                <div><span style={{color:'var(--ink-500)'}}>积分时间：</span><span className="mono">15s</span></div>
                <div><span style={{color:'var(--ink-500)'}}>批号：</span><span className="mono">A2603-01</span></div>
                <div><span style={{color:'var(--ink-500)'}}>温度：</span><span className="mono">37.2°C</span></div>
              </div>
            </div>
          </div>
        )}
      </div>
    </div>
  );
};

window.ResultsPage = ResultsPage;
