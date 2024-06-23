//
//  pageManager.cpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//
/*
#include "pageManager.hpp"
#include <fstream>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
//#include <boost/interprocess/file_mapping.hpp>
//#include <boost/interprocess/mapped_region.hpp>
#include <stdexcept>

//using namespace boost::interprocess;
using namespace std;

void PageManager::prepareFiles() {
    try {
          ofstream dataFile(dataFilename, ios::binary | ios::app);
          if (!dataFile) throw runtime_error("Failed to open or create data file.");
          dataFile.seekp(0, ios::end);
          if (dataFile.tellp() == 0) { // Si el archivo está vacío
              dataFile.write("\0", 1); // Escribir un byte para asegurar que el archivo no está vacío
          }
          dataFile.close();
      } catch (const exception& e) {
          cerr << "Error with data file: " << e.what() << endl;
          throw;
      }

      // Preparar el archivo de índice
      try {
          ofstream indexFile(indexFilename, ios::binary | ios::app);
          if (!indexFile) throw runtime_error("Failed to open or create index file.");
          indexFile.seekp(0, ios::end);
          if (indexFile.tellp() == 0) {
              indexFile.write("\0", 1); // Similarmente asegura que el archivo de índice tiene contenido
          }
          indexFile.close();
      } catch (const exception& e) {
          cerr << "Error with index file: " << e.what() << endl;
          throw;
      }*/
/*    prepareSingleFiles(dataFilename);
    prepareSingleFiles(indexFilename);
}
void PageManager::prepareSingleFiles(const string& filename) {
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        cerr << "Failed to open or create " << filename << endl;
        throw runtime_error("Failed to open or create file.");
    }

    // Ensure the file is not empty
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == 0) {
        // Write a single byte to ensure file is not empty
        if (write(fd, "", 1) != 1) {
            close(fd);
            throw runtime_error("Failed to write to file.");
        }
    }
    close(fd);
}
PageManager::PageManager(const string& dataFileName, const string& indexFileName)
: dataFilename(dataFileName), indexFilename(indexFileName) {
     try {
     // Asegurar que el archivo de datos existe y es accesible
     ofstream outFile(dataFilename, ios::binary | ios::app);
     if(!outFile.is_open()){
     throw runtime_error("NO ABRE O DATA CREADA");
     }
     outFile.close();
     
     // Mapear el archivo de datos
     dataFile = file_mapping(dataFilename.c_str(), read_write);
     dataRegion = mapped_region(dataFile, read_write);
     
     // Asegurar que el archivo de índice existe y es accesible
     ofstream indexOutFile(indexFilename, ios::binary | ios::app);
     indexOutFile.close();
     
     // Mapear el archivo de índice
     indexFileMapping = file_mapping(indexFilename.c_str(), read_write);
     indexRegion = mapped_region(indexFileMapping, read_write);
     } catch (const exception& e) {
     cerr << "Error inicializando el PageManager: " << e.what() << endl;
     throw;
     }*/
    /* mapFile(dataFile,dataRegion,dataFilename);
     mapFile(indexFileMapping, indexRegion, indexFilename);
     }
     
     PageManager::~PageManager() {
     closeDataFile();
     closeIndexFile();
     }
     
     void PageManager::saveData(const void* src, size_t size) {
     if (size > dataRegion.get_size()) {
     throw runtime_error("Buffer overflow: Data size exceeds mapped region size.");
     }
     memcpy(dataRegion.get_address(), src, size);
     }
     
     vector<UserData> PageManager::loadData() const {
     size_t numEntries = dataRegion.get_size() / sizeof(UserData);
     vector<UserData> users(numEntries);
     memcpy(users.data(), dataRegion.get_address(), dataRegion.get_size());
     return users;
     }
     
     void PageManager::saveIndexData() {
     ofstream indexFile(indexFilename, ios::binary | ios::trunc);
     if (!indexFile.is_open()) {
     throw runtime_error("Cannot open index file for writing.");
     }
     for (const auto& entry : index) {
     indexFile.write(reinterpret_cast<const char*>(&entry), sizeof(IndexEntry));
     }
     indexFile.close();
     }
     
     void PageManager::loadIndexData() {
     ifstream indexFile(indexFilename, ios::binary);
     if (!indexFile.is_open()) {
     throw runtime_error("Cannot open index file for reading.");
     }
     IndexEntry entry;
     while (indexFile.read(reinterpret_cast<char*>(&entry), sizeof(IndexEntry))) {
     index.push_back(entry);
     }
     indexFile.close();
     }
     
     void PageManager::closeDataFile() {
     // Intencionadamente vacío: la destrucción de objetos file_mapping y mapped_region cierra los archivos
     }
     
     void PageManager::closeIndexFile() {
     // Intencionadamente vacío: la destrucción de objetos file_mapping y mapped_region cierra los archivos
     }
     */
#include "pageManager.hpp"
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <stdexcept>
#include <iostream>

using namespace std;

PageManager::PageManager(const string& dataFilename, const string& indexFilename)
    : dataFilename(dataFilename), indexFilename(indexFilename), dataFileDescriptor(-1), indexFileDescriptor(-1) {
    prepareFiles();
}

PageManager::~PageManager() {
    closeDataFile();
    closeIndexFile();
}

void PageManager::prepareFiles() {
    // Asegurar el tamaño mínimo inicial del archivo (por ejemplo, suficiente para 10000 registros)
    size_t initialSize = 100 * sizeof(UserData); // Ajusta este tamaño según tus necesidades

    // Preparar archivo de datos
    dataFileDescriptor = open(dataFilename.c_str(), O_RDWR | O_CREAT, 0666);
    if (dataFileDescriptor == -1) throw runtime_error("Fallo al abrir o crear el archivo de datos.");

    // Establecer el tamaño del archivo si es necesario
    struct stat fileInfo;
    if (fstat(dataFileDescriptor, &fileInfo) == -1) throw runtime_error("Fallo al obtener tamaño del archivo de datos.");
    if (fileInfo.st_size < initialSize) {
        if (ftruncate(dataFileDescriptor, initialSize) == -1) {
            throw runtime_error("Fallo al establecer tamaño del archivo de datos.");
        }
    }
    dataRegionSize = initialSize;
    dataRegion = mmap(NULL, dataRegionSize, PROT_READ | PROT_WRITE, MAP_SHARED, dataFileDescriptor, 0);
    if (dataRegion == MAP_FAILED) throw runtime_error("Fallo al mapear archivo de datos.");

    // Preparar archivo de índice
    indexFileDescriptor = open(indexFilename.c_str(), O_RDWR | O_CREAT, 0666);
    if (indexFileDescriptor == -1) throw runtime_error("Fallo al abrir o crear el archivo de índice.");
    if (fstat(indexFileDescriptor, &fileInfo) == -1) throw runtime_error("Fallo al obtener tamaño del archivo de índice.");
    if (fileInfo.st_size < initialSize) {
        if (ftruncate(indexFileDescriptor, initialSize) == -1) {
            throw runtime_error("Fallo al establecer tamaño del archivo de índice.");
        }
    }
    indexRegionSize = initialSize;
    indexRegion = mmap(NULL, indexRegionSize, PROT_READ | PROT_WRITE, MAP_SHARED, indexFileDescriptor, 0);
    if (indexRegion == MAP_FAILED) throw runtime_error("Fallo al mapear archivo de índice.");
}

void PageManager::saveData(const vector<UserData>& users) {
    size_t dataSize = users.size() * sizeof(UserData);
    if (dataSize > dataRegionSize) {
        cerr << "Desborde de Buffer: Data size " << dataSize << " excede el tamaño de la región mapeada " << dataRegionSize << endl;
        throw runtime_error("Buffer overflow: Data size exceeds mapped region size.");
    }
    memcpy(dataRegion, users.data(), dataSize);
    if (msync(dataRegion, dataRegionSize, MS_SYNC) == -1) {
        perror("Error al sincronizar la data al archivo");
        throw runtime_error("Failed to sync data to file.");
    }
    cout << "Data guardada satisfactoriamente." << endl;
}

vector<UserData> PageManager::loadData() {
    size_t numEntries = dataRegionSize / sizeof(UserData);
    vector<UserData> users(numEntries);
    memcpy(users.data(), dataRegion, dataRegionSize);
    return users;
}

void PageManager::saveIndexData() {
    memcpy(indexRegion, indexEntries.data(), indexEntries.size() * sizeof(IndexEntry));
}

void PageManager::loadIndexData() {
    size_t numEntries = indexRegionSize / sizeof(IndexEntry);
    indexEntries.resize(numEntries);
    memcpy(indexEntries.data(), indexRegion, indexRegionSize);
}

void PageManager::closeDataFile() {
    if (dataRegion) {
        munmap(dataRegion, dataRegionSize);
    }
    if (dataFileDescriptor != -1) {
        close(dataFileDescriptor);
    }
}

void PageManager::closeIndexFile() {
    if (indexRegion) {
        munmap(indexRegion, indexRegionSize);
    }
    if (indexFileDescriptor != -1) {
        close(indexFileDescriptor);
    }
}

void PageManager::mapFile(int& fileDescriptor, void*& mappedRegion, size_t& regionSize, const string& filename) {
    fileDescriptor = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    if (fileDescriptor == -1) throw runtime_error("Failed to open or create file.");
    struct stat fileInfo;
    if (fstat(fileDescriptor, &fileInfo) == -1) throw runtime_error("Failed to get file size.");
    if (fileInfo.st_size < regionSize) {
        if (ftruncate(fileDescriptor, regionSize) == -1) throw runtime_error("Failed to set file size.");
    }
    mappedRegion = mmap(NULL, regionSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    if (mappedRegion == MAP_FAILED) throw runtime_error("Failed to map file.");
}

void PageManager::unmapFile(void*& mappedRegion, size_t size) {
    if (mappedRegion) {
        munmap(mappedRegion, size);
        mappedRegion = nullptr;
    }
}

void PageManager::closeFile(int& fileDescriptor) {
    if (fileDescriptor != -1) {
        close(fileDescriptor);
        fileDescriptor = -1;
    }
}
