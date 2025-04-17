#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <cstring>  // Para wcstombs_s

DWORD ObtenerPID(const std::string& nombreProceso) {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 entry = { 0 };
    entry.dwSize = sizeof(entry);

    if (Process32First(snapshot, &entry)) {
        do {
            // Convertimos el nombre del proceso de WCHAR a char* usando wcstombs_s
            char nombreProcesoChar[260];
            size_t outSize;
            wcstombs_s(&outSize, nombreProcesoChar, sizeof(nombreProcesoChar), entry.szExeFile, _TRUNCATE);

            // Comparamos el nombre del proceso en formato char*
            if (_stricmp(nombreProcesoChar, nombreProceso.c_str()) == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}

bool VerificarProcesoEnEjecucion(DWORD pid) {
    // Verificar si el proceso está en ejecución mediante OpenProcess
    HANDLE hProceso = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProceso == NULL) {
        // Si no se puede abrir el proceso, significa que no está en ejecución
        return false;
    }

    // Si OpenProcess tiene éxito, el proceso está en ejecución
    CloseHandle(hProceso);
    return true;
}

void OcultarConsola() {
    HWND consola = GetConsoleWindow();
    ShowWindow(consola, SW_HIDE); // Ocultar la ventana de la consola
}

int main() {
    // Ocultar la ventana de la consola
    OcultarConsola();

    const std::string proceso = "reto1.exe"; // Nombre del proceso que estamos buscando
    const DWORD direccion_virtual = 0x00D357B7; // Dirección en memoria donde está la instrucción JZ
    const BYTE nuevo_byte = 0x75; // JNZ (lo que queremos poner)

    DWORD pid = ObtenerPID(proceso); // Obtener el PID del proceso
    if (pid == 0) {
        return 1; // Si no se encuentra el proceso, termina sin hacer nada
    }

    // Verificar si el proceso está en ejecución
    if (!VerificarProcesoEnEjecucion(pid)) {
        return 1; // Si el proceso no está en ejecución, termina sin hacer nada
    }

    // Abre el proceso con permisos de lectura y escritura
    HANDLE hProceso = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProceso) {
        return 1; // Si no se puede abrir el proceso, termina
    }

    // Escribir el nuevo byte en la dirección especificada
    if (!WriteProcessMemory(hProceso, (LPVOID)direccion_virtual, &nuevo_byte, sizeof(nuevo_byte), nullptr)) {
        CloseHandle(hProceso);
        return 1; // Si no se puede escribir en la memoria, termina
    }

    // Aquí ya hemos aplicado el parche
    // Esperar 1 segundo antes de cerrar para que el proceso termine
    Sleep(1000); // Pausa de 1 segundo (solo para ver que se hizo algo si fuera necesario)

    // Cerrar el handle del proceso
    CloseHandle(hProceso);
    return 0;
}
