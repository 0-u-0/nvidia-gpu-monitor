#include <napi.h>
#include "gpu_monitor.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    GPUMonitor::Init(env, exports);
    return exports;
}

NODE_API_MODULE(addon, InitAll)