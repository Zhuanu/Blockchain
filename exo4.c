#include "exo.h"
void generate_random_data(int nv, int nc) {
    // La fonction permet de génèrer des clés différentes et de créer trois fichiers qui contiennent respectivement
    // les couples de clé, la clé publique de tous les candidats et toutes les déclaration signées.
    if (nc > nv) {
        printf("nc > nv donc 'Erreur de géneration' !!\n");
        return ; // Plus de candidats que votants ce qui va créer une boucle infini
    }
    srand(time(NULL));
    FILE* KEYS = fopen("keys.txt", "w");
    if (!KEYS) {
        printf("--- Erreur d'ouverture de flux ---\n");
        return ;
    }
    // On va afficher la carte électorale des électeurs dans le fichier keys.txt et les
    // affecter dans le tableau keys pour que ca soit plus rapide de récupérer les candidats 
    // parmi les électeurs (je crois?)
    char** keys = (char**)malloc(sizeof(char*)*nv); // création du tableau keys
    for (int i = 0; i < nv; i++) {
        Key* pKey = (Key*)malloc(sizeof(Key)); 
        Key* sKey = (Key*)malloc(sizeof(Key));
        init_pair_keys(pKey, sKey, 3, 7); // création des clés publique et privée
        char* public = key_to_str(pKey); // les transformer en str pour les affecter à keys.txt et au tableau keys
        char* prive = key_to_str(sKey); 
        // la boucle imbriquée sert à vérifier si l'électeur a déjà été généré pour éviter
        // d'avoir 2 fois le même électeur
        int j;
        for (j = 0; j < i; j++) {
            char* test = (char*)malloc(2048);
            sprintf(test, "%s %s", public, prive);
            // Si la carte électorale se trouve dans keys alors on "reset" une itération
            // pour avoir exactement le nombre d'électeur demandé par generate_random_data
            if (strcmp(keys[j], test) == 0) {
                i--;
                free(test);
                break;
            }
            free(test);
        }
        // j == i donc l'électeur n'est pas présent dans keys, on peut donc l'ajouter
        // et passer au prochain électeur à générer
        if (j == i) {
            keys[i] = malloc(2048);
            fprintf(KEYS, "%s %s\n", public, prive);
            sprintf(keys[i], "%s %s", public, prive);
        }
        // Eviter les fuites memoires, on les recrée à chaque fois 
        // en ligne 318-319 et 321-322
        free(pKey);
        free(sKey);
        free(public);
        free(prive);
    }
    fclose(KEYS);

    // même principe que keys.txt pour affecter dans candidates.txt et le tableau candidats
    FILE* CANDIDATS = fopen("candidates.txt", "w");
    char** candidats = (char**)malloc(sizeof(char*)*nc);
    for (int i = 0; i < nc; i++) {
        char* public = (char*)malloc(16);
        char* prive = (char*)malloc(16);
        // on choisi un candidat au hasard parmi les électeurs
        int index = rand() % nv;
        sscanf(keys[index], "%s %s", public, prive);
        int j;
        for (j = 0; j < i; j++) {
            if (strcmp(candidats[j], public) == 0) {
                i--;
                break;
            }
        }
        if (j == i) {
            candidats[i] = malloc(2048);
            fprintf(CANDIDATS, "%s\n", public);
            sprintf(candidats[i], "%s", public);
        }
        free(public);
        free(prive);
    }
    fclose(CANDIDATS);


    FILE* VOTE = fopen("declarations.txt", "w");
    for (int i = 0; i < nv; i++) {
        // L'électeur vote pour un candidat au hasard
        int index = rand() % nc;
        char* public = (char*)malloc(32);
        char* prive = (char*)malloc(32);
        sscanf(keys[i], "%s %s", public, prive); // Récupère la clé publique et privée de l'électeur
        Key* sKey = str_to_key(prive); 
        Signature* s = sign(candidats[index], sKey); // Crypte le message avec la clé privée
        char* vote = signature_to_str(s); 
        // Affiche la clé publique de l'électeur, son message, et sa signature dans déclarations.txt
        fprintf(VOTE, "%s %s %s\n", public, candidats[index], vote);
        // Eviter les fuites mémoires
        free(vote);
        free(s->content); free(s);
        free(sKey);
        free(prive);
        free(public);
    }
    fclose(VOTE);

    // Désalloue le tableau keys
    for (int i = 0; i < nv; i++) {
        free(keys[i]);
    }
    free(keys);

    // Désalloue le tableau candidats
    for (int i = 0; i < nc; i++) {
        free(candidats[i]);
    }
    free(candidats);
}
