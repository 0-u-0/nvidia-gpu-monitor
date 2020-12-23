const GPUMonitor = require('..');

const gpu = new GPUMonitor();
console.log(gpu.isSupported());
console.log(gpu.getDeviceCount());

const index = 0;

console.log(gpu.getDeviceName(index));

let ii = 0;
const timer = setInterval(function(){
  console.log(gpu.getMemoryUsed(index));
  console.log(gpu.getGPUUsage(index));
  console.log(gpu.getTemperature(index));//degrees C
  if(ii++ == 3){
    clearInterval(timer);
    gpu.close();
  }
},2000);
