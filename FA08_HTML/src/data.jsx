// Seed data & constants
const ASSAYS = [
  { code: 'TnI',    name: '肌钙蛋白I',   unit: 'ng/mL', ref: '0 ~ 0.04',   dilution: 'D1' },
  { code: 'Myo',    name: '肌红蛋白',    unit: 'ng/mL', ref: '0 ~ 72',     dilution: 'D1' },
  { code: 'CKMB',   name: '肌酸激酶MB',  unit: 'ng/mL', ref: '0 ~ 5.0',    dilution: 'D1' },
  { code: 'NTproBNP',name: 'NT-proBNP',  unit: 'pg/mL', ref: '0 ~ 125',    dilution: 'D2' },
  { code: 'Ddimer', name: 'D-二聚体',    unit: 'mg/L',  ref: '0 ~ 0.5',    dilution: 'D2' },
  { code: 'CRP',    name: 'C反应蛋白',   unit: 'mg/L',  ref: '0 ~ 10',     dilution: 'D3' },
];

const SAMPLE_TYPES = ['全血', '血清', '血浆', '尿液'];
const GENDERS = ['男', '女'];
const AGE_UNITS = ['岁', '月', '天'];

// 8 slots seeded
const INITIAL_SLOTS = [
  // status: empty | scheduled | running | done | error
  { id:'S1', status:'scheduled', sampleId:'20260418001', name:'张伟',  gender:'男', age:56, ageUnit:'岁', type:'血清', assays:['TnI','Myo','CKMB'] },
  { id:'S2', status:'scheduled', sampleId:'20260418002', name:'李娜',  gender:'女', age:42, ageUnit:'岁', type:'血清', assays:['NTproBNP'] },
  { id:'S3', status:'done',      sampleId:'20260418003', name:'王强',  gender:'男', age:63, ageUnit:'岁', type:'血浆', assays:['Ddimer','CRP'] },
  { id:'S4', status:'empty' },
  { id:'S5', status:'scheduled', sampleId:'20260418004', name:'陈敏',  gender:'女', age:29, ageUnit:'岁', type:'血清', assays:['CRP'] },
  { id:'S6', status:'empty' },
  { id:'S7', status:'error',     sampleId:'20260418005', name:'赵磊',  gender:'男', age:71, ageUnit:'岁', type:'全血', assays:['CRP'] },
  { id:'S8', status:'empty' },
];

// Consumables
const INITIAL_CONSUMABLES = {
  tips:     { name:'TIP吸头',   qty: 52,  capacity: 96, lotNo:'TIP-2603-A', expire:'2026-11' },
  plate:    { name:'深孔板',    qty: 36,  capacity: 96, lotNo:'PLT-2603-B', expire:'2026-09' },
  dilutions: [
    { slot:'D1', name:'SAA',  qty: 18, capacity: 50, lotNo:'SAA-2602-11', expire:'2026-08' },
    { slot:'D2', name:'PCT',  qty: 8,  capacity: 50, lotNo:'PCT-2602-03', expire:'2026-07' },
    { slot:'D3', name:'hsCRP',qty: 0,  capacity: 50, lotNo:'—', expire:'—', loaded:false },
  ],
};

// Reagent cards (inventory)
const INITIAL_CARDS = [
  { code:'TnI',    lot:'A2603-01', expire:'2026-09', remaining: 22 },
  { code:'Myo',    lot:'A2603-02', expire:'2026-09', remaining: 18 },
  { code:'CKMB',   lot:'A2603-05', expire:'2026-10', remaining: 14 },
  { code:'NTproBNP',lot:'B2602-11', expire:'2026-08', remaining: 9 },
  { code:'Ddimer', lot:'B2602-12', expire:'2026-08', remaining: 11 },
  { code:'CRP',    lot:'C2603-20', expire:'2026-11', remaining: 30 },
  { code:'PCT',    lot:'C2603-21', expire:'2026-11', remaining: 7 },
  { code:'hsCRP',  lot:'C2603-22', expire:'2026-11', remaining: 16 },
];

// Past results (for Results tab)
function seedResults() {
  const samples = [
    { id:'R001', sampleId:'20260418100', name:'刘洋',  gender:'男', age:58, type:'血清', time:'2026-04-18 09:12',
      items: [
        { code:'TnI',  value: 0.18, unit:'ng/mL', ref:'0 ~ 0.04', concl:'阳性', cv:'2.1%' },
        { code:'Myo',  value: 112,  unit:'ng/mL', ref:'0 ~ 72',   concl:'阳性', cv:'1.8%' },
        { code:'CKMB', value: 8.6,  unit:'ng/mL', ref:'0 ~ 5.0',  concl:'阳性', cv:'2.4%' },
      ]},
    { id:'R002', sampleId:'20260418101', name:'孙悦',  gender:'女', age:34, type:'血清', time:'2026-04-18 09:38',
      items: [
        { code:'NTproBNP', value: 78, unit:'pg/mL', ref:'0 ~ 125', concl:'阴性', cv:'2.0%' },
      ]},
    { id:'R003', sampleId:'20260418102', name:'周浩',  gender:'男', age:67, type:'血浆', time:'2026-04-18 10:02',
      items: [
        { code:'Ddimer', value: 1.8, unit:'mg/L', ref:'0 ~ 0.5', concl:'阳性', cv:'2.5%' },
        { code:'CRP',    value: 42,  unit:'mg/L', ref:'0 ~ 10',  concl:'阳性', cv:'1.6%' },
      ]},
    { id:'R004', sampleId:'20260418103', name:'钱静',  gender:'女', age:45, type:'血清', time:'2026-04-18 10:41',
      items: [
        { code:'hsCRP', value: 2.1, unit:'mg/L', ref:'0 ~ 3.0', concl:'阴性', cv:'2.2%' },
      ]},
    { id:'R005', sampleId:'20260418104', name:'吴迪',  gender:'男', age:52, type:'血清', time:'2026-04-18 11:15',
      items: [
        { code:'PCT', value: 0.36, unit:'ng/mL', ref:'0 ~ 0.05', concl:'阳性', cv:'2.8%' },
        { code:'CRP', value: 68,   unit:'mg/L',  ref:'0 ~ 10',   concl:'阳性', cv:'1.5%' },
      ]},
  ];
  return samples;
}

// Initial alerts
const INITIAL_ALERTS = [
  { id:1, level:'warn',  time:'2026-04-18 08:42', code:'W-0312', msg:'稀释液D2余量低 (< 10)', handled:false },
  { id:2, level:'info',  time:'2026-04-18 08:46', code:'I-0015', msg:'系统自检通过', handled:true },
];

// Status helpers
const STATUS_MAP = {
  empty:     { label:'空位',   cls:'idle' },
  scheduled: { label:'已编排', cls:'run'  },
  running:   { label:'检测中', cls:'run'  },
  done:      { label:'已完成', cls:'ok'   },
  error:     { label:'异常',   cls:'err'  },
};

Object.assign(window, {
  ASSAYS, SAMPLE_TYPES, GENDERS, AGE_UNITS,
  INITIAL_SLOTS, INITIAL_CONSUMABLES, INITIAL_CARDS,
  INITIAL_ALERTS, STATUS_MAP, seedResults,
});
