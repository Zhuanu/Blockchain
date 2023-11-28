#include "exo.h"

void submit_vote(Protected* p){
	//permet de écrire dans le fichier Pending_votes.txt le vote p
	FILE *f=fopen("Pending_votes.txt","a");//ouverture du fichier pending vote avec le mode a qui permet d'écrire à la fin du fichier
	char* vote = protected_to_str(p);//vote contient la représentation en chaîne de caractère de Protected p
	fprintf(f,"%s\n", vote);//écriture du vote
	free(vote);//gestion fuite mémoire 
	fclose(f);//fermuture du flux
}

void create_block(CellTree* tree, Key* author, int d) {
	//creation block valide avec les contenu du fichier pending_votes.txt et supprimer le fichier pending_votes.txt après avoir lu et écrire  e bloc dans le fichier Pending_block
	Block* b = (Block*)malloc(sizeof(Block));//allocation du block 
	CellProtected* votes = read_protected("Pending_votes.txt");//recupérer les déclarations des votes dans  le fichier pending_vote.txt
	if (tree) {//si tree n'est pas vide on initialise le previous_hash de b à la valeur haché du dernier noeud  de  la plus longue chaîne
		b->previous_hash = last_node(tree)->block->hash;
	}
	else {//si tree est vide on le fait a NULL le previous hash
		b->previous_hash = NULL;
	}
	b->author = author;//initialisation de author avec le paramètre
	b->votes = votes;//initialisation de b->votes avec les votes lu dans pending votes.txt
	compute_proof_work(b, d);//on utlise la fonction compute_poof_work pour mettre à jour la valeur haché de b
	if (!verify_block(b, d)){//on vérifie si la block est valide sinon on renvoie un message d'erreur et on sort de la fonction.
		printf("block non valide: create block");
		return ;
	}
	remove("Pending_votes.txt");//suppression du fichier Pending_votes.txt
	ecriture_fichier(b, "Pending_block");//ecrire dans le fichier Pending_block le block b

	delete_list_protected(b->votes);
	free(b->hash);
	free(b);
}

void add_block(int d, char* name) {
//la fonction permet de vérifie si le bloc  représenté  par le fichier Pending_block est valide si oui on crée un fichier name dans le répertoire Blockchain
	Block* b = lecture_fichier("Pending_block");//b permet de stocker le block dans le pending_block
	if (verify_block(b, d)) {//vérification si b est valide 
		char dir[256] = "./Blockchain/";
		strcat(dir, name);//permet d'avoir le chemin du fichier 
		ecriture_fichier(b, dir); // S'il a été ouvert, on crée un fichier name et le contenu sera b
	}
	remove("Pending_block");//suppression du fichier Pending_block
	delete_list_protected(b->votes);
	free(b->hash);
	free(b);
}

CellTree* read_tree() {
	int taille = 0;//permet de savoir comme de place qu'on doit allouer le tableau 
	DIR* rep = opendir("./Blockchain/");
	if (rep) {
		struct dirent* dir;
		while ((dir = readdir(rep))) {
			// Ajout de DS_Store car on est sur MacOS
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".DS_Store") != 0) {
				taille++;
			}
		}
		closedir(rep);
	}
	CellTree* T[taille];//allocation statique du T;
	rep = opendir("./Blockchain/");
	if (rep) { // Si y a une réponse à l'ouverture du repertoire Blockchain
		struct dirent* dir;
		int index = 0;
		while ((dir = readdir(rep))) { // Lit la réponse et tant qu'il n'est pas nulle on continue
			char fichier[256] = "./Blockchain/";
			// Ajout de DS_Store car on est sur MacOS
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".DS_Store") != 0) { // Faut que le nom du fichier soit différent de  . && ..
				strcat(fichier, dir->d_name);
				Block* b = lecture_fichier(fichier); // Création du block avec les infos contenus dans le fichier d_name
				T[index] = create_node(b); // 1 noeud par case de T
				index++;
			}
		}
		closedir(rep);
	}
	for (int i = 0; i < taille; i++) {
		for (int j = 0; j < taille; j++) {
			if (strcmp(T[i]->block->hash, T[j]->block->previous_hash) == 0) { // Si T[i] est le père de T[j]
				add_child(T[i], T[j]); // Si oui, ajoute
			}
		}
	}
	// Recherche de la racine
	CellTree* root = T[0];
	while (root->father) {
		root = root->father;
	}
	return root;
}


Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
	//La fonction permet de calculer le vainqueur de l'élection
	CellProtected* decl = longueur_max(tree);
	fraude(&decl);
	Key* vainqueur = compute_winner(decl, candidates, voters, sizeC, sizeV);
	delete_list_protected(decl);
	return vainqueur;
}

