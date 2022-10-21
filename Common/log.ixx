module;

#include "framework.h"
#include "macro.h"
#include <iostream>
#include <chrono>
#include <cstdio>
#include <string>
#include <comdef.h>
#include <wrl/client.h>

export module common.log;

import common.var;

using namespace std;
using namespace Microsoft::WRL;

tm& GetTimeNow() {
    auto now = time(nullptr);
    return *localtime(&now);
}

namespace common::log {
    export DLLEXPORT void OpenConsole() {
        if (AllocConsole() == FALSE)
            return;
#pragma warning(push)
#pragma warning(disable: 6031)
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
#pragma warning(pop)
        printf("Debugging Window:\n\n");
    }

    FILE* logFile;
    wstring logPath;
    export DLLEXPORT void ToFile(const char* _Format, ...) {
        va_list args;
        va_start(args, _Format);
        if (logFile == NULL) {
            if (logPath.size() == 0)
                logPath = wstring(g_currentModuleDirPath) + L"/log.txt";
            logFile = _wfsopen(logPath.c_str(), L"a+", _SH_DENYNO);
            if (logFile != NULL)
                setvbuf(logFile, NULL, _IONBF, 0);
        }
        if (logFile != NULL) {
            auto& now = GetTimeNow();
            fprintf(logFile, "[%02d/%02d/%02d %02d:%02d:%02d] ",
                now.tm_mday, now.tm_mon + 1, now.tm_year + 1900,
                now.tm_hour, now.tm_min, now.tm_sec);
            vfprintf(logFile, _Format, args);
            fprintf(logFile, "\n");
        }
        va_end(args);
    }

    export DLLEXPORT void HResultToFile(const char* message, HRESULT hResult) {
        ComPtr<IErrorInfo> errorInfo;
        GetErrorInfo(0, errorInfo.GetAddressOf());
        _com_error error(hResult, errorInfo.Get(), true);

        auto detail = error.ErrorMessage();

        auto description = error.Description();
        auto source = error.Source();

        const char* desStr = description;
        if (desStr == nullptr)
            desStr = "(None)";
        const char* srcStr = source;
        if (srcStr == nullptr)
            srcStr = "(None)";

        ToFile("%s: %s, Description: %s, Source: %s", message, detail, desStr, srcStr);
    }

    export DLLEXPORT void LastErrorToFile(const char* message) {
        _com_error error(GetLastError());

        auto detail = error.ErrorMessage();

        ToFile("%s: %s", message, detail);
    }

    export DLLEXPORT void ToConsole(const char* _Format, ...) {
        OpenConsole();
        va_list args;
        va_start(args, _Format);
        vprintf(_Format, args);
        printf("\n");
        va_end(args);
    }

    export DLLEXPORT void FpsToConsole() {
        using namespace chrono;
        static time_point<steady_clock> oldTime = high_resolution_clock::now();
        static int fps; fps++;

        if (duration_cast<seconds>(high_resolution_clock::now() - oldTime) >= seconds{1}) {
            oldTime = high_resolution_clock::now();
            ToConsole("FPS: %d", fps);
            fps = 0;
        }
    }
}
