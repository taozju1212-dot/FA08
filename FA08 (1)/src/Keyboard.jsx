// Virtual on-screen keyboard — numeric + full QWERTY + Chinese-friendly (ASCII)
const KB_LAYOUTS = {
  num: [
    ['1','2','3'],
    ['4','5','6'],
    ['7','8','9'],
    ['-','0','.'],
  ],
  abc: [
    ['1','2','3','4','5','6','7','8','9','0'],
    ['q','w','e','r','t','y','u','i','o','p'],
    ['a','s','d','f','g','h','j','k','l'],
    ['shift','z','x','c','v','b','n','m','back'],
  ],
};

const Keyboard = ({ mode = 'abc', target, onInput, onClose, value }) => {
  const [shift, setShift] = React.useState(false);
  const layout = mode === 'num' ? KB_LAYOUTS.num : KB_LAYOUTS.abc;

  const press = (key) => {
    if (key === 'shift') { setShift(s => !s); return; }
    if (key === 'back')  { onInput('__back'); return; }
    if (key === 'space') { onInput(' '); return; }
    onInput(shift && key.length === 1 ? key.toUpperCase() : key);
  };

  // Position: fixed within 800x600 stage (bottom sheet)
  const isNum = mode === 'num';
  const w = isNum ? 260 : 640;
  const h = isNum ? 220 : 220;

  return (
    <div style={{
      position:'absolute',
      left: isNum ? 'auto' : (800 - w) / 2,
      right: isNum ? 10 : 'auto',
      bottom: 68,
      width: w,
      background:'#fff',
      border: '1px solid var(--border-strong)',
      borderRadius: 8,
      boxShadow: '0 8px 24px rgba(15,28,43,.25)',
      zIndex: 80,
      padding: 8,
    }}>
      <div style={{display:'flex', justifyContent:'space-between', alignItems:'center', padding:'2px 6px 6px'}}>
        <div style={{fontSize:11, color:'var(--ink-500)'}}>
          <Icon name="edit" size={11}/> 输入: <span className="mono" style={{color:'var(--ink-800)'}}>{value || '—'}</span>
        </div>
        <div style={{display:'flex', gap:4}}>
          <button className="btn sm" onClick={() => onInput('__clear')}>清空</button>
          <button className="btn sm" onClick={onClose}><Icon name="close" size={12}/> 关闭</button>
        </div>
      </div>
      <div style={{display:'flex', flexDirection:'column', gap:4}}>
        {layout.map((row, r) => (
          <div key={r} style={{display:'flex', gap:4, justifyContent:'center'}}>
            {row.map(k => {
              const isSpecial = k === 'shift' || k === 'back';
              return (
                <button
                  key={k}
                  onClick={() => press(k)}
                  style={{
                    flex: isSpecial ? '0 0 60px' : '1 1 0',
                    height: 36,
                    minWidth: 36,
                    border: '1px solid var(--border-strong)',
                    background: k === 'shift' && shift ? 'var(--blue-100)' : '#fff',
                    borderRadius: 4,
                    fontSize: 14,
                    fontFamily: 'var(--font-num)',
                    color: 'var(--ink-800)',
                    cursor: 'pointer',
                  }}
                >
                  {k === 'shift' ? '⇧' : k === 'back' ? '⌫' : (shift && k.length===1 ? k.toUpperCase() : k)}
                </button>
              );
            })}
          </div>
        ))}
        {!isNum && (
          <div style={{display:'flex', gap:4, justifyContent:'center'}}>
            <button onClick={() => press('space')} style={{flex:'1 1 0', height:36, border:'1px solid var(--border-strong)', borderRadius:4, background:'#fff', fontSize:12}}>空格</button>
            <button onClick={() => onInput('__enter')} style={{flex:'0 0 80px', height:36, border:'1px solid var(--blue-700)', borderRadius:4, background:'var(--blue-700)', color:'#fff', fontSize:12, fontWeight:600}}>确认</button>
          </div>
        )}
      </div>
    </div>
  );
};

window.Keyboard = Keyboard;
