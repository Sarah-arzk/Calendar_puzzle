#include "calendrier_puzzle_projet.h"
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>

// ordre des pièces 
int order[NB_PIECES];

// compteur global
extern long long g_nombre_solutions;
extern pthread_mutex_t mutex_solutions;



// STRUCT PLACEMENTS (GLOBAL)

typedef struct {
    uint64_t placements[512];
    int count;
} PiecePlacements;

PiecePlacements all[NB_PIECES][8];



// NORMALISATION

uint64_t normalise_piece(uint64_t p)
{
    if (p == 0) return 0;

    int min_row = 8, min_col = 8;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p & BIT(i,j)) {
                if (i < min_row) min_row = i;
                if (j < min_col) min_col = j;
            }
        }
    }

    uint64_t res = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p & BIT(i,j)) {
                res |= BIT(i - min_row, j - min_col);
            }
        }
    }

    return res;
}



// ROTATION

uint64_t rotation_90(uint64_t piece)
{
    uint64_t res = 0;
    piece = normalise_piece(piece);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (piece & BIT(i,j)) {
                res |= BIT(j, 7 - i);
            }
        }
    }

    return normalise_piece(res);
}



// SYMÉTRIE

uint64_t symetrie_horizontale(uint64_t p)
{
    uint64_t res = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p & BIT(i,j)) {
                res |= BIT(i, 7 - j);
            }
        }
    }

    return normalise_piece(res);
}



// DIMENSIONS

void compute_dimensions(uint64_t p, uint8_t *h, uint8_t *l)
{
    int min_r = 8, max_r = -1;
    int min_c = 8, max_c = -1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p & BIT(i,j)) {

                if (i < min_r) min_r = i;
                if (i > max_r) max_r = i;

                if (j < min_c) min_c = j;
                if (j > max_c) max_c = j;
            }
        }
    }

    *h = max_r - min_r + 1;
    *l = max_c - min_c + 1;
}



// GENERATE PLACEMENTS

void generate_all_placements(uint64_t piece, PiecePlacements *pp)
{
    pp->count = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            uint64_t res = 0;
            int valid = 1;

            for (int r = 0; r < 8 && valid; r++) {
                for (int c = 0; c < 8; c++) {

                    if (piece & BIT(r,c)) {

                        int nr = r + i;
                        int nc = c + j;

                        if (nr >= 8 || nc >= 8) {
                            valid = 0;
                            break;
                        }

                        res |= BIT(nr, nc);
                    }
                }
            }

            if (valid) {
                pp->placements[pp->count++] = res;
            }
        }
    }
}



// INIT PIECES

void init_pieces(pieces_struct *ps)
{
    for (int i = 0; i < NB_PIECES; i++) {

        uint64_t base = PIECES[i];
        uint64_t variants[8];

        variants[0] = normalise_piece(base);
        variants[1] = rotation_90(variants[0]);
        variants[2] = rotation_90(variants[1]);
        variants[3] = rotation_90(variants[2]);

        uint64_t sym = symetrie_horizontale(base);

        variants[4] = normalise_piece(sym);
        variants[5] = rotation_90(variants[4]);
        variants[6] = rotation_90(variants[5]);
        variants[7] = rotation_90(variants[6]);

        int unique = 0;

        for (int r = 0; r < 8; r++) {

            int deja = 0;

            for (int u = 0; u < unique; u++) {
                if (variants[r] == ps->piece[i][u]) {
                    deja = 1;
                    break;
                }
            }

            if (!deja && unique < 8) {

                ps->piece[i][unique] = variants[r];

                generate_all_placements(variants[r], &all[i][unique]);

                compute_dimensions(
                    variants[r],
                    &ps->hauteur[i][unique],
                    &ps->largeur[i][unique]
                );

                unique++;
            }
        }

        ps->nb_rotations[i] = unique;
    }
}



// SORT PIECES

void sort_pieces(pieces_struct *ps)
{
    for (int i = 0; i < NB_PIECES; i++)
        order[i] = i;

    for (int i = 0; i < NB_PIECES - 1; i++) {
        for (int j = i + 1; j < NB_PIECES; j++) {

            int ci = 0, cj = 0;

            for (int r = 0; r < ps->nb_rotations[order[i]]; r++)
                ci += all[order[i]][r].count;

            for (int r = 0; r < ps->nb_rotations[order[j]]; r++)
                cj += all[order[j]][r].count;

            if (cj < ci) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
        }
    }
}



// BACKTRACKING CLASSIQUE

void solve(uint64_t grid, int depth, pieces_struct *ps)
{
    if (depth == NB_PIECES) {

        pthread_mutex_lock(&mutex_solutions);
        g_nombre_solutions++;
        pthread_mutex_unlock(&mutex_solutions);

        return;
    }

    int p = order[depth];

    for (int r = 0; r < ps->nb_rotations[p]; r++) {

        PiecePlacements *pp = &all[p][r];

        for (int k = 0; k < pp->count; k++) {

            uint64_t placed = pp->placements[k];

            if (placed & grid) continue;

            solve(grid | placed, depth + 1, ps);
        }
    }
}



// THREAD DATA

typedef struct {
    uint64_t grid;
    int piece;
    int rotation;
    int start;
    int end;
    pieces_struct *ps;
} ThreadData;



// THREAD FUNCTION

void *thread_solve(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    PiecePlacements *pp = &all[data->piece][data->rotation];

    for (int k = data->start; k < data->end; k++) {

        uint64_t placed = pp->placements[k];

        if (placed & data->grid) continue;

        solve(data->grid | placed, 1, data->ps);
    }

    free(data);
    return NULL;
}



// SOLVE PARALLEL (ENTRY POINT)

void solve_parallel(uint64_t grid, pieces_struct *ps)
{
    pthread_t threads[64];
    int tcount = 0;

    int p = order[0];

    for (int r = 0; r < ps->nb_rotations[p]; r++) {

        PiecePlacements *pp = &all[p][r];

        int chunk = pp->count / 4;
        if (chunk == 0) chunk = 1;

        for (int t = 0; t < 4; t++) {

            ThreadData *data = malloc(sizeof(ThreadData));

            data->grid = grid;
            data->piece = p;
            data->rotation = r;
            data->start = t * chunk;
            data->end = (t == 3) ? pp->count : (t + 1) * chunk;
            data->ps = ps;

            pthread_create(&threads[tcount++], NULL, thread_solve, data);
        }
    }

    for (int i = 0; i < tcount; i++) {
        pthread_join(threads[i], NULL);
    }
}
