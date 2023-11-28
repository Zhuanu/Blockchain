#include "exo.h"

CellTree* create_node(Block* b){
    //La fonction permet de créer et initialiser un noeud avec une hauteur égale à 0.
    CellTree* c = (CellTree*)malloc(sizeof(CellTree));//allocation dynammique pour la celltree de retour
    c->block = b;
    c->father = NULL;
    c->firstChild = NULL;
    c->nextBro = NULL;
    c->height = 0;
    return c;
}
int update_height(CellTree* father, CellTree* child){
    //La fonction permet de mettre à jour la hauteur de father quand il y a une modification d'un de ses fils
    if (child->height+1 > father->height){//on modifie la hauteur du father ssi la hauteur du fils+1 est plus grand que celle de son père
        father->height = (child->height)+1;
        return 1;//retourne 1 sn on a changer 
    }
    return 0;//retourne 0 si y a de changement

}

void add_child(CellTree* father, CellTree* child) {
    // Pas de premier fils
    if (!father->firstChild) {
        father->firstChild = child;
    }
    // Possède un premier fils
    else {
        child->nextBro = father->firstChild;
        father->firstChild = child;
    }
    child->father = father;//on met à jour le pointeur du child->father
    CellTree* cour = child;
    while (cour->father) {//permet de mettre à jour tout les ascendant du child
        update_height(cour->father, cour);
        cour = cour->father;
    }
}                                           

void print_node(CellTree* tree, int etage) {
    // fonction auxilière qui permet d'afficher un noeud et retenir sa profondeur
    if (tree != NULL) {//gestion des espace pour l'affichage
        for (int i=0; i<etage; i++) {
            printf("  ");
        }
        printf("%d %s\n", tree->height, tree->block->hash);
        print_node(tree->firstChild, etage+1);
        print_node(tree->nextBro, etage);
    }
}

void print_tree(CellTree* tree) {
    // fonction qui affiche un arbre
    print_node(tree, 0);
}


void delete_node(CellTree* node){
    //La fonction permet de supprimer un noeud de l'arbre et tout ses descendants
    if (!node) return ; //le cas si node est NULL
    // S'il possède un père
	if(node->father != NULL){//permet de faire des mise à jour des pointeur 
        // Si node est le premier fils de son père
		if(node->father->firstChild == node) {
			node->father->firstChild=node->nextBro;
            
        }
        // Sinon on parcourt la liste de ses frères jusqu'à trouver node -> update pointeur
		else {
            CellTree* tmp = node->father->firstChild;
			while (tmp->nextBro != node){
				tmp=tmp->nextBro;
			}
			tmp->nextBro=node->nextBro;
		}
	}
    // Supprime s'il n'a pas de fils
	if(!(node->firstChild)){
		delete_block(node->block);
		free(node);
	}
    // S'il a un fils, on fait la récursion sur le fils et on le supprime à la fin
    else {
        CellTree* tmp=node->firstChild;//tmp pour parcourir la liste des fils
        while(tmp){
            CellTree* tmp2 = tmp->nextBro;//tmp2 pour stocker le nextBro de tmp parce qu'on perd tmp quand on effecture delete_node(tmp)
            delete_node(tmp);//la récusion sur son fils 
            tmp=tmp2;//pour passer au frère suivant
        }
        delete_block(node->block);//supprimer le node courant après avoir supprimer tout ses déscendant
        free(node);
	}
}



void delete_tree(CellTree* tree){
    //La fonction permet de supprimer l'arbre en entier
    CellTree* tmp=tree;
    while(tmp->father){//permet d'accéder la racine 
        tmp=tmp->father;
    }
    delete_node(tmp);
}


CellTree* highest_child(CellTree* cell){
    //La fonction permet de retourner le noeud fils avec la plus grande hauteur
    //On suppose qu'il existe une unique noeud fils avec la plus grande hauteur
    if(!cell->firstChild){// si cell n'a pas de fils on renvoie null
        return NULL;
    }
    CellTree* cour = cell->firstChild;//cour permet de parcourir la liste des fils
    CellTree* max = cell->firstChild;// max permet de garder le noeud fils qui a la plus grande hauteur
    while(cour){
        if(cour->height > max->height){//le cas si on trouve un noeud qui possède une hauteur plus grande que celle du max on change max
            max = cour;
        }
        cour = cour->nextBro;//passage au frère suivant
    }
    return max;
}

CellTree* last_node(CellTree* tree){
    //La fonction permet de retourner la valeur hachée du dernier bloc de la plus longue chaîne
    if (!tree) return NULL;//le cas ou tree est null
    if (!tree->firstChild) return tree; //quand le noeud tree n'a pas de descendant alors on le renvoie
    CellTree* max = highest_child(tree);//pour descendant dans l'arbre 
    while(max->firstChild) {//sortir du boucle quand on arrive  tout en bas de l'arbre
        max = highest_child(max);//à chaque fois on prend le max des fils 
    }
    return max;
}


CellProtected* fusion_liste(CellProtected* l1, CellProtected* l2){
    //La fonction permet de fusion deux liste 
    CellProtected* res = NULL;//permet de copier les deux liste de déclaration 
    CellProtected* tmp = l1;//variable pour parcourir la liste l1
    while(tmp){//
        tete_protected(&res, tmp->data);//ajoute en tête
        tmp = tmp->next;
    }
    CellProtected* cour = l2;//variable pour parcourir la liste l2
    while (cour) {
        tete_protected(&res, cour->data);//ajoute en tête
        cour = cour->next;
    }
    return res;
}

// Renvoie la liste de déclaration des blocks se situant dans la plus longue chaine
CellProtected* longueur_max(CellTree* tree) {
    CellProtected* chaine = tree->block->votes;//valeur de retour
    CellTree* max = highest_child(tree);//max pour parcourir dans la chaine la plus longue 
    while (max) {
        chaine = fusion_liste(max->block->votes, chaine);// on fait fusion du liste des votes du noeud courant(le max) avec la chaine 
        max = highest_child(max);//on descend dans avec le plus grand des fils de max qui nous garantie de rester dans la chaine la plus longue
    }
    return chaine;
}
