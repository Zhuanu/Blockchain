#include "exo.h"
CellKey* create_cell_key(Key* key) {
    //La fonction permet d'allouer et initialiser un cellKey
    CellKey* cKey = (CellKey*)malloc(sizeof(CellKey));
    cKey->data = key;
    cKey->next = NULL;
    return cKey;
}

void tete_keys(CellKey** list, Key* key) {
    //La fonction permet de ajouter une clé en tête de liste chaînée de clé
    CellKey* cKey = create_cell_key(key);
    cKey->next = *list;
    *list = cKey;
}

CellKey* read_public_keys(char* fichier) {
    //La fonction permet de retourner une liste chaînée contenant toutes les clés publiques du fichier
    if (strcmp(fichier,"candidates.txt") != 0 && strcmp(fichier,"keys.txt") != 0) {
        printf("Ce fichier ne supporte que candidates.txt et keys.txt !\n");
        return NULL;
    }
    FILE* file = fopen(fichier, "r");
    if (!file) {
        printf("----- Erreur d'ouverture de flux -----\n");
        return NULL;
    }
    CellKey* cKey = NULL;
    char ligne[64];
    while (fgets(ligne, 64, file)) {
        char public[16];
        char prive[16];
        // lit la ligne, affecte la clé publique à public et la clé privée à prive
        // si le fichier est candidates prive sera vide
        sscanf(ligne, "%s %s", public, prive);
        Key* pKey = str_to_key(public);
        tete_keys(&cKey, pKey);
    }
    fclose(file);
    return cKey;
}

void print_list_keys(CellKey* LCK) {
    //La fonction permet afficher une liste chainée de clés
    CellKey* cour = LCK;
    int i = 1;
    while (cour) {
        char* s = key_to_str(cour->data);
        printf("-- %d %s --\n", i, s);
        // Evite les fuites mémoires engendrées par key_to_str
        free(s);
        cour = cour->next;
        i++;
    }
    printf("\n");
}

void delete_cell_key(CellKey* c) {
    //La fonction permet de supprimer une cellule de liste chainée de clés
    if (!c) {
        printf("Pas de clé !\n");
        return;
    }
    free(c->data);
    free(c);
}

void delete_list_keys(CellKey* c) {
    //La fonction permet de supprimer une liste chaînée de clés.
    if (!c) {
        printf("Pas de clé !\n");
        return;
    }
    CellKey* tmp = c;
    while (c) {
        tmp = c->next;
        delete_cell_key(c);
        c = tmp;
    }
}

CellProtected* create_cell_protected(Protected* pr) {
    //La fonction permet d'allouer et initialiser une cellule de liste chaînée
    CellProtected* cp = (CellProtected*)malloc(sizeof(CellProtected));
    cp->data = pr;
    cp->next = NULL;
    return cp;
}

void tete_protected(CellProtected** list, Protected* pr) {
    //La fonction permet de ajouter une déclaration signée en tête de liste
    CellProtected* cp = create_cell_protected(pr);
    cp->next = *list;
    *list = cp;
}

CellProtected* read_protected(char* fichier) {
    //La fonction lit un fichier et crée une liste contenant toutes les déclarations signées du fichier
    FILE* file = fopen(fichier, "r");
    if (!file) {
        printf("----- Erreur d'ouverture de flux -----\n");
        return NULL;
    }
    CellProtected* cp = NULL;
    char ligne[512];
    while (fgets(ligne, 512, file)) {
        char sgn[256];
        char pKey[256];
        char candidat[256];
        // extrait la clé publique, le message et la signature de la ligne
        sscanf(ligne, "%s %s %s", pKey, candidat, sgn);
        char* declaration = (char*)malloc(300);
        // On initialise declaration et on affecte les informations extraites dedans
        sprintf(declaration, "%s %s %s", pKey, candidat, sgn);
        // On transforme declaration en Protected* et on l'ajoute en tête de liste
        Protected* pr = str_to_protected(declaration);
        tete_protected(&cp, pr);
        free(declaration);
    }
    fclose(file);
    return cp;
}

void print_list_protected(CellProtected* LCP) {
    //La fonction permet d'afficher une liste chainée
    CellProtected* cour = LCP;
    int i = 1;
    while (cour) {
        char* s = protected_to_str(cour->data);
        printf("------- %d %s -------\n", i, s);
        i++;
        // Evite les fuites mémoires créer par la fonction protected_to_str
        free(s);
        cour = cour->next;
    }
    printf("\n");
}

void delete_cell_protected(CellProtected* c) {
    //La fonction permet de supprimer une cellule de liste chaînée de déclarations signées.
    if (!c) {
        printf("Pas de clé !\n");
        return;
    }
    free(c->data->pKey);
    free(c->data->mess);
    free(c->data->sgn->content);
    free(c->data->sgn);
    free(c->data);
    free(c);
}

void delete_list_protected(CellProtected* c) {
    //La fonction permet de supprmier une liste chaînée
    if (!c) {
        printf("Pas de déclaration !\n");
        return;
    }
    CellProtected* tmp = c;
    while (c) {
        tmp = c->next;
        delete_cell_protected(c);
        c = tmp;
    }
}
