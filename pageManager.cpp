//
//  pageManager.cpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//
/*
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
 */
#include "pageManager.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <cstring>

using namespace std;
namespace fs = std::filesystem;

PageManager::PageManager(const string& dataFilename, const string& indexFilename)
    : dataFilename(dataFilename), indexFilename(indexFilename) {}

PageManager::~PageManager() {}

void PageManager::saveData(const vector<UserData>& users) {
    /*ifstream infile(dataFilename, ios::binary | ios::ate);
    if(infile.is_open()){
        size_t fileSize = infile.tellg();
        cout<<"Tamana del archivo data.bin" << fileSize <<" bytes" <<endl;
        infile.close();
    }else{
        throw runtime_error("Error al abrir al archivo para verificar el tamano uu");
    }*/
    ofstream outFile(dataFilename, ios::binary);
        if (!outFile) {
            throw runtime_error("No se pudo abrir el archivo de datos para escritura.");
        }

        // Obtener la ruta completa
        fs::path fullPath = fs::absolute(dataFilename);
        cout << "Guardando datos en el archivo: " << fullPath << endl;

        // Escribir los datos al archivo
        outFile.write(reinterpret_cast<const char*>(users.data()), users.size() * sizeof(UserData));
        outFile.close();

        if (!outFile.good()) {
            throw runtime_error("Error al escribir los datos al archivo.");
        }
    indexEntries.clear();
    for(size_t i =0;i <users.size(); i++){
        outFile.write(reinterpret_cast<const char*>(&users[i]), sizeof(UserData));
                IndexEntry entry = { users[i].dni, i * sizeof(UserData) };
                indexEntries.push_back(entry);
    }
    outFile.close();

        cout << "Datos guardados satisfactoriamente en: " << fullPath << endl;
}

vector<UserData> PageManager::loadData() {
    ifstream inFile(dataFilename, ios::binary);
        if (!inFile) {
            throw runtime_error("No se pudo abrir el archivo de datos para lectura.");
        }
    
        fs::path fullPath = fs::absolute(dataFilename);
        cout << "Cargando datos desde el archivo: " << fullPath << endl;
        // Obtener el tamaño del archivo
        inFile.seekg(0, ios::end);
        size_t fileSize = inFile.tellg();
        inFile.seekg(0, ios::beg);

        size_t numEntries = fileSize / sizeof(UserData);
        vector<UserData> users(numEntries);

        // Leer los datos del archivo
        inFile.read(reinterpret_cast<char*>(users.data()), fileSize);
        inFile.close();

        if (!inFile.good()) {
            throw runtime_error("Error al leer los datos del archivo.");
        }

        cout << "Datos cargados correctamente desde: " << fullPath << endl;
        return users;
}

void PageManager::saveIndexData() {
    ofstream outFile(indexFilename, ios::binary | ios::trunc);
        if (!outFile) {
            throw runtime_error("No se pudo abrir el archivo de índice para escritura.");
        }

        fs::path fullPath = fs::absolute(indexFilename);
        cout << "Guardando datos de índice en el archivo: " << fullPath << endl;
        // Escribir los datos de índice al archivo
    for (const auto& entry : indexEntries) {
            outFile.write(reinterpret_cast<const char*>(&entry), sizeof(IndexEntry));
        }

        outFile.close();

        cout << "Datos de índice guardados satisfactoriamente en: " << fullPath << endl;
    }

void PageManager::loadIndexData() {
    ifstream inFile(indexFilename, ios::binary);
        if (!inFile) {
            throw runtime_error("No se pudo abrir el archivo de índice para lectura.");
        }
        
        fs::path fullPath = fs::absolute(indexFilename);
        cout << "Cargando datos de índice desde el archivo: " << fullPath << endl;
        // Obtener el tamaño del archivo
        /*inFile.seekg(0, ios::end);
        size_t fileSize = inFile.tellg();
        inFile.seekg(0, ios::beg);

        size_t numEntries = fileSize / sizeof(IndexEntry);
        indexEntries.resize(numEntries);

        // Leer los datos de índice del archivo
        inFile.read(reinterpret_cast<char*>(indexEntries.data()), fileSize);
        inFile.close();*/
        indexEntries.clear();

        IndexEntry entry;
    while (inFile.read(reinterpret_cast<char*>(&entry), sizeof(IndexEntry))) {
            indexEntries.push_back(entry);
        }

    inFile.close();

        if (!inFile.good()) {
            throw runtime_error("Error al leer los datos de índice del archivo.");
        }

        cout << "Datos de índice cargados correctamente desde: " << fullPath << endl;
}

