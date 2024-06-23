//
//  generateSaveData.hpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#ifndef generateSaveData_hpp
#define generateSaveData_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <cstdint>
#include "userData.h"
#include "pageManager.hpp"
//#include <boost/shared_ptr.hpp>

using namespace std;

class GenerateSaveData {
public:
    GenerateSaveData(const string& dataFileName, const string& indexFileName, size_t numRecords);
    ~GenerateSaveData( )= default;
    void generateData();
    const vector<UserData>& getGeneratedData() const;


private:
    
    string dataFilename;
    string indexFilename;
    size_t numRecords;
    vector<UserData> users;
    PageManager pageManager;
    
    uint32_t generateRandomDNI();
    void generateNombreAleatorio(char* result);
    void generateApellidoAleatorio(char* result);
    void generateNacionalidadAleatoria(char* result);
    void generateLugarNacimientoAleatorio(char* result);
    void generateDireccionAleatoria(char* result);
    void generateTelefonoAleatorio(char* result);
    void generateCorreoAleatorio(const char* nombre, const char* apellido, char* result);
    void generateEstadoCivilAleatorio(char* result);
};



#endif /* generateSaveData_hpp */
