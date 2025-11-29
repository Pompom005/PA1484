#include "SMHIFileManager.h"

SMHIFileManager::SMHIFileManager() {
}

bool SMHIFileManager::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return false;
    }
    
    createFolder();
    return true;
}

bool SMHIFileManager::createFolder() {
    // In SPIFFS, folders don't really exist - we'll use filename prefixes
    // But we can create a marker file to represent the folder
    File folder = SPIFFS.open(fileFolder + "/.folder", "w");
    if (!folder) {
        return false;
    }
    folder.close();
    return true;
}

bool SMHIFileManager::fileExists(const String& filename) {
    String fullPath = fileFolder + "/" + filename;
    return SPIFFS.exists(fullPath);
}

void SMHIFileManager::listAllFiles() {
    Serial.println("=== Files in SMHIJsonFiles folder ===");
    
    File root = SPIFFS.open(fileFolder);
    if (!root) {
        Serial.println("Failed to open folder");
        return;
    }
    
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }
    
    File file = root.openNextFile();
    int fileCount = 0;
    
    while (file) {
        if (!file.isDirectory()) {
            String fileName = file.name();
            // Skip the .folder marker file
            if (!fileName.endsWith("/.folder")) {
                Serial.printf("File: %s, Size: %d bytes\n", fileName.c_str(), file.size());
                fileCount++;
            }
        }
        file = root.openNextFile();
    }
    
    if (fileCount == 0) {
        Serial.println("No files found in folder");
    }
}

std::vector<String> SMHIFileManager::getAvailableFiles() {
    std::vector<String> files;
    
    File root = SPIFFS.open(fileFolder);
    if (!root || !root.isDirectory()) {
        return files;
    }
    
    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String fileName = file.name();
            if (!fileName.endsWith("/.folder")) {
                // Extract just the filename without path
                int lastSlash = fileName.lastIndexOf('/');
                if (lastSlash != -1) {
                    fileName = fileName.substring(lastSlash + 1);
                }
                files.push_back(fileName);
            }
        }
        file = root.openNextFile();
    }
    
    return files;
}

bool SMHIFileManager::deleteFile(const String& filename) {
    String fullPath = fileFolder + "/" + filename;
    if (SPIFFS.exists(fullPath)) {
        return SPIFFS.remove(fullPath);
    }
    return false;
}

void SMHIFileManager::deleteAllFiles() {
    std::vector<String> files = getAvailableFiles();
    for (const String& file : files) {
        deleteFile(file);
    }
    Serial.printf("Deleted %d files\n", files.size());
}

void SMHIFileManager::printStorageInfo() {
    Serial.println("=== Storage Information ===");
    Serial.printf("Total space: %d bytes\n", SPIFFS.totalBytes());
    Serial.printf("Used space: %d bytes\n", SPIFFS.usedBytes());
    Serial.printf("Free space: %d bytes\n", SPIFFS.totalBytes() - SPIFFS.usedBytes());
}