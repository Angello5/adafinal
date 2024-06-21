//
//  userData.h
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#ifndef userData_h
#define userData_h
#include <string>
#include <cstdint>

using namespace std;

struct UserData {
    
    uint32_t dni;
    string nombreCompleto;
    string apellido;
    string nacionalidad;
    string birthPlace;
    string direccion;
    string telefono;
    string email;
    string estadoCivil;
    
    UserData( ):dni(0){ }
    
    UserData(uint32_t d, const std::string& nc, const std::string& ap, const std::string& nat, const std::string& bp, const std::string& dir, const std::string& tel, const std::string& em, const std::string& ec)
        : dni(d), nombreCompleto(nc), apellido(ap), nacionalidad(nat), birthPlace(bp), direccion(dir), telefono(tel), email(em), estadoCivil(ec) {}
};



#endif /* userData_h */
