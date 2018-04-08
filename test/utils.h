#ifndef UTILS_H
#define UTILS_H

#include "userlib/libnachos.h"
#include "userlib/syscall.h"

/**
 * Assertion.
 * \param b une valeur bool�enne.
 * \param msg un message.
 * Termine le programme avec le code -1 si l'assertion b
 * n'est pas v�rifi�. Dans ce cas le message msg est imprim�
 * sur la sortie d'erreur.
**/
void assert(bool b, const char *msg){
    if(!b){
        n_printf(msg);
        Exit(1);
    }
}

#endif // UTILS_H
