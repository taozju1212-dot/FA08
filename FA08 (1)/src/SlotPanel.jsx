// Sample slot panel — single-column 8-slot rack (matching physical layout)
const SlotPanel = ({ slots, activeSlot, onSelectSlot, selected, onToggleSelect, onSelectAll }) => {
  const allSelected = slots.every(s => s.status === 'empty' || selected.includes(s.id));
  const nonEmptyCount = slots.filter(s => s.status !== 'empty').length;

  return (
    <div style={{
      height:'100%', display:'flex', flexDirection:'column',
      background:'#fff', borderRight:'1px solid var(--border)',
    }}>
      <div style={{
        padding:'8px 10px', borderBottom:'1px solid var(--divider)',
        background:'var(--surface-alt)',
        display:'flex', alignItems:'center', justifyContent:'space-between',
      }}>
        <div style={{fontSize:12, fontWeight:600}}>样本位</div>
        <div style={{display:'flex', gap:4, alignItems:'center', fontSize:10, color:'var(--ink-500)'}}>
          <span className={'ck' + (allSelected && nonEmptyCount>0 ?' on':'')}
            onClick={() => onSelectAll(!allSelected)} style={{width:13, height:13}}/>
          全选
        </div>
      </div>

      {/* Rack — single column, 8 slots */}
      <div style={{padding:'8px 10px', flex:1, display:'flex', flexDirection:'column', minHeight:0}}>
        <div style={{fontSize:9, color:'var(--ink-400)', textAlign:'center', marginBottom:3, letterSpacing:1}}>
          ▲ 正面
        </div>
        <div style={{
          border:'2px solid var(--border-strong)',
          background:'var(--surface-alt)',
          borderRadius:6,
          padding:'6px 6px',
          flex:1,
          display:'flex', flexDirection:'column', gap:4,
          minHeight:0,
        }}>
          {slots.map(s => {
            const isActive = activeSlot === s.id;
            const isSel = selected.includes(s.id);
            const statusLabel = {
              empty:'空闲', scheduled:'已编排', running:'检测中',
              done:'已完成', error:'异常',
            }[s.status];
            const isEmpty = s.status === 'empty';
            // Sample tube icon (inline SVG) — different fills for empty vs occupied
            const TubeIcon = ({ filled }) => (
              <svg width="14" height="16" viewBox="0 0 14 16" style={{flexShrink:0}}>
                <path d="M3 1 H11 V4 L9.5 14 a1.5 1.5 0 0 1 -1.5 1.2 H6 a1.5 1.5 0 0 1 -1.5 -1.2 L3 4 Z"
                  fill={filled ? '#1E78BE' : '#fff'}
                  stroke={filled ? '#0F4E85' : '#A5B0BC'} strokeWidth="1.2"/>
                {filled && <path d="M4 9 L10 9" stroke="#fff" strokeWidth="1" opacity=".7"/>}
                <path d="M2.5 1 H11.5" stroke={filled ? '#0F4E85' : '#A5B0BC'} strokeWidth="1.4" strokeLinecap="round"/>
              </svg>
            );
            return (
              <div key={s.id}
                onClick={() => onSelectSlot(s.id)}
                style={{
                  position:'relative',
                  flex:1,
                  border: '1.5px solid ' + (isActive ? 'var(--blue-700)' : 'var(--border-strong)'),
                  borderRadius: 4,
                  background: isActive ? 'var(--blue-100)' : '#fff',
                  cursor:'pointer',
                  padding:'2px 6px',
                  display:'flex', alignItems:'center', gap:6,
                  boxShadow: isActive ? '0 0 0 2px var(--blue-100)' : 'none',
                  minHeight: 0,
                }}>
                <TubeIcon filled={!isEmpty}/>
                <span className="mono" style={{
                  fontSize:13, fontWeight:800, flexShrink:0,
                  color: isActive ? 'var(--blue-800)' : 'var(--ink-800)',
                  minWidth: 22,
                }}>{s.id}</span>
                <span style={{
                  flex:1, textAlign:'right',
                  fontSize:10, fontWeight:600,
                  color: isEmpty ? 'var(--ink-400)' :
                    s.status==='error' ? '#C0392B' :
                    s.status==='done' ? '#2E8B57' :
                    'var(--blue-700)',
                }}>{statusLabel}</span>
                {s.status !== 'empty' && (
                  <span className={'ck' + (isSel?' on':'')}
                    style={{width:13, height:13, flexShrink:0}}
                    onClick={(e) => { e.stopPropagation(); onToggleSelect(s.id); }}/>
                )}
              </div>
            );
          })}
        </div>
        <div style={{
          fontSize:9, color:'var(--ink-400)', textAlign:'center', marginTop:3,
          letterSpacing:1,
        }}>▼ 内侧</div>
      </div>
    </div>
  );
};

window.SlotPanel = SlotPanel;
