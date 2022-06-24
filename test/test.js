const GPUMonitor = require('..');

const gpu = new GPUMonitor();


console.log(`Gpu: `,gpu.isSupported());
console.log('Device count: ',gpu.getDeviceCount());

let ii = 0;
const loop = 10;
const timer = setInterval(function(){
  for (let index = 0; index < gpu.getDeviceCount(); index++) {
    console.log('------------');
    console.log(`Device: ${index} ${gpu.getDeviceName(index)}`);
    console.log('Mem: ',gpu.getMemory(index));
    console.log('MemUsed: ',gpu.getMemoryUsed(index));
    console.log('GPU Usage: ',gpu.getGPUUsage(index));
    console.log('Enc: ',gpu.getEncoderUtilization(index));
    console.log('Dec: ',gpu.getDecoderUtilization(index));
    console.log('Temperatur: ',gpu.getTemperature(index));//degrees C
  }
  if(ii++ == loop){
    clearInterval(timer);
    gpu.close();
  } 
},2000);
