#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "lexicon.h"

#define ROWS 30
#define COLS 30
#define EMPTY_CELL '-'






// --------------------------------------------------
// Types et structures : pour eviter beaucoup de variable en entrée en entré des fonctions
// --------------------------------------------------
typedef enum {
    NO_DIRECTION = 0,
    VERTICAL = 1,
    HORIZONTAL = 2
} Direction;

typedef struct {
    int index_mot;   // indice de la lettre dans le mot
    int row;         // position dans la grille
    int col;
} MatchInfo;

// --------------------------------------------------
// Initialisation et affichage grille
// --------------------------------------------------
void initialize_grid(char grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            grid[i][j] = EMPTY_CELL;
}

void print_grid(char grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++)
            printf("%c ", grid[i][j]);
        printf("\n");
    }
}

// --------------------------------------------------
// Vérification débordement (horizontale et verticale)
// --------------------------------------------------
int debordement_horizontal(MatchInfo m, int long_mot) {
    int debut_col = m.col - m.index_mot;               //cacule de la colonne de depart du mot
    int fin_col= debut_col + long_mot - 1;            //calcule de la colonne d'arrivé du mot      
    return (debut_col < 0 || fin_col>= COLS);         //test de debordement à gauche et à droite   
}

int debordement_vertical(MatchInfo m, int long_mot) {
    int debut_ligne = m.row - m.index_mot;
    int fin_ligne = debut_ligne + long_mot - 1;
    return (debut_ligne < 0 || fin_ligne>= ROWS);
}

// --------------------------------------------------
// Vérification placement possible ;
// --------------------------------------------------
Direction placement_possible(char grid[ROWS][COLS], const char* word, MatchInfo m) {
    int L = strlen(word);
    int r_match = m.row;
    int c_match = m.col;
    int k = m.index_mot;

    // --- Vertical ---
    int r_start = r_match - k;
    if (!debordement_vertical(m, L)) {
        bool ok = true;

        if ( grid[r_start - 1][c_match] != EMPTY_CELL) ok = false; // on verifie si la case au dessus de la première lettre est vide 
        if ( grid[r_start + L][c_match] != EMPTY_CELL) ok = false; // on verifie si la case en dessus de la dernière lettre est vide 
         

        //on verifie si toutes les cases où sera loger le mot est vide ou est égale à la lettre du mot, aussi si les cases d'à cotés sont aussi vides 
        for (int i = 0; i < L && ok; i++) {
            int r = r_start + i;
            int c = c_match;
            if ( grid[r][c] != EMPTY_CELL && grid[r][c] != word[i] ) ok = false;
            if ( i == k ) continue;                                                //pour eviter d'effectuer le test sur la ligne du mot sur la grille
            if ( grid[r][c - 1] != EMPTY_CELL ) ok = false;
            if ( grid[r][c + 1] != EMPTY_CELL ) ok = false;
        }
        if (ok) return VERTICAL;
    }

    // --- Horizontal --- (meme logique que pour le "verticale")
    int c_start = c_match - k;
    if (!debordement_horizontal(m, L)) {
        bool ok = true;

        if ( grid[r_match][c_start - 1] != EMPTY_CELL ) ok = false;
        if ( grid[r_match][c_start + L] != EMPTY_CELL ) ok = false;

        for (int i = 0; i < L && ok; i++) {
            int r = r_match;
            int c = c_start + i;
            if ( grid[r][c] != EMPTY_CELL && grid[r][c] != word[i] ) ok = false;
            if ( i == k ) continue;
            if ( grid[r - 1][c] != EMPTY_CELL) ok = false;
            if ( grid[r + 1][c] != EMPTY_CELL) ok = false;
        }
        if (ok) return HORIZONTAL;
    }

    return NO_DIRECTION;
}

// --------------------------------------------------
// Placement du mot
// --------------------------------------------------
bool place_word(char grid[ROWS][COLS], const char* word, MatchInfo m, Direction dir) {
    int L = strlen(word);
    int r_start = m.row;
    int c_start = m.col;
    int k = m.index_mot;

    if (dir == VERTICAL) {
        r_start -= k;
        for (int i = 0; i < L; i++) grid[r_start + i][c_start] = word[i]; 
        return true;
    } else if (dir == HORIZONTAL) {
        c_start -= k;
        for (int i = 0; i < L; i++) grid[r_start][c_start + i] = word[i];
        return true;
    } 
    return false;
}


//j'ai péféré faire une fonction à part pour le placement du premier car j'ai décidé de chosisir le premier mot au hasard 
bool place_first_word(char grid[ROWS][COLS], const char* word, int row, int col, Direction dir) {
    int L = strlen(word);
    if (dir == HORIZONTAL) {
        if (col + L > COLS) return false;
        for (int i = 0; i < L; i++) grid[row][col + i] = word[i];
    } else {
        if (row + L > ROWS) return false;
        for (int i = 0; i < L; i++) grid[row + i][col] = word[i];
    }
    return true;
}


void dummy_crossword_horizontal(char grid[ROWS][COLS], char lexicon[MAX_WORDS][MAX_WORD_SIZE], int lexicon_size) {
    for (int i = 0; i < lexicon_size && i < ROWS; i++) {
        place_first_word(grid, lexicon[i], i, 0, HORIZONTAL);
    }
}

void dummy_crossword_vertical(char grid[ROWS][COLS], char lexicon[MAX_WORDS][MAX_WORD_SIZE], int lexicon_size) {
    for (int j = 0; j < lexicon_size && j < COLS; j++) {
        place_first_word(grid, lexicon[j], 0, j, VERTICAL);
    }
}


// --------------------------------------------------
// fonction qui verifiera si finalement un mot respecté toutes les conditions
// --------------------------------------------------
bool gen_crossword(char grid[ROWS][COLS], const char* word) {
    int L = strlen(word);

    // Parcourir toutes les lettres du mot
    for (int i = 0; i < L; i++) {
        char target = word[i];

        // Parcourir toute la grille
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (grid[r][c] == target) {
                    MatchInfo m = {i, r, c};
                    Direction d = placement_possible(grid, word, m);
                    if (d != NO_DIRECTION) {
                     place_word(grid, word, m, d);
                        return true; // placé avec succès
                    }
                }
            }
        }
    }
    return false; // impossible de placer
}




int main() {
    srand(time(NULL));
    char grid[ROWS][COLS];
    char lexicon[MAX_WORDS][MAX_WORD_SIZE];
    int lexicon_size;

    read_lexicon("lexique", lexicon, &lexicon_size);
    initialize_grid(grid);

    // Premier mot aléatoire
    int rand_index = rand() % lexicon_size;
    const char* first_word = lexicon[rand_index];
    int debut_ligne = ROWS / 2;
    int debut_col = (COLS - strlen(first_word)) / 2;

    while (!place_first_word(grid, first_word, debut_ligne, debut_col, HORIZONTAL)) {
        rand_index = rand() % lexicon_size;
        first_word = lexicon[rand_index];
    }

    printf("Premier mot placé : %s\n", first_word);
    print_grid(grid);

    // Placer tous les mots du lexique (sauf le premier déjà placé)
for (int i = 0; i < lexicon_size; i++) {

    // sauter le premier mot déjà placé
    if (strcmp(lexicon[i], first_word) == 0)
        continue;

    if (gen_crossword(grid, lexicon[i])) {
        printf("Mot placé : %s\n", lexicon[i]);
    } else {
        printf("Impossible de placer : %s\n", lexicon[i]);
    }
}

print_grid(grid);

// printf("la nombre de mot dans le lexique : %d\n",lexicon_size);

// dummy_crossword_horizontal(grid, lexicon, lexicon_size) ;
//  print_grid(grid);

// dummy_crossword_vertical(grid, lexicon, lexicon_size) ;
//  print_grid(grid);





    return 0;
}

