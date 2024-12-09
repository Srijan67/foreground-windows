#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <time.h>
#define MAX_TITLE_LENGTH 256

void GetForegroundWindowDetails(char* details, size_t size) {
    HWND hwnd = GetForegroundWindow();
    if (hwnd) {
        char title[MAX_TITLE_LENGTH];
        DWORD processID;
        GetWindowTextA(hwnd, title, sizeof(title));
        GetWindowThreadProcessId(hwnd, &processID);
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID); 
        char processName[MAX_PATH] = ""; 
        if (hProcess) { 
            HMODULE hMod; 
            DWORD cbNeeded; 
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) { 
                GetModuleBaseNameA(hProcess, hMod, processName, sizeof(processName) / sizeof(char)); 
                } 
                CloseHandle(hProcess); 
            }
        snprintf(details, size, "Foreground Window Title: %s\nProcess ID: %lu\nProcess Name: %s\n", title, processID, processName); 
        printf("%s", details);
        fflush(stdout);

    }

}

void CALLBACK EventProc(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    if (event == EVENT_SYSTEM_FOREGROUND) {
        char details[512] = { 0 };
        GetForegroundWindowDetails(details, sizeof(details)); 

    }
}
void CALLBACK EventProcDetail(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    if ((event == EVENT_SYSTEM_FOREGROUND && idObject == OBJID_WINDOW) ||
        (event == EVENT_OBJECT_NAMECHANGE && idObject == OBJID_WINDOW)) {
        char details[512] = { 0 };
        GetForegroundWindowDetails(details, sizeof(details));
    }
}

int main(int argc, char* argv[]) {
 if (argc > 1 && strcmp(argv[1], "detail-event") == 0) {
      HWINEVENTHOOK hookForeground = SetWinEventHook(
      EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
      NULL, EventProcDetail, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
  );

  HWINEVENTHOOK hookNameChange = SetWinEventHook(
      EVENT_OBJECT_NAMECHANGE, EVENT_OBJECT_NAMECHANGE,
      NULL, EventProcDetail, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
  );

  if (hookForeground == NULL || hookNameChange == NULL) {
      printf("Failed to set event hook(s).\n");
      return 1;
  }

  printf("Monitoring foreground window and title changes... Press Ctrl+C to exit.\n");
  fflush(stdout);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
  }

  UnhookWinEvent(hookForeground);
  UnhookWinEvent(hookNameChange);
  return 0;
 }
 else if (argc > 1 && strcmp(argv[1], "event") == 0) {
    HWINEVENTHOOK hook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        NULL, EventProc, 0, 0, WINEVENT_OUTOFCONTEXT
    );

    if (hook == NULL) {
        printf("Failed to set event hook.\n");
        return 1;
    }

    printf("Monitoring foreground window changes... Press Ctrl+C to exit.\n");
    fflush(stdout);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWinEvent(hook);
    return 0;
 }else {
    char details[512] = { 0 }; 
    GetForegroundWindowDetails(details, sizeof(details));
 }
}
