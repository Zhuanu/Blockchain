#include "exo.h"
//1.1
int is_prime_naive(long p){
    //la fonction retourne 1 si p est premier et 0 s'il n'est pas 
    if ((p < 3) || (p % 2 == 0)) { 
        return 0;
    }
    for (int i = 3; i < p; i++) {
        if (p % i == 0) {
            return 0;
        }
    }
    return 1;
}


//1.2 Sur le rapport


//1.3
long modpow_naive(long a, long m, long n) {
    //la fonction retourne le reste de la division entière de a puissante m par n
	long res = 1;
	for(int i = 0; i < m; i++){
		res = (res * a) % n;
	}
	return res;
}


//1.4
long modpow(long a, long m, long n) {
    //la fonction retourne le reste de la division entière de a puissante m par n
    if (m == 0) return 1;
	if (m == 1) return a % n;
    else {
        int b = modpow(a, m/2, n) % n;
        if (m % 2 == 0) 
            return (b * b) % n;
        else
            return (a * b * b) % n;
    }
}
//1.5

//1.6
int witness(long a, long b, long d, long p) {
    long x = modpow(a, d, p);
    if (x == 1) {
        return 0;
    }
    for(long i = 0; i < b; i++) {
        if (x == p-1) {
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up) {
    return rand() % (up - low+1) + low;
}

int is_prime_miller(long p, int k) {
    if (p == 2) {
        return 1;
    }
    if (!(p && 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d && 1)) {
        d = d/2;
        b = b + 1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a;
    int i;
    for (i = 0; i < k; i++) {
        a = rand_long(2, p-1);
        if (witness(a, b, d, p)) { 
            return 0;
        }
    }
    // probabilité d'erreur O(k)
    return 1;
}

//1.8
long random_prime_number(int low_size, int up_size, int k) {
// La fonction retourne un nombre premier de taille comprise entre low_size et up_size et on effetue k tests de Miller
    long borneInf = 1;
    long borneSup = 1;
    // calcul de la borne inférieur
    for (int i = 0; i < low_size; i++) {
        borneInf = borneInf * 2;
    }
    // calcul de la borne supérieur
    for (int i = 0; i < up_size+1; i++) {
        borneSup = borneSup * 2;
    }
    long p = rand_long(borneInf, borneSup-1);
    int i = 0;
    // On quitte pas la boucle tant qu'un p généré n'est pas premier
    while (i < 2) {
        if (!is_prime_miller(p, k)) {
            p = rand_long(borneInf, borneSup-1);
            i = 0;
        }
        i++;
    }
    return p;
}


