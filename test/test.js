const GPUMonitor = require('..');

const gpu = new GPUMonitor();
console.log(gpu.isSupported());
console.log(gpu.getDeviceCount());

const index = 1;
setInterval(function(){
  console.log(gpu.getMemoryUsed(index));
  console.log(gpu.getGPUUsage(index));
},2000);
