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

string GenerateSaveData::generateNombreAleatorio(){
    static const vector<string> nombres ={"Carlos","Angello","Marcelo","Federico","Valeska","Emmanuel",
                                "Samuel","Namir","Ysabel","Renzo","Erick","Esteban","Julia","Brenda","Ariana","Manuela",
                                "Estefano","Araceli","Gabriel","Victor","Angel","Valentina","Valeria","Italo","Sofia","Amber",
                                "Peter","Kevin","Gwen","Miguel","Daniela","Daniel","Tomas"};
    
    mt19937 gen(random_device{ }());
    uniform_int_distribution<size_t> dis(0,nombres.size()-1);
    return nombres[dis(gen)];
}

string GenerateSaveData::generateApellidoAleatorio() {
    static const vector<string> apellidos = {"Llerena", "Lopez", "Gomez", "Rodriguez", "Roque", "Cerron", "Rios", "Vargas", "Mendoza",
                                                       "Flores", "Sanchez", "Ramos", "Chavez", "Torres", "Cruz", "Vasquez", "Villegas",
                                                       "Vega", "Castillo", "Velasquez", "Guerrero", "Polo", "Quispe", "Mamani", "Toro", "Tello", "Rojas",
                                                       "Alvarez", "Silva", "Fernandez", "Olivos", "Taipe", "Leon", "Garcia", "Cusihuaman", "Aguilar",
                                                       "Salazar", "Ruiz", "Medina", "Menacho", "Orderique", "Rivas", "Ruiz", "Reyes", "Reyna", "Neyra",
                                                       "Ramirez", "Ramos", "Grimaldo", "Sonne", "Dominguez", "Cartagena", "Gallese", "Torres", "Pineda",
                                                       "Pinedo", "Osores", "Zanelatto","Santamaria","Limay","Rocha"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, apellidos.size() - 1);
    return apellidos[dis(gen)];
}

string GenerateSaveData::generateNacionalidadAleatoria() {
    return "Peruano(a)";
}

string GenerateSaveData::generateLugarNacimientoAleatorio() {
    static const vector<string> lugar_nacimiento = {"Lima", "Cajamarca", "Cusco", "Arequipa", "Puno", "Ica", "Tacna", "Piura", "Lambayeque",
                                                              "La Libertad", "Ancash", "Junin", "Huanuco", "Pasco", "Ucayali", "Madre de Dios", "Loreto","Callao",
                                                              "San Martin", "Amazonas", "Tumbes", "Moquegua", "Apurimac", "Ayacucho", "Huancavelica"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, lugar_nacimiento.size() - 1);
    return lugar_nacimiento[dis(gen)];
}

string GenerateSaveData::generateDireccionAleatoria() {
    static const vector<string> direcciones = {"Calle Rosas 121", "Av.  Javier Prado 712", "Manzo de Velasco 266", "Jr. Ancash 143", "Av.                Abancay 561", "Pablo Olavide 405","Jiron Grau 968", "Av.Brasil 211", "Jr. Puno 154", "Jr. Ayacucho 371", "Calle Luiggi                   Pirandello 314", "Av. Guardia Civil 1070","Jiron Camana 123", "Iquique 312", "Av. Abancay", "Av. Tacna 971", "Jr. Las Grullas 138", "Psj.                  Velasquez 211", "Calle Halcones 129", "Av. Alfonso Ugarte 893","Calle Luis Garcia", "Av. de la Marina", "Av. San Felipe 731","Av. Elmer Faucett 1124", "Ca. Pablo Ramirez Taboada", "Av. La Mar"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, direcciones.size() - 1);
    return direcciones[dis(gen)];
}

string GenerateSaveData::generateTelefonoAleatorio() {
    static const string digitos = "0123456789";
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, digitos.size() - 1);
    string resultado;
    for (size_t i = 0; i < 8; ++i) {
        resultado += digitos[dis(gen)];
    }
    return "9" + resultado;
}

string GenerateSaveData::generateCorreoAleatorio(const string& nombre, const string& apellido) {
    static const vector<string> dominios = {"@hotmail.com", "@gmail.com", "@outlook.com", "@yahoo.com", "@icloud.com"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, dominios.size() - 1);
    string nombreLower = nombre;
    transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);
    string apellidoLower = apellido;
    transform(apellidoLower.begin(), apellidoLower.end(), apellidoLower.begin(), ::tolower);
    return nombreLower + apellidoLower + dominios[dis(gen)];
}

string GenerateSaveData::generateEstadoCivilAleatorio() {
    static const vector<string> estados = {"Casado(a)", "Soltero(a)", "Divorciado(a)", "Viudo(a)"};
    mt19937 gen(random_device{}());
    uniform_int_distribution<size_t> dis(0, estados.size() - 1);
    return estados[dis(gen)];
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
        
        string nombreCompleto = generateNombreAleatorio();
        string apellido = generateApellidoAleatorio();
        string nacionalidad = generateNacionalidadAleatoria();
        string birthPlace = generateLugarNacimientoAleatorio();
        string direccion = generateDireccionAleatoria();
        string telefono = generateTelefonoAleatorio();
        string email = generateCorreoAleatorio(nombreCompleto, apellido);
        string estadoCivil = generateEstadoCivilAleatorio();

        users.emplace_back(dni, nombreCompleto, apellido, nacionalidad, birthPlace, direccion, telefono, email, estadoCivil);
        std::cout << "Generated user: " << dni <<" " <<email<< std::endl;
    }
}
const vector<UserData>& GenerateSaveData::getGeneratedData() const {
    return users;
}
