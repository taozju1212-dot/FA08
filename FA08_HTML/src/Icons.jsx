// Simple inline SVG icons — 16px grid
const Icon = ({ name, size = 16, color = 'currentColor', style }) => {
  const s = size;
  const stroke = { stroke: color, strokeWidth: 1.6, fill: 'none', strokeLinecap:'round', strokeLinejoin:'round' };
  const svgProps = { width: s, height: s, viewBox: '0 0 16 16', style };

  switch(name) {
    case 'play':
      return <svg {...svgProps}><path d="M4 3 L13 8 L4 13 Z" fill={color} stroke="none" /></svg>;
    case 'pause':
      return <svg {...svgProps}><rect x="4" y="3" width="3" height="10" fill={color} /><rect x="9" y="3" width="3" height="10" fill={color} /></svg>;
    case 'stop':
      return <svg {...svgProps}><rect x="3" y="3" width="10" height="10" fill={color} /></svg>;
    case 'alert':
      return <svg {...svgProps}><path d="M8 2 L14 13 L2 13 Z" {...stroke} /><path d="M8 6 L8 9.5" {...stroke}/><circle cx="8" cy="11.5" r=".8" fill={color}/></svg>;
    case 'bell':
      return <svg {...svgProps}><path d="M4 11 h8 v-1 c-.6 -.3 -1 -1 -1 -1.8 v-2.2 a3 3 0 0 0 -6 0 v2.2 c0 .8 -.4 1.5 -1 1.8 z" {...stroke}/><path d="M7 13 a1 1 0 0 0 2 0" {...stroke}/></svg>;
    case 'user':
      return <svg {...svgProps}><circle cx="8" cy="6" r="2.6" {...stroke}/><path d="M3 13 c1 -2.5 3 -3.6 5 -3.6 s4 1.1 5 3.6" {...stroke}/></svg>;
    case 'search':
      return <svg {...svgProps}><circle cx="7" cy="7" r="4" {...stroke}/><path d="M10 10 L13.5 13.5" {...stroke}/></svg>;
    case 'scan':
      return <svg {...svgProps}><path d="M3 6 V4 h2 M11 4 h2 v2 M3 10 v2 h2 M13 10 v2 h-2" {...stroke}/><path d="M2.5 8 H13.5" {...stroke}/></svg>;
    case 'add':
      return <svg {...svgProps}><path d="M8 3 V13 M3 8 H13" {...stroke}/></svg>;
    case 'minus':
      return <svg {...svgProps}><path d="M3 8 H13" {...stroke}/></svg>;
    case 'close':
      return <svg {...svgProps}><path d="M4 4 L12 12 M12 4 L4 12" {...stroke}/></svg>;
    case 'edit':
      return <svg {...svgProps}><path d="M3 13 L3 11 L10 4 L12 6 L5 13 Z" {...stroke}/></svg>;
    case 'trash':
      return <svg {...svgProps}><path d="M3 4 H13 M5 4 V3 H11 V4 M4 4 L5 13 H11 L12 4" {...stroke}/></svg>;
    case 'print':
      return <svg {...svgProps}><path d="M4 6 V3 H12 V6 M4 10 H3 V6 H13 V10 H12 M5 10 H11 V13 H5 Z" {...stroke}/></svg>;
    case 'export':
      return <svg {...svgProps}><path d="M3 13 H13 M8 3 V10 M5 7 L8 10 L11 7" {...stroke}/></svg>;
    case 'check':
      return <svg {...svgProps}><path d="M3 8.5 L6.5 12 L13 4.5" {...stroke} strokeWidth="2"/></svg>;
    case 'card':
      return <svg {...svgProps}><rect x="2" y="4" width="12" height="8" rx="1" {...stroke}/><path d="M2 7 H14" {...stroke}/></svg>;
    case 'tip':
      return <svg {...svgProps}><path d="M6 2 H10 V6 L8 13 L6 6 Z" {...stroke}/></svg>;
    case 'plate':
      return <svg {...svgProps}><rect x="2" y="3" width="12" height="10" rx="1" {...stroke}/><circle cx="5" cy="6" r=".7" fill={color}/><circle cx="8" cy="6" r=".7" fill={color}/><circle cx="11" cy="6" r=".7" fill={color}/><circle cx="5" cy="10" r=".7" fill={color}/><circle cx="8" cy="10" r=".7" fill={color}/><circle cx="11" cy="10" r=".7" fill={color}/></svg>;
    case 'flask':
      return <svg {...svgProps}><path d="M6 2 H10 V6 L13 12 a1 1 0 0 1 -1 1 H4 a1 1 0 0 1 -1 -1 L6 6 Z" {...stroke}/></svg>;
    case 'therm':
      return <svg {...svgProps}><path d="M8 2 a2 2 0 0 1 2 2 V9 a3 3 0 1 1 -4 0 V4 a2 2 0 0 1 2 -2 Z" {...stroke}/></svg>;
    case 'chart':
      return <svg {...svgProps}><path d="M2 12 L6 8 L9 10 L14 4" {...stroke}/></svg>;
    case 'settings':
      return <svg {...svgProps}><circle cx="8" cy="8" r="2" {...stroke}/><path d="M8 2 V4 M8 12 V14 M2 8 H4 M12 8 H14 M3.5 3.5 L5 5 M11 11 L12.5 12.5 M12.5 3.5 L11 5 M5 11 L3.5 12.5" {...stroke}/></svg>;
    case 'flask-small':
      return <svg {...svgProps}><path d="M7 2 H9 V5 L12 12 H4 L7 5 Z" {...stroke}/></svg>;
    case 'arrow-right':
      return <svg {...svgProps}><path d="M4 8 H12 M9 5 L12 8 L9 11" {...stroke}/></svg>;
    case 'chevron-down':
      return <svg {...svgProps}><path d="M4 6 L8 10 L12 6" {...stroke}/></svg>;
    case 'chevron-up':
      return <svg {...svgProps}><path d="M4 10 L8 6 L12 10" {...stroke}/></svg>;
    case 'chevron-right':
      return <svg {...svgProps}><path d="M6 4 L10 8 L6 12" {...stroke}/></svg>;
    case 'reset':
      return <svg {...svgProps}><path d="M3 8 a5 5 0 1 1 1.5 3.5" {...stroke}/><path d="M3 4 V8 H7" {...stroke}/></svg>;
    case 'refresh':
      return <svg {...svgProps}><path d="M3 8 a5 5 0 1 1 1.5 3.5 M3 4 V8 H7" {...stroke}/></svg>;
    case 'dot':
      return <svg {...svgProps}><circle cx="8" cy="8" r="3" fill={color}/></svg>;
    default: return null;
  }
};

window.Icon = Icon;
