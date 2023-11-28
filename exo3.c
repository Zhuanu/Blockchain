#include "exo.h"

void init_key(Key* key, long val, long n) {
    //La fonction permet de initialiser une clé
    key->v = val;
    key->n = n;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size) {
    //la fonction permet de initialiser la clé publique et la clé secrète  en respectant le protocole de RSA
    long p = random_prime_number(low_size, up_size, 5000);
    long q = random_prime_number(low_size, up_size, 5000);
    while(p == q){
        q = random_prime_number(low_size, up_size, 5000); 
    }
    long n, s, u;
    generate_keys(p, q, &n, &s, &u);
    if (u < 0){
        long t = (p - 1) * (q - 1);
        u = u + t; // on aura toujours s*u mod t = 1 
    }
    init_key(pKey, s, n);
    init_key(sKey, u, n);
}

char* key_to_str(Key* key) {
    //La fonction permet de passer une variable Key à sa représentation sous forme de chaîne de caractère et la return
    char* cle = malloc(16);
    sprintf(cle, "(%lx,%lx)", key->v, key->n);
    return cle;
}

Key* str_to_key(char* str) {
    //La fonction permet de passer une chaîne de caractère à sa représentation sous forme de Key
    Key* cle = (Key*)malloc(sizeof(Key));
    sscanf(str, "(%lx,%lx)", &cle->v, &cle->n);
    return cle;
}

Signature* init_signature(long* content, int size) {
    //La fonction permet de allouer une signature et de l'initialiser avec les paramètre donnée
    Signature* sign = (Signature*)malloc(sizeof(Signature));
    sign->content = content;
    sign->size = size;
    return sign;
}


Signature* sign(char* mess, Key* key) {
    //La fonction permet de crée une signature à l'aide de la fonction encrypt
    long* content = encrypt(mess, key->v, key->n);
    return init_signature(content, strlen(mess));
}

char* signature_to_str(Signature* sgn) {
    // La foncion permet de passer d'une structure signature à sa représentation sous forme de chaîne de caractère
    char* result = malloc(10 * sgn->size);
    result[0] = '#';
    int pos = 1;
    char buffer[156];
    for (int i = 0; i < sgn->size; i++) {
        sprintf(buffer, "%lx", sgn->content[i]);
        for (int j = 0; j < strlen(buffer); j++) {
            result[pos] = buffer[j];
            pos++;
        }
        result[pos] = '#';
        pos++;
    }
    result[pos] = '\0';
    result = realloc(result, pos+1);
    return result;
}

Signature* str_to_signature(char* str) {
    //La fonction permet de passer d'une chaîne de caractère à sa représentation sous forme de Signature
    int len = strlen(str);
    long* content = (long*)malloc(sizeof(long) * len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] != '#') {
            buffer[pos] = str[i];
            pos++;
        }
        else {
            if (pos != 0) {
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num++;
                pos = 0;
            }
        }
    }
    content = realloc(content, num * sizeof(long));
    return init_signature(content, num);
}

Protected* init_protected(Key* pKey, char* mess, Signature* sgn) {
    //La fonction permet d'initialiser et allouer une structure Protected
    Protected* proc = (Protected*)malloc(sizeof(Protected));
    proc->pKey = pKey;
    proc->mess = mess;
    proc->sgn = sgn;
    return proc;
}

int verify(Protected* pr) {
    // La fonction permet de vérifie que la signature contenue dans pr correspond bien au message à la personne contenus dans pr
    char* msg = decrypt(pr->sgn->content, pr->sgn->size, pr->pKey->v, pr->pKey->n);
    if (strcmp(msg, pr->mess) == 0) {
        free(msg);
        return 1;
    }
    free(msg);
    return 0;
}

char* protected_to_str(Protected* pr) {
    //La fonction permet de passer une structure Protected à sa représention sous forme de chaîne de caractère
    char* res = (char*)malloc(2048);
    char* k = key_to_str(pr->pKey);
    char* s = signature_to_str(pr->sgn);
    sprintf(res, "%s %s %s", k, pr->mess, s);
    free(k); free(s);
    return res;
}

Protected* str_to_protected(char* str) {
    //La fonction permet de passer d'une chaîne de caractère à sa représentattion sous forme de Protected
    char* pKey = (char*)malloc(sizeof(char)*256);
    char* mess = (char*)malloc(sizeof(char)*256);
    char* sgn = (char*)malloc(sizeof(char)*256);
    sscanf(str, "%s %s %s", pKey, mess, sgn);
    Key* cle = str_to_key(pKey);
    Signature* signe = str_to_signature(sgn);
    free(pKey);
    free(sgn);
    Protected* p = init_protected(cle, mess, signe);
    return p;
}
