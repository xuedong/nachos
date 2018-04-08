#ifndef MATRIX_H
#define MATRIX_H

#include "utils.h"

#define N 4
#define FUZZY 0.00025

/**
 * structure matrix_t
 * représente une matrice carré.
**/
typedef struct{
    size_t size;
    //double *data;
    double data[N*N];
}matrix_t;

/**
 * constructeur de matrice
 * \param m pointeur vers la matrice à construire.
 * \param size taille de la matrice.
**/
void matrix_create(matrix_t *m/*, size_t size*/){
    //m->data = (double*)malloc(size*size*sizeof(double));
    m->size = N;//size;
    n_memset((void*)m->data, 0, m->size*m->size*sizeof(double));
}

/**
 * destructeur de matrice
 * \param m pointeur vers la matrice à libérer.
**/
void matrix_free(matrix_t *m){
    if(m->size > 0){
        m->size = 0;
        //n_free(m->data);
    }
}

/**
 * getter
 * \param m pointeur vers une matrice
 * \param i index de la ligne
 * \param j index de la colonne
 * \return la valeur de m en (i, j)
**/
double matrix_get(matrix_t *m, size_t i, size_t j){
    return m->data[i*m->size + j];
}

/**
 * setter
 * \param m pointeur vers une matrice
 * \param i index de la ligne
 * \param j index de la colonne
 * \param value
 * affecte value à m en (i, j)
**/
void matrix_set(matrix_t *m, size_t i, size_t j, double value){
    m->data[i*m->size + j] = value;
}

/**
 * visualisation de matrice
 * \param m une matrice
 * Affiche la matrice m sur la sortie standard.
**/
void matrix_print(matrix_t *m){
    size_t i, j;
    for(i = 0; i < m->size; ++i){
        for(j = 0; j < m->size; ++j){
            n_printf("%d\t", m->data[i*m->size + j]);
        }
        n_puts("");
    }
}

/**
 * Comparaison de matrices
 * \param a une matrice
 * \param b une matrice
 * \return true si les valeurs des matrices a b sont identiques à FUZZY près.
**/
bool matrix_compare(matrix_t *a, matrix_t *b){
    assert(a->size == b->size, "incompatibles matrices");
    size_t i = 0;
    for(i = 0; i < a->size*a->size; ++i){
        double d = a->data[i]-b->data[i];
        if(d > FUZZY || -d > FUZZY){
            return false;
        }
    }

    return true;
}

#endif // MATRIX_H
