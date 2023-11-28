#ifndef EXO_H
#define EXO_H

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <openssl/sha.h>

//EXERCICE I
int is_prime_naive(long p);
long modpow_naive(long a, long m, long n);
long modpow(long a, long m, long n);
int witness(long a, long b, long d, long p);
long rand_long(long low, long up);
int is_prime_miller(long p, int k);
long random_prime_number(int low_size, int up_size, int k);



//EXERCICE II
void print_long_vector(long *result, int size);
long extended_gcd(long s, long t, long* u, long* v);
void generate_keys(long p, long q, long* n, long* s, long* u);
long* encrypt(char* chaine, long s, long n);
char* decrypt(long* crypted, int size, long u, long n);



//EXERCICE III
typedef struct key {
    long v;
    long n;
} Key;

void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);

typedef struct sign {
    int size;
    long* content;
} Signature;

Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* key);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);

typedef struct protected {
    Key* pKey;
    char* mess;
    Signature* sgn;
} Protected;

Protected* init_protected(Key* pKey, char* mess, Signature* sgn);
int verify(Protected* pr);
char* protected_to_str(Protected* pr);
Protected* str_to_protected(char* str);


//EXERCICE IV
void generate_random_data(int nv, int nc);


//EXERCICE V
typedef struct cellKey {
    Key* data;
    struct cellKey* next; 
} CellKey;

CellKey* create_cell_key(Key* key);
void ajout_tete(CellKey** list, Key* key);
CellKey* read_public_keys(char* fichier);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* c);


typedef struct cellProtected {
    Protected* data;
    struct cellProtected* next;
} CellProtected;

CellProtected* create_cell_protected(Protected* pr);
void tete_protected(CellProtected** list, Protected* pr);
CellProtected* read_protected(char* fichier);
void print_list_protected(CellProtected* LCP);
void delete_cell_protected(CellProtected* c);
void delete_list_protected(CellProtected* c);


//EXERCICE VI
void fraude(CellProtected** LCP);

typedef struct hashcell {
    Key* key;
    int val;
} HashCell;

typedef struct hashtable {
    HashCell** tab;
    int size;
} HashTable;

HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);


//EXERCICE VII
typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
} Block;

void ecriture_fichier(Block* b, char* fichier);
Block* lecture_fichier(char* fichier);
char* block_to_str(Block* block);
unsigned char* hache(char* s);
void compute_proof_work(Block *B,int d);
int verify_block(Block* b,int d);
void delete_block(Block* b);
//EXERCICE VIII



typedef struct block_tree_cell{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
} CellTree;

CellTree* create_node(Block *b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_node(CellTree * node, int etage);
void print_tree(CellTree* cell);
void delete_node(CellTree* node);
void delete_tree(CellTree* tree);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected* fusion_liste(CellProtected* l1,CellProtected* l2);
CellProtected* longueur_max(CellTree* tree);


void submit_vote(Protected* p);
void create_block(CellTree* tree, Key* author, int d);
void add_block(int d, char* name);
CellTree* read_tree();
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC,int sizeV);

//EXERCICE IX
#endif
