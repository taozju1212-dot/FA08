// Sample list — table with row-click selection
const SampleList = ({ slots, selected, onSelect, onSelectAll, onEdit, onDelete, filterSlot }) => {
  const visible = slots.filter(s => s.status !== 'empty');
  const allSelected = visible.length > 0 && visible.every(s => selected.includes(s.id));

  return (
    <div style={{height:'100%', display:'flex', flexDirection:'column', background:'#fff'}}>
      <div style={{
        padding:'8px 12px', borderBottom:'1px solid var(--divider)',
        background:'var(--surface-alt)',
        display:'flex', alignItems:'center', justifyContent:'space-between',
      }}>
        <div style={{fontSize:13, fontWeight:600}}>
          <Icon name="flask-small" size={13}/> 样本编排列表
          <span style={{marginLeft:10, fontSize:11, color:'var(--ink-500)', fontWeight:400}}>
            共 <b style={{color:'var(--ink-800)'}}>{visible.length}</b> 个样本
            {selected.length>0 && <span> · 已选 <b style={{color:'var(--blue-700)'}}>{selected.length}</b></span>}
          </span>
        </div>
        <div style={{display:'flex', gap:6}}>
          <button className="btn sm" disabled={selected.length!==1} onClick={onEdit}>
            <Icon name="edit" size={11}/> 编辑
          </button>
          <button className="btn sm" disabled={selected.length===0} onClick={onDelete}>
            <Icon name="trash" size={11}/> 删除{selected.length>0 && `(${selected.length})`}
          </button>
          <button className="btn sm primary" onClick={() => onSelectAll(!allSelected)}>
            {allSelected ? '取消全选' : '全选'}
          </button>
        </div>
      </div>

      {visible.length === 0 ? (
        <div style={{flex:1, display:'grid', placeItems:'center', color:'var(--ink-400)'}}>
          <div style={{textAlign:'center'}}>
            <Icon name="flask-small" size={36} color="#D6DEE6"/>
            <div style={{fontSize:13, marginTop:8}}>尚无编排的样本</div>
            <div style={{fontSize:11, marginTop:4}}>点击左侧样本位（空位）开始录入</div>
          </div>
        </div>
      ) : (
        <div className="scroll" style={{flex:1}}>
          <table className="tbl">
            <thead>
              <tr>
                <th style={{width:54}}>编号</th>
                <th style={{width:110}}>样本ID</th>
                <th style={{width:60}}>姓名</th>
                <th style={{width:44}}>性别</th>
                <th style={{width:44}}>类型</th>
                <th style={{width:52}}>位置</th>
                <th>检测项目</th>
                <th style={{width:72}}>状态</th>
              </tr>
            </thead>
            <tbody>
              {visible.map((s, i) => {
                const st = STATUS_MAP[s.status];
                const isSel = selected.includes(s.id);
                return (
                  <tr key={s.id} className={isSel?'selected':''}
                    style={{cursor:'pointer'}}
                    onClick={() => onSelect(s.id)}>
                    <td><span className="mono" style={{fontWeight:700, color:'var(--ink-700)'}}>{String(i+1).padStart(3,'0')}</span></td>
                    <td className="num">{s.sampleId}</td>
                    <td>{s.name}</td>
                    <td>{s.gender}</td>
                    <td>{s.type}</td>
                    <td><span className="mono" style={{fontWeight:700, color:'var(--blue-700)'}}>{s.id}</span></td>
                    <td style={{maxWidth:0}}>
                      <div style={{display:'flex', gap:3, flexWrap:'wrap'}}>
                        {s.assays?.map(code => (
                          <span key={code} style={{
                            fontSize:10, padding:'1px 5px',
                            background:'var(--blue-050)', color:'var(--blue-700)',
                            borderRadius:3, border:'1px solid var(--blue-100)',
                          }}>{code}</span>
                        ))}
                      </div>
                    </td>
                    <td><span className={'pill ' + st.cls}><span className="dot"/>{st.label}</span></td>
                  </tr>
                );
              })}
            </tbody>
          </table>
        </div>
      )}
    </div>
  );
};

window.SampleList = SampleList;
