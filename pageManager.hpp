//
//  pageManager.hpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//
/*
#ifndef pageManager_hpp
#define pageManager_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
//#include <boost/interprocess/file_mapping.hpp>
//#include <boost/interprocess/mapped_region.hpp>
#include "userData.h"


using namespace std;
//using namespace boost::interprocess;

class PageManager {
    
struct IndexEntry {
    uint32_t dni;
    size_t position;
};
    
public:
    PageManager(const string& dataFileName, const string& indexFileName);
    ~PageManager();
    
    void saveData(const void* src, size_t size);
    vector<UserData> loadData() const;
    void saveIndexData();
    void loadIndexData();
    
private:
    
    string dataFilename;
    string indexFilename;
    //file_mapping dataFile;
    //mapped_region dataRegion;
    //file_mapping indexFileMapping;
    //mapped_region indexRegion;
    int dataFile;
    int indexFileMapping;
    void* dataRegion;
    void* indexRegion;
    size_t dataRegionSize;
    size_t indexRegionSize;
    vector<IndexEntry> index;
    void prepareFiles( );
    void prepareSingleFiles(const string& filname);
    void mapFile(int& fileDescriptor, void*& mappedRegion, const string& filename);
    void unmapFile(void* mappedRegion, size_t size);
    void closeDataFile();
    void closeIndexFile();

};


#endif
*/

/*#ifndef PAGE_MANAGER_HPP
#define PAGE_MANAGER_HPP

#include <string>
#include <vector>
#include <cstdint>
#include "userData.h"
#include <cstdint>

using namespace std;

struct IndexEntry {
    uint32_t dni;
    size_t position;
};

class PageManager {
private:
    string dataFilename;
    string indexFilename;
    int dataFileDescriptor;
    int indexFileDescriptor;
    void* dataRegion;
    void* indexRegion;
    size_t dataRegionSize;
    size_t indexRegionSize;
    vector<IndexEntry> indexEntries;

    void prepareFiles();
    void mapFile(int& fileDescriptor, void*& mappedRegion, size_t& regionSize, const string& filename);
    void unmapFile(void*& mappedRegion, size_t size);
    void closeFile(int& fileDescriptor);
public:
    PageManager(const string& dataFilename, const string& indexFilename);
    ~PageManager();
    void saveData(const vector<UserData>& users);
    vector<UserData> loadData();
    void saveIndexData();
    void loadIndexData();
    void closeDataFile();
    void closeIndexFile();
};

#endif // PAGE_MANAGER_HPP

*/
#ifndef PAGE_MANAGER_HPP
#define PAGE_MANAGER_HPP

#include <string>
#include <vector>
#include <cstdint>
#include "userData.h"
#include <cstdint>

struct IndexEntry {
    uint32_t dni;
    size_t position;
};

class PageManager {
private:
    std::string dataFilename;
    std::string indexFilename;
    std::vector<IndexEntry> indexEntries;

public:
    PageManager(const std::string& dataFilename, const std::string& indexFilename);
    ~PageManager();
    void saveData(const std::vector<UserData>& users);
    std::vector<UserData> loadData();
    void saveIndexData();
    void loadIndexData();
};

#endif // PAGE_MANAGER_HPP

