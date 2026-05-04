#include "calendrier_puzzle_projet.h"
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>


// AFFICHAGE GRILLE

void print_board(uint64_t board)
{
    printf("\nBOARD\n");

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            if (board & BIT(i, j))
                printf("1 ");
            else
                printf(". ");
        }
        printf("\n");
    }

    printf("\n");
}



// AFFICHAGE PIECES

void print_pieces(const pieces_struct *ps)
{
    printf("\nPIECES\n");

    for (int p = 0; p < NB_PIECES; p++) {

        printf("\nPiece %d\n", p + 1);

        uint64_t piece = ps->piece[p][0];

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                if (piece & BIT(i, j))
                    printf("1 ");
                else
                    printf(". ");
            }
            printf("\n");
        }
    }

    printf("\n");
}



// MAIN

int main()
{
    int jour, mois, weekday;

    pieces_struct ps;

    // init pièces
    init_pieces(&ps);

    // optimisation ordre pièces
    sort_pieces(&ps);

    printf("PUZZLE CALENDRIER\n");
    printf("Entrez la date (jour mois weekday[1-7]) : ");
    scanf("%d %d %d", &jour, &mois, &weekday);

    // validation input
    if (jour < 1 || jour > 31 ||
        mois < 1 || mois > 12 ||
        weekday < 1 || weekday > 7) {

        printf("Date invalide\n");
        return 1;
    }

    // construction grille
    uint64_t grid = CALENDRIER;

    grid |= POS_JOURS[jour];
    grid |= POS_MOIS[mois];
    grid |= POS_SEMAINE[weekday];

    print_board(grid);

    // timer
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    g_nombre_solutions = 0;

    
    // SOLVER MULTITHREAD
    
    solve_parallel(grid, &ps);

    gettimeofday(&t2, NULL);

    double temps =
        (t2.tv_sec - t1.tv_sec) +
        (t2.tv_usec - t1.tv_usec) / 1000000.0;

    // résultat
    printf("\nRESULTATS\n");
    printf("Nombre de solutions : %lld\n", g_nombre_solutions);
    printf("Temps d'execution   : %.3f secondes\n", temps);

    return 0;
}
