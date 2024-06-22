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
    /*try {
          std::ofstream dataFile(dataFilename, std::ios::binary | std::ios::app);
          if (!dataFile) throw std::runtime_error("Failed to open or create data file.");
          dataFile.seekp(0, std::ios::end);
          if (dataFile.tellp() == 0) { // Si el archivo está vacío
              dataFile.write("\0", 1); // Escribir un byte para asegurar que el archivo no está vacío
          }
          dataFile.close();
      } catch (const std::exception& e) {
          std::cerr << "Error with data file: " << e.what() << std::endl;
          throw;
      }

      // Preparar el archivo de índice
      try {
          std::ofstream indexFile(indexFilename, std::ios::binary | std::ios::app);
          if (!indexFile) throw std::runtime_error("Failed to open or create index file.");
          indexFile.seekp(0, std::ios::end);
          if (indexFile.tellp() == 0) {
              indexFile.write("\0", 1); // Similarmente asegura que el archivo de índice tiene contenido
          }
          indexFile.close();
      } catch (const std::exception& e) {
          std::cerr << "Error with index file: " << e.what() << std::endl;
          throw;
      }*/
/*    prepareSingleFiles(dataFilename);
    prepareSingleFiles(indexFilename);
}
void PageManager::prepareSingleFiles(const std::string& filename) {
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
    /* try {
     // Asegurar que el archivo de datos existe y es accesible
     std::ofstream outFile(dataFilename, ios::binary | ios::app);
     if(!outFile.is_open()){
     throw runtime_error("NO ABRE O DATA CREADA");
     }
     outFile.close();
     
     // Mapear el archivo de datos
     dataFile = file_mapping(dataFilename.c_str(), read_write);
     dataRegion = mapped_region(dataFile, read_write);
     
     // Asegurar que el archivo de índice existe y es accesible
     std::ofstream indexOutFile(indexFilename, ios::binary | ios::app);
     indexOutFile.close();
     
     // Mapear el archivo de índice
     indexFileMapping = file_mapping(indexFilename.c_str(), read_write);
     indexRegion = mapped_region(indexFileMapping, read_write);
     } catch (const std::exception& e) {
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
    // Asegurar el tamaño mínimo inicial del archivo (por ejemplo, suficiente para 100 registros)
    size_t initialSize = 10000 * sizeof(UserData); // Ajusta este tamaño según tus necesidades

    dataFileDescriptor = open(dataFilename.c_str(), O_RDWR | O_CREAT, 0666);
    if (dataFileDescriptor == -1) throw runtime_error("Fallo al abrir o crear la data del archivo uu");

    // Establecer el tamaño del archivo si es necesario
    struct stat fileInfo;
    if (fstat(dataFileDescriptor, &fileInfo) == -1) throw runtime_error("Fallo al obtener tamano de data");
    if (fileInfo.st_size < initialSize) {
        if (ftruncate(dataFileDescriptor, initialSize) == -1) {
            throw runtime_error("Failed to set file size.");
        }
    }
    dataRegionSize = initialSize;
    dataRegion = mmap(NULL, dataRegionSize, PROT_READ | PROT_WRITE, MAP_SHARED, dataFileDescriptor, 0);
    if (dataRegion == MAP_FAILED) throw runtime_error("Failed to map data file.");

    // Similar para el archivo de índice
    indexFileDescriptor = open(indexFilename.c_str(), O_RDWR | O_CREAT, 0666);
    if (indexFileDescriptor == -1) throw runtime_error("Failed to open or create index file.");
    if (fstat(indexFileDescriptor, &fileInfo) == -1) throw runtime_error("Failed to get file size.");
    if (fileInfo.st_size < initialSize) { // Asegurar que el tamaño del archivo de índice también es adecuado
        if (ftruncate(indexFileDescriptor, initialSize) == -1) {
            throw runtime_error("Failed to set index file size.");
        }
    }
    indexRegionSize = initialSize;
    indexRegion = mmap(NULL, indexRegionSize, PROT_READ | PROT_WRITE, MAP_SHARED, indexFileDescriptor, 0);
    if (indexRegion == MAP_FAILED) throw runtime_error("Failed to map index file.");
}


void PageManager::saveData(const void* src, size_t size) {
    if (size > dataRegionSize) {
        cerr<<"Desborde de Buffer: Data size :" <<size<< " excede el tamana de la region mapeada "<<dataRegionSize<<" uu"<<endl;
        throw runtime_error("Buffer overflow: Data size exceeds mapped region size.");
    }
    memcpy(dataRegion, src, size);
    if(msync(dataRegion, dataRegionSize, MS_SYNC)== -1){
        perror("Error al sincronizar la data al archivo");
        throw runtime_error("Failed to sync data to file");
    }
    cout<<"Data guardado satisfactoriamente siiiiu"<<endl;
}

std::vector<UserData> PageManager::loadData() {
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
    munmap(dataRegion, dataRegionSize);
    close(dataFileDescriptor);
}

void PageManager::closeIndexFile() {
    munmap(indexRegion, indexRegionSize);
    close(indexFileDescriptor);
}

