/***************************************************************************/
#pragma once
/***************************************************************************/
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define STRICT
#include <windows.h>
/***************************************************************************/
#include <stdlib.h>
#include <system_error>

    /* Last WIN32 error, message */
inline std::wstring last_win32_error_message(int code = 0)
{
    std::error_code ec(
        (code ? code : ::GetLastError()
        ),
        std::system_category()
    );
    char buf_[0xFF]{};
    /* negative rezult on error */
    (void)std::snprintf(buf_, 0xFF, "%s", (ec.message().c_str()) );

    ::SetLastError(0); //yes this helps

    return std::wstring{ buf_, buf_ + 0xFF };
}
   // CAUTION! __wargv can be easily NULL
   // if wrong main is used
   // it has to be wmain() or 
   /// this is the right one for UNICODE win builds
// int WINAPI wWinMain(
//     _In_ HINSTANCE hInstance,
//     _In_opt_ HINSTANCE hPrevInstance,
//     _In_ LPWSTR lpCmdLine,
//     _In_ int nShowCmd
//     )

    inline wchar_t * app_full_path = __wargv[0];
/// ---------------------------------------------------------------------
    inline int mbox(const wchar_t* message) 
    {
        return MessageBox(NULL, message, app_full_path, MB_ICONEXCLAMATION);
    }

    inline int mbox(int last_error_code, const wchar_t * prompt = nullptr )
    {

        std::wstring message_ = last_win32_error_message(last_error_code);

        wchar_t buf_[1024]{};
        /* negative rezult on error */
        (void)::swprintf(buf_, 0xFF, 
            L"\nEror!\n\n%s\n\nCode: %d, \nMessage: %s\n", 
            ( prompt ? prompt : L""),
            last_error_code, 
            (message_.c_str())
        );

        return MessageBox(NULL, buf_ , app_full_path, MB_ICONEXCLAMATION);
    }

    inline auto dbj_terror(bool result, const wchar_t * expression)
    {
        if (result) return;
        mbox( ::GetLastError(), expression );
        exit( EXIT_FAILURE );
    };
 /**************************************************************************/