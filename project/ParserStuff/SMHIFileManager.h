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
    bool file_exists(const String& filename);
    void list_all_files();
    std::vector<String> get_available_files();
    bool delete_file(const String& filename);
    void delete_all_files();
    
    // Folder operations
    String get_folder_path() { return fileFolder; }
    bool create_folder();
    
    // Debug info
    void print_storage_info();
};