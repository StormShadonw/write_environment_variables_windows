// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <string>

// Función para obtener el valor actual de una cadena en el registro
std::wstring GetRegistryStringValue(HKEY hKey, LPCWSTR subKey, LPCWSTR valueName) {
    HKEY hSubKey;
    if (RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
        DWORD bufferSize = 0;
        if (RegQueryValueEx(hSubKey, valueName, NULL, NULL, NULL, &bufferSize) == ERROR_SUCCESS) {
            wchar_t* buffer = new wchar_t[bufferSize / sizeof(wchar_t)];
            if (RegQueryValueEx(hSubKey, valueName, NULL, NULL, reinterpret_cast<LPBYTE>(buffer), &bufferSize) == ERROR_SUCCESS) {
                std::wstring value(buffer);
                delete[] buffer;
                RegCloseKey(hSubKey);
                return value;
            }
            delete[] buffer;
        }
        RegCloseKey(hSubKey);
    }
    return L"";
}

// Función para establecer el valor de una cadena en el registro
bool SetRegistryStringValue(HKEY hKey, LPCWSTR subKey, LPCWSTR valueName, const std::wstring& newValue) {
    HKEY hSubKey;
    if (RegOpenKeyEx(hKey, subKey, 0, KEY_SET_VALUE, &hSubKey) == ERROR_SUCCESS) {
        if (RegSetValueEx(hSubKey, valueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(newValue.c_str()), (newValue.length() + 1) * sizeof(wchar_t)) == ERROR_SUCCESS) {
            RegCloseKey(hSubKey);
            return true;
        }
        RegCloseKey(hSubKey);
    }
    return false;
}

int main() {
    LPCWSTR subKey = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
    LPCWSTR valueName = L"PATH";

    // Obtener el valor actual del registro
    std::wstring currentValue = GetRegistryStringValue(HKEY_LOCAL_MACHINE, subKey, valueName);
    std::wcout << L"Valor actual: " << currentValue << std::endl;

    // Agregar más datos al valor actual
    std::wstring newData = L"C:\\Users\\luisj\\Desktop\\Jorge\\test\\test2";
    std::wstring newData1 = L"\\flutter-master\\bin";
    std::wstring updatedValue = currentValue + L";" + newData + newData1; // Aquí se está concatenando el valor actual con los nuevos datos

    // Establecer el nuevo valor en el registro
    if (SetRegistryStringValue(HKEY_LOCAL_MACHINE, subKey, valueName, updatedValue)) {
        std::wcout << L"Valor actualizado correctamente." << std::endl;
    }
    else {
        std::wcerr << L"Error al actualizar el valor." << std::endl;
    }

    return 0;
}