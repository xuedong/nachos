// On aime les booléens.
#define bool int
#define true 1
#define false 0

#include "matrix.h"

// Multiplication de matrice sans thread. Permettra de comparer avec
// la multiplication multi-threads.
// r = a * b
void matrix_mult_seq(matrix_t *a, matrix_t *b, matrix_t *r){
	// On vérifie que les matrices sont compatibles.
    assert(a->size == b->size && b->size == r->size, "incompatibles matrix");
    assert(a->size, "null matrix"); // Matrices non nulles.
    size_t n = a->size;

	// Une simple multiplication de matrices...
    size_t i, j, k;
    for(i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){

            double tmp = 0.0;
            for(k = 0; k < n; ++k){
                tmp += matrix_get(a, i, k) * matrix_get(b, k, j);
            }

            matrix_set(r, i, j, tmp);

        }
    }
}

// Environnement de calcul communiqué à chaque thread
struct matrix_mult_proc_t{
    matrix_t *a, *b; // Les matrices passées en paramètres.
	matrix_t *r; // La matrice résultat.
    size_t i; // Le numéro du thread.
    size_t nthread; // Le nombre de threads.
};

// Code éxécuté dans chaque thread lors de la multiplication
// matricielle. Chaque thread prend une instance de 
// matrix_mult_proc_t permettant de connaitre les modélités du
// calcul à effectuer.
void matrix_mult_thread(struct matrix_mult_proc_t *proc){
    size_t n = proc->a->size;

    size_t i, j, k;
	// On reprend l'idée d'une simple multiplication de matrices,
	// mais on commence i à proc->i (le numéro du thread) et on
	// incrémente à chaque fois de proc->nthread (le nombre de
	// threads). Les calculs manquants seront effectués simultanement
	// sur les autres threads.
    for(i = proc->i; i < n; i += proc->nthread){
        for(j = 0; j < n; ++j){

            double tmp = 0.0;
            for(k = 0; k < n; ++k){
                tmp += matrix_get(proc->a, i, k) * matrix_get(proc->b, k, j);
            }

            matrix_set(proc->r, i, j, tmp);

        }
    }
}

// Multiplication matricielle multi-threads.
// r = a * b, sur nthreads threads.
void matrix_mult(matrix_t *a, matrix_t *b, matrix_t *r, size_t nthread){
	// On vérifie que les matrices sont compatibles.
    assert(a->size == b->size && b->size == r->size, "incompatibles matrix");
    assert(a->size, "null matrix"); // Matrices non nulles.

    pthread_t t[nthread]; // On alloue de l'espace pour les ids de nos threads.
    matrix_mult_proc_t proc[nthread];
    size_t i;
    for(i = 0; i < nthread; ++i){ // Pour chaque thread.
		// On créé notre environnement de calcul.
        proc[i]->a = a;
        proc[i]->b = b;
        proc[i]->r = r;
        proc[i]->i = i;
        proc[i]->nthread = nthread;

		// On lance le calcul.
        pthread_create(&t[i], NULL, (void *(*)(void*))matrix_mult_thread, (void*)&proc[i]);
    }

    for(i = 0; i < nthread; ++i){
        pthread_join(t[i], NULL); // On attend la fin de tous les threads.
    }
}

int main(int argc, char *argv[]){
    //size_t n = 4; // Taille des matrices par défaut.
    size_t nthread = 8; // Nombre de threads par défaut.
    //if(argc > 1){
    //    n = atoi(argv[1]); // On change la taille des matrices.

        if(argc > 1){
            nthread = atoi(argv[1]); // On change le nombre de threads.
        }
    //}

	// On créé une matrice, ainsi que r1 et r2
	// qui contiendront le résultat de m*m suivant nos
	// deux méthodes de multiplication.
    matrix_t m, r1, r2;
    matrix_create(&m, n);
    matrix_create(&r1, n);
    matrix_create(&r2, n);

	// On rempli? la matrice...
    size_t i;
    for(i = 0; i < m.size*m.size; ++i){
        m.data[i] = i;
    }

	// On fait deux multiplications différentes.
    matrix_mult_seq(&m, &m, &r1);
    matrix_mult(&m, &m, &r2, nthread);

	// Si la taille des matrices est résonable, on
	// se permet de les afficher sur la sortie standard.
    if(n < 10){
        matrix_print(&r1);
        n_puts("para: ");
        matrix_print(&r2);
    }

	// On compare nos deux résultats.
    if(matrix_compare(&r1, &r2)){
        n_puts("Looks fine for me."); // On a gagné !
    }else{
        n_fprintf(stderr, "Oh my... ! This is not working at all!\n"); // :(
    }

	// On oublie pas de libérer nos matrices.
    matrix_free(&m);
    matrix_free(&r1);
    matrix_free(&r2);

	// Fin.
    return 0;
}
