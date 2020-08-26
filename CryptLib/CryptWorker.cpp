#pragma comment(lib, "crypt32.lib")

#include <iostream>
#include <string>
#include <windows.h>
#include <Wincrypt.h>
#include "Base64.h"
using namespace std;

extern "C"
{
    //If we can take wstring instead and can use it from other C++ & .NET, it would be good
    //Output should be Base64 string that could be decrypted in calling C#
    __declspec(dllexport) void Encrypt(const wchar_t* str, wchar_t* output)
    {
        DATA_BLOB DataOut;
        DATA_BLOB DataIn;

        wstring input(str);

        char* inputBuf = new char[input.size() + 1];
        size_t temp = 0;
        wcstombs_s(&temp, inputBuf, input.size() + 1, input.c_str(), input.size());

        DataIn.cbData = strlen(inputBuf) + 1;
        DataIn.pbData = (BYTE*)inputBuf;


        CBase64 m_Base64;

        if (CryptProtectData(
            &DataIn,
            NULL,         // A description string. 
            NULL,                               // Optional entropy
                                                // not used.
            NULL,                               // Reserved.
            NULL,                      // Pass a PromptStruct.
            CRYPTPROTECT_LOCAL_MACHINE,
            &DataOut))
        {
            std::string res = m_Base64.Base64_Encode(DataOut.pbData, DataOut.cbData);

            std::wstring resWstr(res.begin(), res.end());

            int size = resWstr.size() * sizeof(wchar_t);
            wcscpy_s(output, size, resWstr.c_str());
        }
        else
        {
            DWORD error = GetLastError();
            throw std::exception("Error during data encryption occured", 1);
        }
    }

    __declspec(dllexport) void EncryptW(const std::wstring& str, std::wstring& output)
    {
        DATA_BLOB DataOut;
        DATA_BLOB DataIn;

        wstring input(str);

        char* inputBuf = new char[input.size() + 1];
        size_t temp = 0;
        wcstombs_s(&temp, inputBuf, input.size() + 1, input.c_str(), input.size());

        DataIn.cbData = strlen(inputBuf) + 1;
        DataIn.pbData = (BYTE*)inputBuf;


        CBase64 m_Base64;

        if (CryptProtectData(
            &DataIn,
            NULL,         // A description string. 
            NULL,                               // Optional entropy
                                                // not used.
            NULL,                               // Reserved.
            NULL,                      // Pass a PromptStruct.
            CRYPTPROTECT_LOCAL_MACHINE,
            &DataOut))
        {
            std::string res = m_Base64.Base64_Encode(DataOut.pbData, DataOut.cbData);

            output = std::wstring(res.begin(), res.end());
        }
        else
        {
            DWORD error = GetLastError();
            throw std::exception("Error during data encryption occured", 1);
        }
        //LocalFree(DataIn.pbData);
        //LocalFree(DataOut.pbData);
        delete[] inputBuf;
    }

    __declspec(dllexport) void Decrypt(const std::wstring& encryptedBase64, std::wstring& output)
    {
        //TODO using CryptUnprotectData
        CBase64 m_Base64;
        std::string s = m_Base64.Wstring_To_Utf8(encryptedBase64);
        //std::string s((const char*)&encryptedBase64[0], sizeof(wchar_t) / sizeof(char) * encryptedBase64.size());
        std::string nws = m_Base64.Base64_Decode(s);
        //std::wstring input((const wchar_t*)&nws[0], sizeof(char) / sizeof(wchar_t) * nws.size());
        std::wstring input = m_Base64.Utf8_To_Wstring(s);

        DATA_BLOB DataOut;
        DATA_BLOB DataIn;

        char* inputBuf = new char[encryptedBase64.size() + 1];
        size_t temp = 0;
        wcstombs_s(&temp, inputBuf, encryptedBase64.size() + 1, encryptedBase64.c_str(), encryptedBase64.size());

        DataIn.cbData = strlen(inputBuf) + 1;
        DataIn.pbData = (BYTE*)inputBuf;

        if (CryptUnprotectData(
            &DataIn,
            NULL,         // A description string. 
            NULL,                               // Optional entropy
                                                // not used.
            NULL,                               // Reserved.
            NULL,                      // Pass a PromptStruct.
            CRYPTPROTECT_LOCAL_MACHINE,
            &DataOut))
        {
            //std::string res(DataIn.pbData, DataIn.cbData);
            std::string res(reinterpret_cast<char const*>(DataIn.pbData), DataIn.cbData);


            output = std::wstring(res.begin(), res.end());
        }
        else
        {
            DWORD error = GetLastError();
            throw std::exception("Error during data encryption occured", 1);
        }
        //LocalFree(DataIn.pbData);
        //LocalFree(DataOut.pbData);
        delete[] inputBuf;

    }
}



