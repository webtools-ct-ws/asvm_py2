#include "avmplus.h"
#include <dlfcn.h>
#include <unistd.h>
#include <sys/reboot.h>

namespace avmplus {
    // Universal Bridge: The "Heart" of your Framework
    NativeID UniversalNativeCall(MethodEnv, uint32_t argc, AvmBox* argv) {
        if (argc < 2) return nullObjectAtom;
        
        Stringp libPath = avm_box_to_string(argv[0]);
        Stringp funcName = avm_box_to_string(argv[1]);

        void* handle = dlopen(libPath->toUTF8()->data(), RTLD_LAZY);
        if (!handle) return nullObjectAtom;

        typedef void (*eos_entry)(AvmBox*);
        eos_entry f = (eos_entry)dlsym(handle, funcName->toUTF8()->data());

        if (f) {
            f(&argv[2]); // Passes remaining AS3 args to the C++ plugin
        }
        return nullObjectAtom;
    }

    // Power Management for Settings.asp
    NativeID SysPowerAction(MethodEnv, uint32_t argc, AvmBox* argv) {
        int action = avm_box_to_int(argv[0]); 
        if (action == 1) reboot(RB_AUTOBOOT); // Reboot
        if (action == 2) reboot(RB_POWER_OFF); // Shutdown
        return nullObjectAtom;
    }
}