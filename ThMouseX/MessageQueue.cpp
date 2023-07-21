#include "framework.h"
#include <vector>
#include "imgui_impl_win32.h"

#include "../Common/macro.h"
#include "../Common/MinHook.h"
#include "../Common/Variables.h"
#include "../Common/Helper.h"
#include "../Common/NeoLua.h"
#include "../Common/CallbackStore.h"
#include "../Common/Log.h"
#include "Initialization.h"
#include "MessageQueue.h"

namespace minhook = common::minhook;
namespace neolua = common::neolua;
namespace helper = common::helper;
namespace callbackstore = common::callbackstore;
namespace note = common::log;

using namespace std;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define HandleMousePress(e, __ev, downAction, upAction) HandleMousePressImpl(e, __ev, downAction, upAction, __COUNTER__)
#define HandleMousePressImpl(e, __ev, downAction, upAction, unique) \
static bool MAKE_UNIQUE_VAR(unique) = false; \
if (e->message == __ev##DOWN && MAKE_UNIQUE_VAR(unique) == false) { \
    MAKE_UNIQUE_VAR(unique) = true; \
    downAction; \
} \
else if (e->message == __ev##UP && MAKE_UNIQUE_VAR(unique) == true) { \
    MAKE_UNIQUE_VAR(unique) = false; \
    upAction; \
}0

#define HandleKeyboardPress(e, __ev, action) HandleKeyboardPressImpl(e, __ev, action, __COUNTER__)
#define HandleKeyboardPressImpl(e, __ev, action, unique) \
static bool MAKE_UNIQUE_VAR(unique) = false; \
if (e->wParam == __ev && e->message == WM_KEYDOWN && MAKE_UNIQUE_VAR(unique) == false) { \
    MAKE_UNIQUE_VAR(unique) = true; \
    action; \
} \
else if (e->wParam == __ev && e->message == WM_KEYUP && MAKE_UNIQUE_VAR(unique) == true) { \
    MAKE_UNIQUE_VAR(unique) = false; \
}0

namespace core::messagequeue {
    UINT CLEAN_MANAGED_DATA;
    bool RegisterCleanManagedDataMessage() {
        CLEAN_MANAGED_DATA = RegisterWindowMessageA("CLEAN_MANAGED_DATA {6BF7C2B8-F245-4781-AA3C-467366CA3551}");
        if (CLEAN_MANAGED_DATA == 0) {
            MessageBoxA(NULL, "Failed to register CLEAN_MANAGED_DATA message.", "RegisterWindowMessage error", MB_OK | MB_ICONERROR);
            return false;
        }
        return true;
    }

    HCURSOR WINAPI _SetCursor(HCURSOR hCursor);
    decltype(&_SetCursor) OriSetCursor;
    int WINAPI _ShowCursor(BOOL bShow);
    decltype(&_ShowCursor) OriShowCursor;

    bool isCursorShow = true;
    auto hCursor = LoadCursorA(NULL, IDC_ARROW);

    HCURSOR WINAPI _SetCursor(HCURSOR hCursor) {
        return NULL;
    }

    int WINAPI _ShowCursor(BOOL bShow) {
        return bShow == TRUE ? 0 : -1;
    }

    bool cursorNormalized;
    int cursorVisibility;
    void ShowCursorEx(bool show) {
        if (show && cursorVisibility < 0)
            OriShowCursor(TRUE);
        else if (!show && cursorVisibility >= 0)
            OriShowCursor(FALSE);
    }

    void HideMousePointer() {
        OriSetCursor(NULL);
        ShowCursorEx(false);
        isCursorShow = false;
    }

    void ShowMousePointer() {
        OriSetCursor(hCursor);
        ShowCursorEx(true);
        isCursorShow = true;
    }

    void NormalizeCursor() {
        // Set cursor visibility to -1, reset cursor to a normal arrow,
        // to ensure that there is a visible mouse cursor on the game's config dialog
        while (OriShowCursor(TRUE) < 0);
        while (OriShowCursor(FALSE) >= 0);
        cursorVisibility = -1;
        ShowMousePointer();
    }

    LRESULT CALLBACK GetMsgProcW(int code, WPARAM wParam, LPARAM lParam) {
        if (code == HC_ACTION && g_hookApplied) {
            auto e = (PMSG)lParam;
            if (g_hFocusWindow) {
                HandleKeyboardPress(e, gs_toggleImGuiButton, { {
                    g_showImGui = !g_showImGui;
                    if (g_showImGui) {
                        g_inputEnabled = false;
                        ShowMousePointer();
                    }
                    else
                        HideMousePointer();
                    } }
                );
            }
            if (g_showImGui)
                ImGui_ImplWin32_WndProcHandler(e->hwnd, e->message, e->wParam, e->lParam);
            else {
                HandleMousePress(e, WM_LBUTTON, g_leftMousePressed = true, 0);
                HandleMousePress(e, WM_MBUTTON, g_midMousePressed = true, 0);
                HandleMousePress(e, WM_RBUTTON, 0, g_inputEnabled = !g_inputEnabled);
                HandleKeyboardPress(e, gs_toggleOsCursorButton, isCursorShow ? HideMousePointer() : ShowMousePointer());
            }
        }
        return CallNextHookEx(NULL, code, wParam, lParam);
    }

    LRESULT CALLBACK CallWndRetProcW(int code, WPARAM wParam, LPARAM lParam) {
        static auto initialized = false;
        if (!initialized) {
            initialized = true;
            core::Initialize();
        }
        if (code == HC_ACTION && g_hookApplied) {
            if (!cursorNormalized) {
                cursorNormalized = true;
                NormalizeCursor();
            }
            auto e = (PCWPRETSTRUCT)lParam;
            if (e->message == CLEAN_MANAGED_DATA) {
                neolua::Uninitialize();
            }
            else if (e->message == WM_SETCURSOR) {
                if (LOWORD(e->lParam) == HTCLIENT) {
                    if (isCursorShow)
                        ShowMousePointer();
                    else
                        HideMousePointer();
                }
                else {
                    ShowCursorEx(true);
                    DefWindowProcW(e->hwnd, e->message, e->wParam, e->lParam);
                }
            }
            else if (e->message == WM_SIZE) {
                if (e->wParam == SIZE_RESTORED) {
                    callbackstore::TriggerClearMeasurementFlagsCallbacks();
                }
            }
        }
        return CallNextHookEx(NULL, code, wParam, lParam);
    }

    bool CheckHookProcHandle(HHOOK handle) {
        if (handle != NULL)
            return true;
        helper::ReportLastError(APP_NAME ": SetWindowsHookEx Error");
        return false;
    }

    HHOOK GetMsgProcHandle;
    HHOOK CallWndRetProcHandle;

    bool InstallHooks() {
        GetMsgProcHandle = SetWindowsHookExW(WH_GETMESSAGE, GetMsgProcW, g_coreModule, NULL);
        if (!CheckHookProcHandle(GetMsgProcHandle))
            return false;
        CallWndRetProcHandle = SetWindowsHookExW(WH_CALLWNDPROCRET, CallWndRetProcW, g_coreModule, NULL);
        if (!CheckHookProcHandle(CallWndRetProcHandle))
            return false;
        return true;
    }

    void RemoveHooks() {
        DWORD _;
        auto broadcastFlags = SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG;
        // notify targets to clean up managed data, but managed DLLs/assemblies unfortunately cannot be unloaded.
        SendMessageTimeoutW(HWND_BROADCAST, CLEAN_MANAGED_DATA, 0, 0, broadcastFlags, 1000, &_);
        // unregister hooks.
        UnhookWindowsHookEx(GetMsgProcHandle);
        UnhookWindowsHookEx(CallWndRetProcHandle);
        // force all top-level windows to process a message, therefore force all processes to unload the DLL.
        SendMessageTimeoutW(HWND_BROADCAST, WM_NULL, 0, 0, broadcastFlags, 1000, &_);
    }

    void PostRenderCallback() {
        static bool callbackDone = false;
        if (cursorNormalized && !callbackDone) {
            callbackDone = true;
            HideMousePointer();
        }
    }

    void Initialize() {
        // Hide the mouse cursor when D3D is running, but only after cursor normalization
        callbackstore::RegisterPostRenderCallback(PostRenderCallback);
        minhook::CreateApiHook(vector<minhook::HookApiConfig>{
            { L"USER32.DLL", "SetCursor", & _SetCursor, (PVOID*)&OriSetCursor },
            { L"USER32.DLL", "ShowCursor", &_ShowCursor, (PVOID*)&OriShowCursor },
        });
    }
}