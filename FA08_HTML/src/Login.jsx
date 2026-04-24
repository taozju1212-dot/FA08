// Login screen
const Login = ({ onLogin }) => {
  const [user, setUser] = React.useState('admin');
  const [pwd, setPwd]   = React.useState('123456');
  const [remember, setRemember] = React.useState(true);
  const [autoLogin, setAutoLogin] = React.useState(false);
  const [err, setErr] = React.useState('');

  const submit = () => {
    if (!user || !pwd) { setErr('请输入用户名和密码'); return; }
    onLogin({ user });
  };

  return (
    <div style={{
      position:'absolute', inset:0,
      background:'linear-gradient(135deg, #0B3C66 0%, #1464A5 60%, #2E86C1 100%)',
      display:'flex', alignItems:'center', justifyContent:'center',
    }}>
      {/* decorative watermark grid */}
      <svg width="800" height="600" style={{position:'absolute', inset:0, opacity:.08, pointerEvents:'none'}}>
        <defs><pattern id="g" width="40" height="40" patternUnits="userSpaceOnUse">
          <path d="M40 0 L0 0 L0 40" stroke="#fff" strokeWidth=".6" fill="none"/>
        </pattern></defs>
        <rect width="800" height="600" fill="url(#g)"/>
      </svg>

      {/* centered panel */}
      <div style={{
        width:340, background:'#fff', borderRadius:10, padding:'32px 32px 24px',
        boxShadow:'0 24px 60px rgba(0,0,0,.25)', position:'relative', zIndex:1,
      }}>
        <div style={{
          fontSize:20, fontWeight:600, color:'#0F1C2B',
          textAlign:'center', letterSpacing:1, marginBottom:28,
        }}>全自动荧光免疫分析仪</div>
        <div style={{fontSize:15, fontWeight:600, color:'#0F1C2B', marginBottom:18}}>用户登录</div>

        <div className="field" style={{marginBottom:12}}>
          <label>用户名</label>
          <input className="input" value={user} onChange={e => setUser(e.target.value)} />
        </div>
        <div className="field" style={{marginBottom:16}}>
          <label>密码</label>
          <input className="input" type="password" value={pwd} onChange={e => setPwd(e.target.value)} />
        </div>

        <div style={{display:'flex', gap:14, marginBottom:18, fontSize:12, color:'#334155'}}>
          <label style={{display:'flex', gap:6, alignItems:'center', cursor:'pointer'}}>
            <span className={'ck' + (remember?' on':'')} onClick={() => setRemember(!remember)}/>
            记住密码
          </label>
          <label style={{display:'flex', gap:6, alignItems:'center', cursor:'pointer'}}>
            <span className={'ck' + (autoLogin?' on':'')} onClick={() => setAutoLogin(!autoLogin)}/>
            自动登录
          </label>
        </div>

        {err && <div style={{color:'var(--err)', fontSize:12, marginBottom:10}}>{err}</div>}

        <button className="btn primary lg" style={{width:'100%'}} onClick={submit}>登 录</button>
      </div>
    </div>
  );
};

window.Login = Login;
