
#include "gpu_monitor.h"

#include <iostream>

void GPUMonitor::Init(Napi::Env env, Napi::Object exports){
   Napi::Function func =
            DefineClass(env,
                        "GPUMonitor",
                        {
                          InstanceMethod("isSupported", &GPUMonitor::isSupported),
                          InstanceMethod("getDeviceCount", &GPUMonitor::getDeviceCount),
                          InstanceMethod("getMemory", &GPUMonitor::getMemory),
                          InstanceMethod("getMemoryUsed", &GPUMonitor::getMemoryUsed),
                          InstanceMethod("getGPUUsage", &GPUMonitor::getGPUUsage),
                          InstanceMethod("getDeviceName", &GPUMonitor::getDeviceName),
                          InstanceMethod("getTemperature", &GPUMonitor::getTemperature),
                          InstanceMethod("getEncoderUtilization", &GPUMonitor::getEncoderUtilization),
                          InstanceMethod("getDecoderUtilization", &GPUMonitor::getDecoderUtilization),
                          InstanceMethod("close", &GPUMonitor::close),
                        });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("GPUMonitor", func);
}


GPUMonitor::GPUMonitor(const Napi::CallbackInfo& info) : Napi::ObjectWrap<GPUMonitor>(info){
  #ifdef GPU_ENABLED
    nvmlReturn_t result;
    
    result = nvmlInit_v2();
    
    if (NVML_SUCCESS != result)
    {
        std::cout << "Failed to init: " << nvmlErrorString(result) << std::endl;
    }

    result = nvmlDeviceGetCount_v2(&deviceCount);

    if (NVML_SUCCESS != result)
    {
        std::cout << "Failed to get count: " << nvmlErrorString(result) << std::endl;
    }
  #endif
}

Napi::Value GPUMonitor::getGPUUsage(const Napi::CallbackInfo& info){
  auto env = info.Env();

  #ifdef GPU_ENABLED

  if(info.Length() < 0 || !info[0].IsNumber()){
    return Napi::Number::New(env, 0);
  }
  uint32_t index = info[0].ToNumber().Uint32Value();

  // std::cout << index << std::endl;
  if (index >= 0 &&  index <= deviceCount - 1){

    nvmlReturn_t result;
    nvmlDevice_t device;
    result = nvmlDeviceGetHandleByIndex(index, &device);
    //TODO: check result
    nvmlUtilization_t utilization;
    result = nvmlDeviceGetUtilizationRates(device, &utilization);
    return Napi::Number::New(env, utilization.gpu);
  }    
  #endif

  return Napi::Number::New(env, 0);
}
//in bytes
Napi::Value GPUMonitor::getMemoryUsed(const Napi::CallbackInfo& info){
  auto env = info.Env();

  #ifdef GPU_ENABLED

  if(info.Length() < 0 || !info[0].IsNumber()){
    return Napi::Number::New(env, 0);
  }
  uint32_t index = info[0].ToNumber().Uint32Value();

  // std::cout << index << std::endl;
  if (index >= 0 &&  index <= deviceCount - 1){

    nvmlReturn_t result;
    nvmlDevice_t device;
    result = nvmlDeviceGetHandleByIndex(index, &device);
    //TODO: check result
    nvmlMemory_t memory;
    nvmlDeviceGetMemoryInfo(device, &memory);

    return Napi::Number::New(env,memory.used);
  }    
  #endif

  return Napi::Number::New(env, 0);
  
}

Napi::Value GPUMonitor::getEncoderUtilization(const Napi::CallbackInfo& info){
  auto env = info.Env();

  #ifdef GPU_ENABLED

  if(info.Length() < 0 || !info[0].IsNumber()){
    return Napi::Number::New(env, 0);
  }
  uint32_t index = info[0].ToNumber().Uint32Value();

  // std::cout << index << std::endl;
  if (index >= 0 &&  index <= deviceCount - 1){

    nvmlReturn_t result;
    nvmlDevice_t device;
    result = nvmlDeviceGetHandleByIndex(index, &device);

    uint utilization = 0;
    uint samplingPeriodUs = 0;
    //TODO(CC): check result
    nvmlDeviceGetEncoderUtilization(device, &utilization, &samplingPeriodUs);

    return  Napi::Number::New(env, utilization);
  }    
  #endif

  return Napi::Number::New(env, 0);
  
}

Napi::Value GPUMonitor::getDecoderUtilization(const Napi::CallbackInfo& info){
  auto env = info.Env();

  #ifdef GPU_ENABLED

  if(info.Length() < 0 || !info[0].IsNumber()){
    return Napi::Number::New(env, 0);
  }
  uint32_t index = info[0].ToNumber().Uint32Value();

  // std::cout << index << std::endl;
  if (index >= 0 &&  index <= deviceCount - 1){

    nvmlReturn_t result;
    nvmlDevice_t device;
    result = nvmlDeviceGetHandleByIndex(index, &device);

    uint utilization = 0;
    uint samplingPeriodUs = 0;
    //TODO(CC): check result
    nvmlDeviceGetDecoderUtilization(device, &utilization, &samplingPeriodUs);
    
    return  Napi::Number::New(env, utilization);
  }    
  #endif

  return Napi::Number::New(env, 0);
  
}


Napi::Value GPUMonitor::getMemory(const Napi::CallbackInfo& info){
  auto env = info.Env();

  #ifdef GPU_ENABLED

  if(info.Length() < 0 || !info[0].IsNumber()){
    return Napi::Number::New(env, 0);
  }
  uint32_t index = info[0].ToNumber().Uint32Value();

  // std::cout << index << std::endl;
  if (index >= 0 &&  index <= deviceCount - 1){

    nvmlReturn_t result;
    nvmlDevice_t device;
    result = nvmlDeviceGetHandleByIndex(index, &device);
    //TODO: check result
    nvmlMemory_t memory;
    nvmlDeviceGetMemoryInfo(device, &memory);

    return Napi::Number::New(env,memory.total);
  }
  #endif

  return Napi::Number::New(env, 0);

}

Napi::Value GPUMonitor::isSupported(const Napi::CallbackInfo& info){
  #ifdef GPU_ENABLED
    return Napi::Boolean::New(info.Env(),true);
  #else
    return Napi::Boolean::New(info.Env(),false);
  #endif
}

Napi::Value GPUMonitor::getDeviceCount(const Napi::CallbackInfo& info){
  return Napi::Number::New(info.Env(),deviceCount);
}

Napi::Value GPUMonitor::close(const Napi::CallbackInfo& info){
  #ifdef GPU_ENABLED
  nvmlReturn_t result = nvmlShutdown();
  if (NVML_SUCCESS != result)
  {
      std::cout << "Failed to close: " << nvmlErrorString(result) << std::endl;
  }
  #endif
  return info.Env().Undefined();
}

Napi::Value GPUMonitor::getDeviceName(const Napi::CallbackInfo& info){
  auto env = info.Env();

  #ifdef GPU_ENABLED

  if(info.Length() < 0 || !info[0].IsNumber()){
    return Napi::Number::New(env, 0);
  }
  uint32_t index = info[0].ToNumber().Uint32Value();

  // std::cout << index << std::endl;
  if (index >= 0 &&  index <= deviceCount - 1){

    nvmlReturn_t result;
    nvmlDevice_t device;
    result = nvmlDeviceGetHandleByIndex(index, &device);
    //TODO: check result
    char name[NVML_DEVICE_NAME_BUFFER_SIZE];
    result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);

    return Napi::String::New(env, name);
  }    
  #endif

  return env.Null();

}

Napi::Value GPUMonitor::getTemperature(const Napi::CallbackInfo& info){
  auto env = info.Env();

  #ifdef GPU_ENABLED

  if(info.Length() < 0 || !info[0].IsNumber()){
    return Napi::Number::New(env, 0);
  }
  uint32_t index = info[0].ToNumber().Uint32Value();

  // std::cout << index << std::endl;
  if (index >= 0 &&  index <= deviceCount - 1){

    nvmlReturn_t result;
    nvmlDevice_t device;
    result = nvmlDeviceGetHandleByIndex(index, &device);
    //TODO: check result
    uint32_t temperature;
    nvmlDeviceGetTemperature(device,NVML_TEMPERATURE_GPU,&temperature);

    return Napi::Number::New(env, temperature);
  }    
  #endif

  return  Napi::Number::New(env, 0);
}
