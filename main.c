#include "exo.h"
int main() {

/* ------------------------------------ Exercice 1 --------------------------------------- */

    /*
	clock_t temps_initial1;
	clock_t temps_final1;
	double temps_cpu1;
	int s;
	temps_initial1 = clock () ;
    s=is_prime_naive(300001969);
    temps_final1 = clock () ;
	temps_cpu1 = ((double)(temps_final1 - temps_initial1))/CLOCKS_PER_SEC;
	printf("%f\n",temps_cpu1);
	printf("%d\n",is_prime_naive(s));

    
    clock_t temps_initialNaif;
	clock_t temps_finalNaif;
	double temps_cpuNaif;

    clock_t temps_initial;
	clock_t temps_final;
	double temps_cpu;

    // echantillon pour créer la courbe de modpow et modpow naïf
    FILE *f = fopen("compmod.txt", "w");
    for(int i = 100; i < 5000; i++) {
        temps_initialNaif = clock();
        modpow_naive(2, i, 29);
        temps_finalNaif = clock();
        temps_cpuNaif = ((double)(temps_finalNaif - temps_initialNaif))/CLOCKS_PER_SEC;

        temps_initial = clock();
        modpow(2, i, 29);
        temps_final = clock();
        temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;

        fprintf(f, "%d %f %f\n", i, temps_cpuNaif, temps_cpu);
    }
    fclose(f);
    
    printf("modpow_naive : %ld\n", modpow_naive(2, 9, 29));
    printf("modpow : %ld\n", modpow(2, 9, 29));

    printf("is_prime_miller : %d\n", is_prime_miller(21, 1));
    printf("rand_long : %ld\n", rand_long(4, 31));

    printf("random_prime_number : %ld\n", random_prime_number(4, 7, 3));
    */

/* ------------------------------------ Exercice 2 --------------------------------------- */

    /*
    // Generation de cle :
    long p = random_prime_number(3, 7, 5000);
    long q = random_prime_number(3, 7, 5000);
    while(p==q){
        q = random_prime_number(3, 7, 5000); 
    }
    long n, s, u;
    generate_keys(p, q, &n, &s, &u);
    if (u<0){
        long t = (p-1)*(q-1);
        u = u+t; //on aura toujours s*u mod t = 1 
    }
    printf("cle publique = (%lx, %lx) \n", s, n);
    printf("cle privee = (%lx, %lx) \n", u, n);
    
    char message[1000] = "Hello";
    int len = strlen(message);

    // Chiffrement :
    long* crypted = encrypt(message, s, n);
    
    printf("Initial message : %s \n", message);
    printf("Encoded representation : \n");
    print_long_vector(crypted, len);

    // Dechiffrement :
    char* decoded = decrypt(crypted, len, u, n);
    printf("Decoded : %s\n", decoded);
    free(decoded);
    free(crypted);
    */

/* ------------------------------------ Exercice 3 --------------------------------------- */

    /*
    // Testing Init Keys
    Key* pKey = malloc(sizeof(Key));
    Key* sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey,3,7);
    printf("pKey : %lx , %lx \n", pKey->v, pKey->n); 
    printf("sKey : %lx , %lx \n", sKey->v, sKey->n);

    // Testing Key Serialization
    char* chaine = key_to_str(pKey); 
    printf("key to str : %s \n", chaine);
    Key* k = str_to_key(chaine);
    printf("str to key : %lx , %lx \n", k->v, k->n);

    // Testing signature
    // Candidate keys :
    Key* pKeyC = malloc(sizeof(Key));
    Key* sKeyC = malloc(sizeof(Key)); 
    init_pair_keys(pKeyC, sKeyC,3,7);

    // Declaration:
    char* mess = key_to_str(pKeyC);
    printf("%s vote pour %s\n", chaine, mess);
    Signature* sgn = sign(mess, sKey);
    printf("signature : "); 
    print_long_vector(sgn->content,sgn->size);
    char* signStr = signature_to_str(sgn);
    printf("signature to str : %s \n", signStr);
    Signature* strSign = str_to_signature(signStr);
    printf("str to signature : "); 
    print_long_vector(sgn->content,sgn->size);

    // Testing protected :
    Protected* pr = init_protected(pKey, mess, sgn);

    // Verification :
    if (verify(pr)) printf("Signature valide\n");
    else printf("Signature non valide\n");
    char* c3 = protected_to_str(pr);
    printf("protected to str : %s\n", c3);
    Protected* pr1 = str_to_protected(c3);
    char* key = key_to_str(pr1->pKey);
    char* s = signature_to_str(pr1->sgn);
    printf("str to protected : %s %s %s\n", key, pr1->mess, s);

    // Fuite mémoire du main fourni
    free(pKey); free(sKey); free(pKeyC); free(sKeyC); 
    free(k); free(chaine); free(mess); 
    free(sgn->content); free(sgn); free(signStr); free(strSign->content); free(strSign); free(pr); free(c3);
    free(pr1->mess); free(pr1->pKey); free(pr1->sgn->content); free(pr1->sgn); free(pr1);
    free(key); free(s);
    */

/* ------------------------------------ Exercice 4 --------------------------------------- */

    // generate_random_data(100, 2);

/* ------------------------------------ Exercice 5 --------------------------------------- */

    /*
    CellKey* cKey = read_public_keys("candidates.txt");
    print_list_keys(cKey);
    delete_list_keys(cKey);


    CellProtected* cp = read_protected("declarations.txt");
    print_list_protected(cp);
    delete_list_protected(cp);
    */

/* ------------------------------------ Exercice 6 --------------------------------------- */

    /*
    // teste seulement la question 6.1 pour déterminer les fraudes

    CellProtected* cp = read_protected("declarations.txt");
    print_list_protected(cp);
    fraude(&cp);
    printf("\n\n\n");
    print_list_protected(cp);
    
    
    printf("\n\n\n");
    tete_protected(&cp, str_to_protected("(e3,323) (329,caf) #141#9c#24e#244#f7#ed#"));
    tete_protected(&cp, str_to_protected("(222b,3523) (329,caf) #1145#1dff#11ad#1f10#"));
    tete_protected(&cp, str_to_protected("(e3,323) (329,caf) #141#9c#24e#244#108#226#121#f71"));
    tete_protected(&cp, str_to_protected("(ef,afd) (bf5,34b5) #6c1#857#527#3d5#666#37c#2ec#857#"));
    print_list_protected(cp);
    
    fraude(&cp);
    printf("\n\n");
    print_list_protected(cp);
    delete_list_protected(cp);
    */
    /*
    // Renvoie le vainqueur de l'élection

    generate_random_data(100, 2);
    CellKey* candidats = read_public_keys("candidates.txt");
    print_list_keys(candidats);
    printf("\n");
    CellKey* voteurs = read_public_keys("keys.txt");
    print_list_keys(voteurs);
    printf("\n");
    CellProtected* cp = read_protected("declarations.txt");
    print_list_protected(cp);
    printf("\n");

    compute_winner(cp, candidats, voteurs, 3, 100);

    // Pour régler les fuites mémoires
    delete_list_keys(voteurs);
    delete_list_keys(candidats);
    delete_list_protected(cp);
    */

/* ------------------------------------ Exercice 7 --------------------------------------- */

    /*
    // Pour tester block_to_str
    Block *b = (Block *)malloc(sizeof(Block));
    generate_random_data(20, 2);
    b->votes = read_protected("declarations.txt");
    fraude(&(b->votes));
    b->author = (Key *)malloc(sizeof(Key));
    init_key(b->author,551,288);
    compute_proof_work(b, 2);
    char* block = block_to_str(b);
    printf("\nBlock to str = %s\n\n", block);
    printf("Verify block = %d\n", verify_block(b, 2));
    // Il reste que les protected à desallouer
    free(block);
    free(b->author);
    delete_block(b);
    */

    /*
    // courbe du proof of work en fonction de d
    clock_t temps_initial ;
	clock_t temps_final ;
	double temps_cpu ;
	FILE *f = fopen("courbes_compute_proof_of_work.txt","w");
	for(int i = 0; i < 6; i++){
		temps_initial = clock ();
		compute_proof_work(b, i);
		temps_final = clock ();
		temps_cpu = (( double ) ( temps_final - temps_initial ) ) / CLOCKS_PER_SEC ;
		fprintf(f,"%d %f",i ,temps_cpu);
	}
	fclose(f);
    */

/* ------------------------------------ Exercice 8 --------------------------------------- */

    /*
    CellTree** T = (CellTree**)malloc(sizeof(CellTree*)*8);
    for (int i = 0; i < 8; i++) {
        Block* b = (Block*)malloc(sizeof(Block));
        generate_random_data(3, 1);
        b->votes=read_protected("declarations.txt");
        fraude(&(b->votes));
        b->author = malloc(sizeof(Key));
        init_key(b->author, i, i);
        compute_proof_work(b, 2);
        
        T[i] = create_node(b);
  
    }
    add_child(T[0], T[5]); 
    add_child(T[0], T[4]);  
    add_child(T[0], T[3]); 
    add_child(T[0], T[1]); 
    add_child(T[1], T[2]); 
    add_child(T[5], T[6]); 
    add_child(T[6], T[7]);
    
    printf("ARBRE SANS MODIFICATION\n");
    print_tree(T[0]);
    printf("\n\n");
    
    
    printf("ARBRE en supprimant T[1] et T[7]\n");
    delete_node(T[1]);//T[1] le firstchild de t[0]
    delete_node(T[7]);//T[7] le first child de T[6]
    print_tree(T[0]);
     printf("\n\n");
     
     
    //delete_tree(T[7]);// permet de tester si la suppression de l'arbre dont la racine est T[0]
    //print_tree(T[0]); //normalement print_tree(T[0]) pose un problème de segmentation car avec delete_tree on l'a supprimé
    printf("\n\n");
    
    
    printf("le fils de T[0]avec la plus grande hauteur en supprimant T[1] et T[7]\n");
    print_tree(highest_child(T[0]));// il affiche le fils de T[0] avec la plus grand auteur normalemnt c'est T[5]
    printf("\n");
    printf("valeur haché de T[5]:\n%s\n",T[5]->block->hash);
    printf("\n\n");
    
    
    printf("Le last_node de la plus longue chaîne est T[0]en supprimant T[1] et T[7]\n");
    print_tree(last_node(T[0]));//normalment après la suppression de T[1] et T[7] le dernier noeud de la plus longue chaîne est T[6] 
    printf("\n");
    printf("valeur haché de T[6]:\n%s\n",T[6]->block->hash);//valeur haché de T[0] pour comparer avec ce qu'on trouve avec la fonction lastnode
    printf("\n\n");
    
    
    printf("La liste obtenue par fusion des listes chaînées de déclarations contenue de la plus longue chaîne\n");
    CellProtected* a = longueur_max(T[0]);//c'est la fusion des déclartion de T[0] T[5] et T[6]
    print_list_protected(a);
    printf("\n");
    printf("list protected de T[0]\n");
    print_list_protected(T[0]->block->votes);
    printf("list protected de T[5]\n");
    print_list_protected(T[5]->block->votes);
    printf("list protected de T[6]\n");
    print_list_protected(T[6]->block->votes);

    printf("\n\n");
    //pour libérer la  mémoire :
    //delete_node(T[0]);
    //free(T);
    */
    
/* ------------------------------------ Exercice 9 --------------------------------------- */

	/*
	//9.1
	
	generate_random_data(3, 1);
   	CellProtected* tmp = read_protected("declarations.txt");
    while (tmp) {
   		submit_vote(tmp->data);
    	tmp = tmp->next;
    }// Pour vérifier on regarde dans le fichier Pending_votes.txt si les Protected de déclaration est bien ajouter
    
   
    //9.2
	create_block(T[0], str_to_key("(1,1)"), 2);
	//Pour vérifier on regarder si le Pending_votes.txt est supprimé et si on obtenu un fichier Pending_block qui contient un block
	
	
	
	//9.3
	add_block(2, "c.txt");
	//Pour vérifier on va dans le répertoire Blockchain et on regarde si le fichier c.txt existe et dans si dans le fichier on a bien le block qu'on a vu dans le 9.2
	*/

    
	//9.4-9.5-9.6
	
	generate_random_data(20, 5);
    CellProtected* decl= read_protected("declarations.txt");
    CellKey* voters = read_public_keys("keys.txt");
    CellKey* candidates = read_public_keys("candidates.txt");
    CellTree* tree = NULL;
    int i = 1;
    char* j = malloc(2048);
    CellProtected* declarations = decl;
    while (declarations) {
        if (i == 10) {
            i = 0;
            Key* author = declarations->data->pKey;
            create_block(tree, author, 3);
            if (tree == NULL) {
                tree = create_node(lecture_fichier("Pending_block"));
            } else {
                add_child(last_node(tree), create_node(lecture_fichier("Pending_block")));
            }
            strcat(j, "k");
            add_block(3, j);
        }
        submit_vote(declarations->data);
        i++;
        declarations = declarations->next;
    }
    CellTree* read_tr = read_tree();
    print_tree(read_tr);
    print_tree(tree);
    compute_winner_BT(tree, candidates, voters, 50, 3000);
    

    // Désallocation de la mémoire
    free(j);
    delete_list_keys(candidates);
    delete_list_keys(voters);
    delete_list_protected(decl);
    delete_tree(tree);
    delete_tree(read_tr);

	return 0;
}



