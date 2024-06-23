//
//  generateSaveData.cpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//
#include "generateSaveData.hpp"
#include <random>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <cstring>

using namespace std;

GenerateSaveData::GenerateSaveData(const string& dataFileName, const string& indexFileName, size_t numRecords)
: dataFilename(dataFileName), indexFilename(indexFileName), numRecords(numRecords), pageManager(dataFileName,indexFileName){ }

uint32_t GenerateSaveData::generateRandomDNI(){
    static mt19937 gen(random_device{ }());
    static uniform_int_distribution<uint32_t> dis(10000000, 99999999);
    return dis(gen);
}

void GenerateSaveData::generateNombreAleatorio(char* result){
    static const vector<string> nombres = {"Carlos","Angello","Marcelo","Federico","Valeska","Emmanuel",
                                "Samuel","Namir","Ysabel","Renzo","Erick","Esteban","Julia","Brenda","Ariana","Manuela",
                                "Estefano","Araceli","Gabriel","Victor","Angel","Valentina","Valeria","Italo","Sofia","Amber",
                                "Peter","Kevin","Gwen","Miguel","Daniela","Daniel","Tomas"};
    
    mt19937 gen(random_device{ }());
    uniform_int_distribution<size_t> dis(0,nombres.size()-1);
    strncpy(result, nombres[dis(gen)].c_str(), 49);
}

void GenerateSaveData::generateApellidoAleatorio(char* result) {
    static const vector<string> apellidos = {"Llerena", "Lopez", "Gomez", "Rodriguez", "Roque", "Cerron", "Rios", "Vargas", "Mendoza",
                                                       "Flores", "Sanchez", "Ramos", "Chavez", "Torres", "Cruz", "Vasquez", "Villegas",
                                                       "Vega", "Castillo", "Velasquez", "Guerrero", "Polo", "Quispe", "Mamani", "Toro", "Tello", "Rojas",
                                                       "Alvarez", "Silva", "Fernandez", "Olivos", "Taipe", "Leon", "Garcia", "Cusihuaman", "Aguilar",
                                                       "Salazar", "Ruiz", "Medina", "Menacho", "Orderique", "Rivas", "Ruiz", "Reyes", "Reyna", "Neyra",
                                                       "Ramirez", "Ramos", "Grimaldo", "Sonne", "Dominguez", "Cartagena", "Gallese", "Torres", "Pineda",
                                                       "Pinedo", "Osores", "Zanelatto","Santamaria","Limay","Rocha"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, apellidos.size() - 1);
    strncpy(result, apellidos[dis(gen)].c_str(), 49);
}

void GenerateSaveData::generateNacionalidadAleatoria(char* result) {
    strncpy(result, "Peruano(a)", 19);
}

void GenerateSaveData::generateLugarNacimientoAleatorio(char* result) {
    static const vector<string> lugar_nacimiento = {"Lima", "Cajamarca", "Cusco", "Arequipa", "Puno", "Ica", "Tacna", "Piura", "Lambayeque",
                                                              "La Libertad", "Ancash", "Junin", "Huanuco", "Pasco", "Ucayali", "Madre de Dios", "Loreto","Callao",
                                                              "San Martin", "Amazonas", "Tumbes", "Moquegua", "Apurimac", "Ayacucho", "Huancavelica"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, lugar_nacimiento.size() - 1);
    strncpy(result, lugar_nacimiento[dis(gen)].c_str(), 19);
}

void GenerateSaveData::generateDireccionAleatoria(char* result) {
    static const vector<string> direcciones = {"Calle Rosas 121", "Av.  Javier Prado 712", "Manzo de Velasco 266", "Jr. Ancash 143", "Av. Abancay 561", "Pablo Olavide 405","Jiron Grau 968", "Av.Brasil 211", "Jr. Puno 154", "Jr. Ayacucho 371", "Calle Luiggi Pirandello 314", "Av. Guardia Civil 1070","Jiron Camana 123", "Iquique 312", "Av. Abancay", "Av. Tacna 971", "Jr. Las Grullas 138", "Psj. Velasquez 211", "Calle Halcones 129", "Av. Alfonso Ugarte 893","Calle Luis Garcia", "Av. de la Marina", "Av. San Felipe 731","Av. Elmer Faucett 1124", "Ca. Pablo Ramirez Taboada", "Av. La Mar"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, direcciones.size() - 1);
    strncpy(result, direcciones[dis(gen)].c_str(), 149);
}

void GenerateSaveData::generateTelefonoAleatorio(char* result) {
    static const string digitos = "0123456789";
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, digitos.size() - 1);
    string telefono = "9";
    for (size_t i = 0; i < 8; ++i) {
        telefono += digitos[dis(gen)];
    }
    strncpy(result, telefono.c_str(), 14);
}

void GenerateSaveData::generateCorreoAleatorio(const char* nombre, const char* apellido, char* result) {
    static const vector<string> dominios = {"@hotmail.com", "@gmail.com", "@outlook.com", "@yahoo.com", "@icloud.com"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, dominios.size() - 1);
    string correo = string(nombre) + string(apellido) + dominios[dis(gen)];
    strncpy(result, correo.c_str(), 149);
}

void GenerateSaveData::generateEstadoCivilAleatorio(char* result) {
    static const vector<string> estados = {"Casado(a)", "Soltero(a)", "Divorciado(a)", "Viudo(a)"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, estados.size() - 1);
    strncpy(result, estados[dis(gen)].c_str(), 19);
}

void GenerateSaveData::generateData() {
    unordered_set<uint32_t> dniSet;
    users.reserve(numRecords);
    for (size_t i = 0; i < numRecords; ++i) {
        uint32_t dni;
        do{
            dni = generateRandomDNI();
        }while(dniSet.find(dni) != dniSet.end());
        dniSet.insert(dni);

        char nombreCompleto[50];
        char apellido[50];
        char nacionalidad[20];
        char birthPlace[20];
        char direccion[150];
        char telefono[15];
        char email[150];
        char estadoCivil[20];

        generateNombreAleatorio(nombreCompleto);
        generateApellidoAleatorio(apellido);
        generateNacionalidadAleatoria(nacionalidad);
        generateLugarNacimientoAleatorio(birthPlace);
        generateDireccionAleatoria(direccion);
        generateTelefonoAleatorio(telefono);
        generateCorreoAleatorio(nombreCompleto, apellido, email);
        generateEstadoCivilAleatorio(estadoCivil);

        users.emplace_back(dni, nombreCompleto, apellido, nacionalidad, birthPlace, direccion, telefono, email, estadoCivil);
        std::cout << "Generated user: " << dni <<" " <<email<< std::endl;
    }
}

const vector<UserData>& GenerateSaveData::getGeneratedData() const {
    return users;
}
