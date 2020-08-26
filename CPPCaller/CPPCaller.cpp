#include <windows.h>
#include <iostream>
#include <string>
#include <fcntl.h> //_O_WTEXT
#include <io.h>    //_setmode()

using namespace std;

typedef void(__cdecl* encryptFuncDef)(const std::wstring& str, std::wstring& output);
int main()
{
    _setmode(_fileno(stdout), _O_WTEXT); //needed for output
    _setmode(_fileno(stdin), _O_WTEXT); //needed for input

    HINSTANCE hGetProcIDDLL = LoadLibrary(L"CryptLib.dll");

    if (hGetProcIDDLL == NULL) {
        cout << "DLL could not be loaded. " << GetLastError() << endl;
        return 1;
    }

    encryptFuncDef encryptFunc = (encryptFuncDef)GetProcAddress(hGetProcIDDLL, "EncryptW");

    //!This needs to support unicode strings, to be able to work
    //with chinese, russian etc.
    //For example, '你好' or 'Привет'

    std::wstring strToEncrypt;
    wcout << "Enter string to encrypt:" << endl;
    //wcin >> strToEncrypt;
    std::getline(std::wcin, strToEncrypt);
    wcout << "Sstring to encrypt: " << strToEncrypt << endl;
    //Can we dynamically allocate buffer, so we don't specify 1024 as exact value?
    wchar_t encryptedRes[1024];
    std::wstring encryptOut;
    encryptFunc(strToEncrypt, encryptOut);

    std::wcout << L"Encrypted string - " << encryptOut.c_str();

    encryptFuncDef decryptFunc = (encryptFuncDef)GetProcAddress(hGetProcIDDLL, "Decrypt");
    std::wstring decryptOut;
    decryptFunc(encryptOut, decryptOut);
    std::wcout << L"Decrypted string - " << decryptOut.c_str();
    //TODO get decrypted output and display it

    return 0;
}