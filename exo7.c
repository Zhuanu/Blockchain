#include "exo.h"
void ecriture_fichier(Block* b, char* fichier) {
    //La fonction permet d'écrire dans un ficher un block
    FILE* flux = fopen(fichier, "w");//ouverture du flux
    if (!flux) {//test de l'ouverture
        printf("Erreur d'ouverture de flux\n");
        return ;
    }
	char* authorKeys = key_to_str(b->author);
    fprintf(flux, "%s\n", authorKeys);//écriture de l'author
    fprintf(flux, "%s\n", b->hash);//écriture de la valeur haché
    fprintf(flux, "%s\n", b->previous_hash);//écriture de la valeur haché précédant
    fprintf(flux, "%d\n", b->nonce);//écriture de la valeur de nonce

    CellProtected* decl = b->votes;
    while (decl) {//on parcourt tout la liste chainée de déclaration on écrit un par un 
        char* vote = protected_to_str(decl->data);
        fprintf(flux, "%s\n", vote);
        free(vote);
        decl = decl->next;
    }
    free(authorKeys);
    fclose(flux);//fermuture du flux
}

Block* lecture_fichier(char* fichier) {
    //La fonction permet de lire un bloc depuis un fichier
    FILE* flux = fopen(fichier, "r");//ouverture du fichier
    if (!flux) {//test de l'ouverture 
        printf("Erreur d'ouverture de flux\n");
        return NULL;
    }
    Block* b = (Block*)malloc(sizeof(Block));//allocation dynamique du block 
    char author[256];
    fscanf(flux, "%s", author);//lecture de l'author
    b->author = str_to_key(author);
    b->hash = (unsigned char*)malloc(2000);
    fscanf(flux, "%s", b->hash);//lecture de la valeur hachée
    b->previous_hash = (unsigned char*)malloc(2000);
    fscanf(flux, "%s", b->previous_hash);//lecture de la valeur haché précedant
    fscanf(flux, "%d\n", &(b->nonce));//lecture de nonce
    char ligne[128];
	b->votes = NULL;
    while(fgets(ligne,128,flux)!=NULL){//lecture de tout les déclaration
		//printf("%s",ligne);
        Protected* decl = str_to_protected(ligne);
        tete_protected(&(b->votes), decl); // Eviter l'ajout en tête si y a un problème avec la liste de déclarations inversée
    }	
    fclose(flux);//fermeture du fichier
    return b;
}

char* block_to_str(Block* block) {
    char* str = (char*)malloc(10000);//allocation de str de retour problème avec realloc donc on utilise malloc avec un grand nombre
    char* author = key_to_str(block->author); //mettre le author sans la représentation chaîne de caractère
    sprintf(str, "%s %s %d ", author, block->previous_hash, block->nonce);//écrire dans str les chaîne de caractère author, hash et prrevious hash
    free(author);//gestion fuite mémoire
    CellProtected* cour = block->votes;// variable qui permet de parcourir la liste des déclarations
    while (cour) {
        char* decl = protected_to_str(cour->data);//decl permet de stocker la réprésentation en chaîne de caractère du protected 
        strcat(str,decl);//ajout dans str du protected courant
        free(decl);//gestion fuite mémoire
        cour = cour->next;//passage à protected suivant
    }
    //strcat(str, '\0');
    return str;
}


unsigned char* hache(char* chaine) {
    unsigned char *str = malloc(sizeof(unsigned char)*256);
    str[0] = '\0';
    unsigned char *d = SHA256(chaine,strlen(chaine),0);
    char c[256];
    for (int i=0; i<SHA256_DIGEST_LENGTH; i++) {
        sprintf(c, "%02x", d[i]);
        strcat(str, c);
    }
    return str;
}

void compute_proof_work(Block* B, int d) {
	B->nonce = 0;//initialisation du nonce à 0
    char* str;//str permet de stocker la réprésentation en chaîne de caractère du block B
    unsigned char* hach;//hach permet de stocker la valeur haché de B
	int i = 0;
	while(i < d){//on sort de la boucle quand i =>le nombre de d zéro
		i = 0;
		char* str = block_to_str(B);//mettre à jour le block_to_str(B) car nonce a changer
        hach = hache(str);//mettre à jour la valeur haché avec la nouvelle str
        free(str);//gestion fuite mémoire
		B->nonce++;//on incrémente b->nonce à chaque fois on effecture la fonction de hachage 
		for (int k = 0; k < d; k++){//test si la valeur haché de commence par d zero successifs si oui on incrémente i de 1 à chaque boucle, sinon on sort du fort on repart dans la boucle while
			if (hach[k] != '0') {
                free(hach);
				break;
			}
			i++;
		}
	}
	B->hash = hach;//on remplace la valeur hach de b par le valeur haché trouvé
}


int verify_block(Block* b,int d){
//la fonction retourne 1 si le block est valide,0 sinon
	unsigned char* tmp = b->hash;
	for (int i = 0; i < d; i++){
		if (tmp[i] != '0')
			return 0;
	}
	return 1;
}

void delete_block(Block* b){
    if (!b) return ;//si le cas où le block est NULL on renvoie NULL
    if(b->hash) {//on libère la valeur hachée du b si elle existe
        free(b->hash);
    }
    if(b->previous_hash) {//on libère la valeur haché précendant du b si elle existe
        free(b->previous_hash);
    }
    while (b->votes) {//on libère la liste des protected sans libèrer leur contenu
        CellProtected* tmp = b->votes;
        b->votes = b->votes->next;
        free(tmp);
    }
    free(b);//on finit par libérer b
}



