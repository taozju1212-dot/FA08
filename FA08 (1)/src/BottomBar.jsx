// Bottom bar: compact read-only consumables + prominent card button + device controls
const BottomBar = ({
  consumables, device, onStart, onPause,
  scheduledCount, pendingCardCount, onAddCard
}) => {
  const canStart = device === 'idle' || device === 'paused';
  const canPause = device === 'running';

  const Chip = ({ icon, qty, capacity, label, warn, crit }) => {
    const color = crit ? '#C0392B' : warn ? '#D08419' : '#2E8B57';
    return (
      <div style={{
        display:'flex', alignItems:'center', gap:5,
        padding:'0 8px', height:38,
        background:'#fff', border:'1px solid var(--border)', borderRadius:4,
      }}>
        <Icon name={icon} size={14} color={color}/>
        <span style={{fontSize:10, color:'var(--ink-500)'}}>{label}</span>
        <span className="mono" style={{fontSize:13, fontWeight:700, color}}>
          {qty}<span style={{fontSize:9, fontWeight:400, color:'var(--ink-400)'}}>/{capacity}</span>
        </span>
      </div>
    );
  };

  const DChip = ({ d }) => {
    const loaded = d.loaded !== false;
    const pct = loaded && d.capacity > 0 ? Math.max(0, Math.min(1, d.qty / d.capacity)) : 0;
    const state = !loaded ? 'unloaded'
      : pct < 0.12 ? 'crit'
      : pct < 0.25 ? 'warn'
      : 'ok';
    const color = {
      unloaded: '#C5CCD4',
      crit:     '#E0574B',
      warn:     '#E8A33D',
      ok:       '#4CAF50',
    }[state];
    const trackColor = loaded ? '#E6EAEF' : '#EEF0F3';

    const R = 10, C = 2 * Math.PI * R;
    const dash = loaded ? C * pct : 0;

    return (
      <div style={{
        display:'flex', flexDirection:'column', alignItems:'center', justifyContent:'center',
        width:46, height:38, gap:1,
        background: loaded ? '#fff' : '#F5F6F8',
        border:'1px solid ' + (loaded ? 'var(--border)' : '#E0E3E7'),
        borderRadius:4,
      }} title={loaded ? `${d.name}  ${d.qty}/${d.capacity}` : `${d.name} 未装载`}>
        <svg width="22" height="22" viewBox="0 0 24 24">
          <circle cx="12" cy="12" r={R} fill="none" stroke={trackColor} strokeWidth="2.5"/>
          {loaded ? (
            <circle cx="12" cy="12" r={R} fill="none"
              stroke={color} strokeWidth="2.5" strokeLinecap="round"
              strokeDasharray={`${dash} ${C}`}
              transform="rotate(-90 12 12)"/>
          ) : (
            <circle cx="12" cy="12" r={R} fill="none"
              stroke="#C5CCD4" strokeWidth="1" strokeDasharray="2 2.5"/>
          )}
          {state === 'unloaded' && (
            <text x="12" y="15" textAnchor="middle" fontSize="8" fill="#9AA4B0" fontWeight="700">—</text>
          )}
          {state === 'crit' && (
            <g>
              <path d="M12 8 V13" stroke={color} strokeWidth="1.8" strokeLinecap="round"/>
              <circle cx="12" cy="15.4" r="1" fill={color}/>
            </g>
          )}
          {(state === 'ok' || state === 'warn') && (
            <circle cx="12" cy="12" r="1.8" fill={color}/>
          )}
        </svg>
        <span style={{fontSize:9, fontWeight:700, color: loaded ? 'var(--ink-700)' : 'var(--ink-400)', lineHeight:1, letterSpacing:.3}}>
          {d.name}
        </span>
      </div>
    );
  };

  return (
    <div style={{
      height: 56,
      background: 'var(--surface-alt)',
      borderTop:'1px solid var(--border)',
      padding:'8px 10px',
      display:'flex', alignItems:'center', gap:6,
    }}>
      <Chip icon="tip" label="吸头"
        qty={consumables.tips.qty} capacity={consumables.tips.capacity}
        warn={consumables.tips.qty<20} crit={consumables.tips.qty<10}/>
      <Chip icon="plate" label="混匀板"
        qty={consumables.plate.qty} capacity={consumables.plate.capacity}
        warn={consumables.plate.qty<20} crit={consumables.plate.qty<10}/>
      {consumables.dilutions.map(d => <DChip key={d.slot} d={d}/>)}

      <div className="vsep" style={{height:28, margin:'0 6px'}}/>

      {/* Card queue — prominent */}
      <div onClick={onAddCard} style={{
        display:'flex', alignItems:'center', gap:8,
        padding:'0 14px', height:40,
        background: pendingCardCount>0 ? 'var(--warn-bg)' : '#fff',
        border:'1.5px solid '+(pendingCardCount>0 ? '#D08419' : 'var(--border-strong)'),
        borderRadius:5, cursor:'pointer',
      }}>
        <Icon name="card" size={18} color={pendingCardCount>0?'#8A5A10':'var(--blue-700)'}/>
        <span style={{fontSize:12, color:'var(--ink-700)', fontWeight:500}}>待加卡</span>
        <span className="mono" style={{fontSize:18, fontWeight:800,
          color: pendingCardCount>0 ? '#8A5A10' : 'var(--ink-800)'}}>{pendingCardCount}</span>
        <span style={{
          fontSize:13, padding:'6px 14px', background:'var(--blue-700)',
          color:'#fff', borderRadius:4, fontWeight:700, marginLeft:4,
        }}>加 卡</span>
      </div>

      <div style={{flex:1}}/>

      {/* Device controls */}
      <button className={'btn ' + (canStart ? 'success' : '')}
        disabled={!canStart || scheduledCount===0} onClick={onStart}
        style={{height:40, minWidth:96}}>
        <Icon name="play" size={13} color="currentColor"/> 启动检测
      </button>
      <button className={'btn ' + (canPause ? 'warn' : '')}
        disabled={!canPause} onClick={onPause}
        style={{height:40, minWidth:68}}>
        <Icon name="pause" size={11} color="currentColor"/> 暂停
      </button>
    </div>
  );
};

window.BottomBar = BottomBar;
