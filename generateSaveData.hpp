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
    string generateNombreAleatorio();
    string generateApellidoAleatorio();
    string generateNacionalidadAleatoria();
    string generateLugarNacimientoAleatorio();
    string generateDireccionAleatoria();
    string generateTelefonoAleatorio();
    string generateCorreoAleatorio(const string& nombre, const string& apellido);
    string generateEstadoCivilAleatorio();
};



#endif /* generateSaveData_hpp */
