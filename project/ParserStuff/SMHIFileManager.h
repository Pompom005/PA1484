#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <vector>
#include <String>

class SMHIFileManager {
private:
    String fileFolder = "/SMHIJsonFiles";

public:
    SMHIFileManager();
    bool begin();
    
    // File operations
    bool fileExists(const String& filename);
    void listAllFiles();
    std::vector<String> getAvailableFiles();
    bool deleteFile(const String& filename);
    void deleteAllFiles();
    
    // Folder operations
    String getFolderPath() { return fileFolder; }
    bool createFolder();
    
    // Debug info
    void printStorageInfo();
};