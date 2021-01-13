#ifndef GPU_MONITOR_H_
#define GPU_MONITOR_H_

#include <napi.h>

#ifdef GPU_ENABLED
#include <nvml.h>
#endif

class GPUMonitor: public Napi::ObjectWrap<GPUMonitor>{
public:
  static void Init(Napi::Env env, Napi::Object exports);
  GPUMonitor(const Napi::CallbackInfo& info);
private:
  Napi::Value isSupported(const Napi::CallbackInfo& info);
  Napi::Value getDeviceCount(const Napi::CallbackInfo& info);
  Napi::Value getMemoryUsed(const Napi::CallbackInfo& info);
  Napi::Value getMemory(const Napi::CallbackInfo& info);
  Napi::Value getGPUUsage(const Napi::CallbackInfo& info);
  Napi::Value getDeviceName(const Napi::CallbackInfo& info);
  Napi::Value getTemperature(const Napi::CallbackInfo& info);
  Napi::Value close(const Napi::CallbackInfo& info);


  uint32_t deviceCount = 0;
};


#endif  //GPU_MONITOR_H_
