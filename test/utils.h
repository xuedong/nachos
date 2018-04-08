#ifndef UTILS_H
#define UTILS_H

#include "userlib/libnachos.h"
#include "userlib/syscall.h"

/**
 * Assertion.
 * \param b une valeur booléenne.
 * \param msg un message.
 * Termine le programme avec le code -1 si l'assertion b
 * n'est pas vérifié. Dans ce cas le message msg est imprimé
 * sur la sortie d'erreur.
**/
void assert(bool b, const char *msg){
    if(!b){
        n_printf(msg);
        Exit(1);
    }
}

#endif // UTILS_H
