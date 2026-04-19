// Detect page: combines SlotPanel + (SampleList | SampleForm)
const DetectPage = ({
  slots, activeSlotId, onSelectSlot,
  selected, onToggleSelect, onSelectAll,
  view, setView,
  onSubmitForm, onCancelForm,
  onEdit, onDelete, onNew,
  onKeyboard, keyboardTarget,
  editing, nextSeq,
}) => {
  const activeSlot = slots.find(s => s.id === activeSlotId);

  return (
    <div style={{
      flex:1, display:'flex', gap:8, padding:8, minHeight:0, background:'var(--bg)',
    }}>
      <div style={{width: 150, flexShrink:0}}>
        <SlotPanel
          slots={slots}
          activeSlot={activeSlotId}
          onSelectSlot={onSelectSlot}
          selected={selected}
          onToggleSelect={onToggleSelect}
          onSelectAll={onSelectAll}
        />
      </div>
      <div style={{flex:1, minWidth:0, border:'1px solid var(--border)', borderRadius:6, overflow:'hidden'}}>
        {view === 'form' ? (
          <SampleForm
            slot={activeSlot}
            initial={editing}
            nextSeq={nextSeq}
            onSubmit={onSubmitForm}
            onCancel={onCancelForm}
            onKeyboard={onKeyboard}
            keyboardTarget={keyboardTarget}
          />
        ) : (
          <SampleList
            slots={slots}
            selected={selected}
            onSelect={onToggleSelect}
            onSelectAll={onSelectAll}
            onEdit={onEdit}
            onDelete={onDelete}
            onNew={onNew}
          />
        )}
      </div>
    </div>
  );
};

window.DetectPage = DetectPage;
