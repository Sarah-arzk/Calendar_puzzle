#include "calendrier_puzzle_projet.h"

// compteur threads
long long g_nombre_solutions = 0;
pthread_mutex_t mutex_solutions = PTHREAD_MUTEX_INITIALIZER;




// DÉFINITION PIECES


const uint64_t PIECES[NB_PIECES] = {
    PIECE_1, PIECE_2, PIECE_3, PIECE_4, PIECE_5,
    PIECE_6, PIECE_7, PIECE_8, PIECE_9, PIECE_10
};




// CALENDRIER


const uint64_t CALENDRIER =
    BIT(0,3) | BIT(0,4) | BIT(0,5) | BIT(0,6) | BIT(0,7) |
    BIT(1,7) | BIT(2,7) | BIT(3,7) | BIT(4,7) | BIT(5,7) |
    BIT(6,0) | BIT(6,7) | BIT(7,0) | BIT(7,7);



    
// POSITIONS MOIS


const uint64_t POS_MOIS[13] = {
    0,
    BIT(6,1), BIT(6,2), BIT(6,3), BIT(6,4), BIT(6,5), BIT(6,6),
    BIT(7,1), BIT(7,2), BIT(7,3), BIT(7,4), BIT(7,5), BIT(7,6)
};




// POSITIONS SEMAINE


const uint64_t POS_SEMAINE[8] = {
    0,
    BIT(0,0), BIT(0,1), BIT(0,2),
    BIT(1,0), BIT(1,1), BIT(1,2), BIT(1,3)
};




// POSITIONS JOURS


const uint64_t POS_JOURS[32] = {
    0,
    BIT(1,4), BIT(1,5), BIT(1,6),
    BIT(2,0), BIT(2,1), BIT(2,2), BIT(2,3), BIT(2,4), BIT(2,5), BIT(2,6),
    BIT(3,0), BIT(3,1), BIT(3,2), BIT(3,3), BIT(3,4), BIT(3,5), BIT(3,6),
    BIT(4,0), BIT(4,1), BIT(4,2), BIT(4,3), BIT(4,4), BIT(4,5), BIT(4,6),
    BIT(5,0), BIT(5,1), BIT(5,2), BIT(5,3), BIT(5,4), BIT(5,5), BIT(5,6)
};
