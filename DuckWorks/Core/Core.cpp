#include <windows.h>

BOOL APIENTRY DllMain(HMODULE, DWORD  ul_reason_for_call, LPVOID) 
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
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
