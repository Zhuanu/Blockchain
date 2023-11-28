#include "exo.h"
void print_long_vector(long *result, int size) {
    printf("Vector : [");
    for (int i = 0; i < size; i++) {
        printf("%lx \t", result[i]);
    }
    printf("]\n");
}
long extended_gcd(long s, long t, long* u, long* v) {
    if (s == 0) {
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t % s, s, &uPrim, &vPrim);
    *u = vPrim - (t/s) * uPrim;
    *v = uPrim;
    return gcd;
}

void generate_keys(long p, long q, long* n, long* s, long* u) {
     //La fonction permet de générer les valeurs s et u des clé publique et privé à partir des nombres premier p  et q.
    *n = p * q;
    long t = (p-1) * (q-1);
    long v;
    *s = rand_long(2, t-1);
    long gcd = extended_gcd(*s, t, u, &v);
    // Pour quitter la boucle il faut que l'entier s générer soit premier, que son GCD = 1,
    // que le paramètre u pour la clé secrète soit positive et que la clé secrète et publique soit differentes.
    // sinon on continue à générer un nouvel entier et on applique le gcd dessus
    while(!is_prime_miller(*s, 1000) || gcd != 1 || v >= 0 || *s == *u) {
        *s = rand_long(2, t-1);
        gcd = extended_gcd(*s, t, u, &v);
    }
}

long* encrypt(char* chaine, long s, long n) {
    //La fonction permet de chiffer la chaîne de caractère chaine avec la clé publique (s,n)
    int taille = strlen(chaine);
    long* msg = (long*)malloc(sizeof(long)*taille);
    int i = 0;
    while(chaine[i]){
        msg[i] = modpow(chaine[i], s, n);
        i++;
    }
    return msg;
}

char* decrypt(long* crypted, int size, long u, long n) {
    //La fonction retourne le message déchiffrer depuis un tableau de long et de la clé secret(u,n)
    char* msg = (char*)malloc(sizeof(char)*size);
    for (int i = 0; i < size; i++) {
        msg[i] = modpow(crypted[i], u, n);
    }
    msg[size] = '\0';
    return msg;
}
