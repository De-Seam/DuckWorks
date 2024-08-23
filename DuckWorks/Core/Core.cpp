// Core includes
#include <Core/Module/ModuleManager.h>
#include <Core/Module/CoreModule.h>

// Std includes

namespace Win32
{
#include <windows.h>
}

Win32::BOOL APIENTRY DllMain(Win32::HMODULE, Win32::DWORD  ul_reason_for_call, Win32::LPVOID)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        if (gModuleManager == nullptr)
        {
            gModuleManager = new ModuleManager;
            gModuleManager->CreateModule<CoreModule>();
        }
        // Code to execute when the DLL is loaded.
        break;
    case DLL_THREAD_ATTACH:
        // Code to execute when a new thread is created in the process.
        break;
    case DLL_THREAD_DETACH:
        // Code to execute when a thread exits cleanly.
        break;
    case DLL_PROCESS_DETACH:
        // Code to execute when the DLL is unloaded.
        break;
    }
    return TRUE;  // Return TRUE to indicate successful loading.
}