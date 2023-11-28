#include "exo.h"

void fraude(CellProtected** LCP) {
    //La fonction permet de supprimer toutes les déclarations dont la signature n'est pas valide
    CellProtected* cour = *LCP;
    CellProtected* last = NULL;
    CellProtected* trash = NULL;
    while (cour) {
        if (!verify(cour->data)) {
            if (last == NULL) {
                *LCP = cour->next;
            }
            else {
                last->next = cour->next;
            }
            trash = cour;
            cour = cour->next;
            delete_cell_protected(trash);
        }
        else {
            last = cour;
            cour = cour->next;
        }
    }
}

HashCell* create_hashcell(Key* key) {
    //La fonction permet d'allouer et initialiser une cellule de la tables de hachage à 0
    HashCell* c = (HashCell*)malloc(sizeof(HashCell));
    c->key = key;
    c->val = 0;
    return c;
} 

int hash_function(Key* key, int size) {
    // La fonction est une fonction de hachage qui permet de savoir la position d'un élément dans la table de hachage
    return (key->v + key->n) % size;
}

int find_position(HashTable* t, Key* key) {
    //La fonction retourne la position de key dans le table hachage t si l'élément est trouvé, et retourne la position où il aurait dû être si elle n'est pas dans la table t
    int index = hash_function(key, t->size);
    for (int i = index+1; i != index; i++) {
        if (i == t->size) i = 0;
        HashCell* cell = t->tab[i];
        if (cell == NULL) {
            return i;
        }
        else {
            if (key->v == t->tab[i]->key->v && key->n == t->tab[i]->key->n) {
                return i;
            }
        }
    }
    return -1;
}

HashTable* create_hashtable(CellKey* keys, int size) {
    //La fonction permet de crée et initialisaer une table de hachage de taille size contenant une cellule pour chaque clé de la liste chaînée keys
    // Mettre le cas ou nombres de keys > size !!
    HashTable* t = (HashTable*)malloc(sizeof(HashTable));
    t->tab = (HashCell**)malloc(sizeof(HashCell*) * size);
    t->size = size;
    for (int i = 0; i < t->size; i++) {
        t->tab[i] = NULL;
    }
    CellKey* cour = keys;
    while(cour) {
        int index = find_position(t, cour->data);
        t->tab[index] = create_hashcell(cour->data);
        cour = cour->next;
    }
    return t;
}

void delete_hashtable(HashTable* t) {
    //La fonction permet de supprimer une table de hachage 
    for(int i = 0; i < t->size; i++) {
        free(t->tab[i]);
    }
    free(t->tab);
    free(t);
} 

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
    //La fonction permet de calculer le vainqueur de l'élection
    HashTable* Hv = create_hashtable(voters, sizeV);
    printf("\n");
    HashTable* Hc = create_hashtable(candidates, sizeC);
    fraude(&decl);
    while (decl) {
        Key* cand = str_to_key(decl->data->mess);
        int indexHc = find_position(Hc, cand);
        int indexHv = find_position(Hv, decl->data->pKey);
        HashCell* candidat =  Hc->tab[indexHc];
        HashCell* voteur = Hv->tab[indexHv];
        if (candidat && voteur->val == 0) {
            voteur->val = 1;
            candidat->val++;
        }
        free(cand);
        decl = decl->next;
    }
    int max = 0;
    Key* vainqueur = NULL;
    for (int i = 0; i < Hc->size; i++) {
        HashCell* cell = Hc->tab[i];
        if (cell != NULL && cell->val > max) {
            max = cell->val;
            vainqueur = cell->key;
        }
    }
    char* nom = key_to_str(vainqueur);
    printf("---- Vainqueur : %s avec %d votes ! ----\n", nom, max);
    delete_hashtable(Hv);
    delete_hashtable(Hc);
    free(nom);
    return vainqueur;
}
