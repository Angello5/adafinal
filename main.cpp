//
//  main.cpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#include <iostream>
#include "arbolB.hpp"
#include "cuckooHashing.hpp"
#include "pageManager.hpp"
#include "generateSaveData.hpp"
#include "userData.h"
#include <iostream>
#include <vector>
#include <memory>
//#include <boost/make_shared.hpp>
//#include <boost/interprocess/file_mapping.hpp>
//#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <cstdint>

using namespace std;
//using namespace boost::interprocess;

void displayMenu() {
    cout << "/nSeleccione una opción:" << endl;
    cout << "1. Añadir usuario" << endl;
    cout << "2. Eliminar usuario" << endl;
    cout << "3. Buscar usuario" << endl;
    cout << "4. Salir" << endl;
    cout << "Opción: ";
}

int main() {
    const string dataFilename = "data.bin";
    const string indexFilename = "index.bin";

    try {
        GenerateSaveData generateSaveData(dataFilename, indexFilename, 100);  // Número reducido para pruebas
        generateSaveData.generateData();

        PageManager pageManager(dataFilename, indexFilename);

        // Acceder a los datos generados y guardarlos
        const auto& generatedUsers = generateSaveData.getGeneratedData();
        cout << "Guardando datos generados en el archivo..." << endl;
        pageManager.saveData(generatedUsers.data(), generatedUsers.size() * sizeof(UserData));
        cout << "Datos guardados correctamente." << endl;

        // Cargar los datos guardados para uso posterior
        cout << "Cargando datos desde el archivo..." << endl;
        vector<UserData> loadedUsers = pageManager.loadData();
        cout << "Datos cargados correctamente. Total de usuarios cargados: " << loadedUsers.size() << endl;

        Btree tree(10);
        cuckooHashing cuckoo(10003);

        cout << "Insertando datos en estructuras de datos..." << endl;
        for (const auto& user : loadedUsers) {
            if (!cuckoo.insert(user.dni)) {
                cout << "Error al insertar el DNI en Cuckoo Hashing: " << user.dni << endl;
            }
            tree.insert(user.dni, make_shared<UserData>(user));
        }
        cout << "Datos insertados correctamente en las estructuras de datos." << endl;

        int option;
        uint32_t dni;

        do {
            displayMenu();
            cin >> option;

            switch (option) {
                case 1: {
                    cout << "Ingrese el DNI del nuevo usuario: ";
                    cin >> dni;
                    auto newUser = make_shared<UserData>(dni, "Nombre", "Apellido", "Nacionalidad", "LugarNacimiento", "Direccion", "Telefono", "Email", "EstadoCivil");
                    if (!cuckoo.insert(dni)) {
                        cout << "Error: No se pudo insertar el usuario." << endl;
                    } else {
                        tree.insert(dni, newUser);
                        loadedUsers.push_back(*newUser);
                        pageManager.saveData(loadedUsers.data(), loadedUsers.size() * sizeof(UserData));
                        pageManager.saveIndexData();
                    }
                    break;
                }
                case 2: {
                    cout << "Ingrese el DNI del usuario a eliminar: ";
                    cin >> dni;
                    if (cuckoo.remove(dni)) {
                        tree.remove(dni);
                        cout << "Usuario eliminado exitosamente." << endl;
                    } else {
                        cout << "Error: No se encontró el usuario." << endl;
                    }
                    break;
                }
                case 3: {
                    cout << "Ingrese el DNI del usuario a buscar: ";
                    cin >> dni;
                    auto user = cuckoo.search(dni);
                    if (user) {
                        cout << "Usuario encontrado: " << user->nombreCompleto << endl;
                    } else {
                        cout << "Usuario no encontrado." << endl;
                    }
                    break;
                }
                case 4:
                    cout << "Saliendo..." << endl;
                    break;
                default:
                    cout << "Opción no válida. Intente nuevamente." << endl;
                    break;
            }
        } while (option != 4);

    } catch (const exception& ex) {
        cerr << "Error durante la inicialización: " << ex.what() << endl;
        return 1;
    }

    return 0;
}
