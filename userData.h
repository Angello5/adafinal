//
//  userData.h
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#ifndef userData_h
#define userData_h
#include <cstring>
#include <cstdint>
#include <iostream>

using namespace std;

struct UserData {
    
    uint32_t dni;
    char nombreCompleto[50];
    char apellido[50];
    char nacionalidad[20];
    char birthPlace[20];
    char direccion[150];
    char telefono[15];
    char email[150];
    char estadoCivil[20];
    
    UserData( ):dni(0){
        memset(nombreCompleto, 0, sizeof(nombreCompleto));
        memset(apellido, 0, sizeof(apellido));
        memset(nacionalidad, 0, sizeof(nacionalidad));
        memset(birthPlace, 0, sizeof(birthPlace));
        memset(direccion, 0, sizeof(direccion));
        memset(telefono, 0, sizeof(telefono));
        memset(email, 0, sizeof(email));
    }
    
    UserData(uint32_t d, const char* nc, const char* ap, const char* nat, const char* bp, const char* dir, const char* tel, const char* em, const char* ec) : dni(d) {
        strncpy(nombreCompleto, nc, sizeof(nombreCompleto) - 1);
        strncpy(apellido, ap, sizeof(apellido) - 1);
        strncpy(nacionalidad, nat, sizeof(nacionalidad) - 1);
        strncpy(birthPlace, bp, sizeof(birthPlace) - 1);
        strncpy(direccion, dir, sizeof(direccion) - 1);
        strncpy(telefono, tel, sizeof(telefono) - 1);
        strncpy(email, em, sizeof(email) - 1);
        strncpy(estadoCivil, ec, sizeof(estadoCivil) - 1);
    }
    void printUsers( ){
        cout<<"DNI: "<< dni<<endl;
        cout<<"Nombre: "<<nombreCompleto<<endl;
        cout<<"Apellido: "<<apellido<<endl;
        cout<<"Lugar de Nacimiento: "<<birthPlace<< endl;
        cout<<"Direccion: "<<direccion<<endl;
        cout<<"Telefono: "<< telefono <<endl;
        cout<<"Correo: "<< email << endl;
        cout<<"Estado Civil" <<estadoCivil<<endl;
    }
};

#endif /* userData_h */
