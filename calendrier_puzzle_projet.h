#ifndef CALENDRIER_PUZZLE_PROJET_H
#define CALENDRIER_PUZZLE_PROJET_H

#include <stdint.h>
#include <pthread.h>

#define NB_PIECES 10
#define BIT(row, col) (1ULL << ((row) * 8 + (col)))


// PIECES

#define PIECE_1   (BIT(0,0) | BIT(0,1) | BIT(0,2) | BIT(1,1) | BIT(2,1))
#define PIECE_2   (BIT(0,1) | BIT(0,2) | BIT(1,0) | BIT(1,1))
#define PIECE_3   (BIT(0,1) | BIT(0,2) | BIT(1,0) | BIT(1,1) | BIT(1,2))
#define PIECE_4   (BIT(0,0) | BIT(0,1) | BIT(0,2) | BIT(1,0) | BIT(2,0))
#define PIECE_5   (BIT(0,0) | BIT(0,1) | BIT(1,0) | BIT(2,0) | BIT(2,1))
#define PIECE_6   (BIT(0,0) | BIT(1,0) | BIT(2,0) | BIT(3,0) | BIT(3,1))
#define PIECE_7   (BIT(0,0) | BIT(1,0) | BIT(2,0) | BIT(2,1))
#define PIECE_8   (BIT(0,0) | BIT(0,1) | BIT(1,1) | BIT(2,1) | BIT(2,2))
#define PIECE_9   (BIT(0,0) | BIT(0,1) | BIT(1,1) | BIT(1,2) | BIT(1,3))
#define PIECE_10  (BIT(0,0) | BIT(0,1) | BIT(0,2) | BIT(0,3))

// tableau pièces
extern const uint64_t PIECES[NB_PIECES];



// STRUCT PIECES

typedef struct {
    uint64_t piece[NB_PIECES][8];
    uint8_t largeur[NB_PIECES][8];
    uint8_t hauteur[NB_PIECES][8];
    int nb_rotations[NB_PIECES];
} pieces_struct;



// FONCTIONS CORE

void init_pieces(pieces_struct *ps);
void solve(uint64_t grid, int index, pieces_struct *ps);
void solve_parallel(uint64_t grid, pieces_struct *ps);

uint64_t normalise_piece(uint64_t p);
uint64_t rotation_90(uint64_t piece);
void compute_dimensions(uint64_t p, uint8_t *h, uint8_t *l);
void sort_pieces(pieces_struct *ps);



// CALENDRIER GRID

extern const uint64_t CALENDRIER;
extern const uint64_t POS_MOIS[13];
extern const uint64_t POS_SEMAINE[8];
extern const uint64_t POS_JOURS[32];



// GLOBAL THREAD SAFE

extern long long g_nombre_solutions;
extern pthread_mutex_t mutex_solutions;

#endif
