// Top bar: module tabs + system state + clock + alarm
const useClock = () => {
  const [t, setT] = React.useState(() => new Date());
  React.useEffect(() => {
    const id = setInterval(() => setT(new Date()), 1000);
    return () => clearInterval(id);
  }, []);
  const p = n => String(n).padStart(2,'0');
  return `${t.getFullYear()}-${p(t.getMonth()+1)}-${p(t.getDate())} ${p(t.getHours())}:${p(t.getMinutes())}:${p(t.getSeconds())}`;
};

const TopBar = ({ tab, onTab, device, temp, alerts, onAlerts, user, message }) => {
  const clock = useClock();
  const unhandled = alerts.filter(a => !a.handled).length;

  const Tab = ({ id, label, disabled, icon }) => (
    <button
      disabled={disabled}
      onClick={() => !disabled && onTab(id)}
      style={{
        height:40, padding:'0 18px',
        border:'none', background: tab===id ? '#fff' : 'transparent',
        color: disabled ? '#A5B0BC' : (tab===id ? 'var(--blue-700)' : '#fff'),
        fontSize:14, fontWeight: tab===id ? 700 : 500,
        cursor: disabled ? 'not-allowed' : 'pointer',
        borderTopLeftRadius: 6, borderTopRightRadius: 6,
        display:'flex', alignItems:'center', gap:6,
        position:'relative',
        marginRight: 2,
      }}
    >
      <Icon name={icon} size={14}/>
      {label}
      {disabled && <span style={{fontSize:10, marginLeft:4, opacity:.7}}>（未开放）</span>}
    </button>
  );

  const deviceMap = {
    idle:    { label:'待机', cls:'idle', color:'#A5B0BC' },
    running: { label:'运行中', cls:'run', color:'#4CAF50' },
    paused:  { label:'暂停', cls:'warn', color:'#E8A33D' },
    error:   { label:'报警', cls:'err', color:'#E74C3C' },
  };
  const dev = deviceMap[device] || deviceMap.idle;

  return (
    <div style={{
      height: 64,
      background: 'linear-gradient(180deg, #0F4E85 0%, #1464A5 100%)',
      color:'#fff',
      display:'flex', alignItems:'flex-end',
      padding: '0 12px',
      boxShadow: '0 2px 8px rgba(0,0,0,.1)',
      position:'relative',
    }}>
      {/* Status cluster (top-right) */}
      <div style={{position:'absolute', right:10, top:6, display:'flex', alignItems:'center', gap:10, fontSize:11}}>
        <div style={{display:'flex', alignItems:'center', gap:4}}>
          <span style={{width:8, height:8, borderRadius:'50%', background: dev.color, boxShadow: device==='running' ? '0 0 6px '+dev.color : 'none'}}
            className={device==='running'?'pulse':''}/>
          <span style={{fontWeight:600}}>{dev.label}</span>
        </div>
        {message && (
          <>
            <span style={{opacity:.4}}>|</span>
            <div style={{
              display:'flex', alignItems:'center', gap:5,
              padding:'2px 8px', borderRadius:3,
              background:'rgba(255,255,255,.14)', maxWidth:180,
            }}>
              <span style={{width:5, height:5, borderRadius:'50%', background:'#8FD1A0'}} className="pulse"/>
              <span style={{
                fontSize:11, whiteSpace:'nowrap', overflow:'hidden', textOverflow:'ellipsis',
              }}>{message}</span>
            </div>
          </>
        )}
        <span style={{opacity:.4}}>|</span>
        <div style={{display:'flex', alignItems:'center', gap:4}}>
          <Icon name="therm" size={12}/>
          <span className="mono">{temp.toFixed(1)}°C</span>
        </div>
        <span style={{opacity:.4}}>|</span>
        <div
          onClick={onAlerts}
          style={{display:'flex', alignItems:'center', gap:4, cursor:'pointer', position:'relative', padding:'2px 6px', borderRadius:4, background: unhandled>0 ? 'rgba(231,76,60,.2)' : 'transparent'}}
          title="报警"
        >
          <Icon name="bell" size={12}/>
          <span>报警</span>
          {unhandled>0 && (
            <span style={{
              marginLeft:2, minWidth:16, height:16, borderRadius:8,
              background:'#E74C3C', color:'#fff', fontSize:10, fontWeight:700,
              padding:'0 5px', display:'inline-grid', placeItems:'center',
            }}>{unhandled}</span>
          )}
        </div>
        <span style={{opacity:.4}}>|</span>
        <Icon name="user" size={12}/><span>{user}</span>
        <span style={{opacity:.4}}>|</span>
        <span className="mono" style={{opacity:.95}}>{clock}</span>
      </div>

      {/* Tabs */}
      <div style={{display:'flex', marginLeft: 0}}>
        <Tab id="detect"      label="检测" icon="flask-small" />
        <Tab id="results"     label="结果" icon="chart" />
        <Tab id="consumables" label="耗材" icon="plate" />
        <Tab id="settings"    label="设置" icon="settings" disabled />
      </div>

      {/* Running stripe under bar */}
      {device === 'running' && (
        <div className="running-strip" style={{position:'absolute', left:0, right:0, bottom:-3}}/>
      )}
    </div>
  );
};

window.TopBar = TopBar;
