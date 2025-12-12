#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "lexicon.h"



#define ROWS 15            // nombre de lignes de la grille
#define COLS 15            // nombre de colonnes de la grille
#define EMPTY_CELL '.'     // caractère représentant une case vide


//fonction dpour trouver une lettre dans la grille coorspondant à la lettre du mots à placer 

typedef struct {
    int found;       // 0 = rien trouvé, 1 = trouvé
    int index_mot;   // indice de la lettre dans le mot
    int row;         // position dans la grille
    int col;
} MatchInfo;

MatchInfo find_first_match(char grid[ROWS][COLS], const char* word) {
    MatchInfo info;
    
    info.found = 0;
    info.index_mot = -1;
    info.row = -1;
    info.col = -1;

    int word_len = strlen(word);

    // Pour chaque lettre du mot à placer
    for (int i = 0; i < word_len; i++) {

        char target = word[i];

        // Parcourir toute la grille pour trouver une correspondance
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {

                if (grid[r][c] == target) {

                    // On a trouvé !
                    info.found = 1;
                    info.index_mot = i;
                    info.row = r;
                    info.col = c;

                    return info; // on arrête immédiatement
                }
            }
        }
        // si aucune lettre trouvée pour ce i → on passe à la lettre suivante
    }

    return info; // si rien trouvé du tout
}









//fonctionn de verification de debordememt horizontale et verticale 
int overflow_horizontal(MatchInfo m, int word_len) {
    int start_col = m.col - m.index_mot;              // première lettre du mot
    int end_col   = start_col + (word_len - 1);       // dernière lettre

    if (start_col < 0) return 1;                      // dépasse à gauche
    if (end_col >= COLS) return 1;               // dépasse à droite

    return 0; // pas de débordement
}

// ----------------------------------------------------------
// Vérifie si un mot déborde en vertical après placement
// ----------------------------------------------------------
int overflow_vertical(MatchInfo m, int word_len) {
    int start_row = m.row - m.index_mot;              // première lettre
    int end_row   = start_row + (word_len - 1);       // dernière lettre

    if (start_row < 0) return 1;                      // dépasse en haut
    if (end_row >= ROWS) return 1;               // dépasse en bas

    return 0; // pas de débordement
}








//initialisation de la grille
void initialize_grid(char grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = EMPTY_CELL;
        }
    }
}

// Fonction pour afficher la grille
void display_grid(char grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}




typedef enum {
    NO_DIRECTION = 0,
    VERTICAL = 1,
    HORIZONTAL = 2
} Direction;



Direction can_place_from_match(char grid[ROWS][COLS], const char* word, MatchInfo m) {
    int L = strlen(word);

    int row_match = m.row;
    int col_match = m.col;
    int k         = m.index_mot; 

    /* ============================================================
       1) TEST VERTICAL
       ============================================================ */

    int row_start_vertical = row_match - k;

    if (!overflow_vertical(m, L)) {

        int ok = 1;

        if (row_start_vertical > 0 &&
            grid[row_start_vertical - 1][col_match] != EMPTY_CELL)
            ok = 0;

        if (row_start_vertical + L < ROWS &&
            grid[row_start_vertical + L][col_match] != EMPTY_CELL)
            ok = 0;

        for (int i = 0; i < L && ok; i++) {

            int r = row_start_vertical + i;
            int c = col_match;

            if (grid[r][c] != EMPTY_CELL && grid[r][c] != word[i])
                ok = 0;

            if (i == k) continue;

            if (c > 0 && grid[r][c-1] != EMPTY_CELL) ok = 0;
            if (c < COLS-1 && grid[r][c+1] != EMPTY_CELL) ok = 0;
        }

        if (ok) return VERTICAL;
    }

    /* ============================================================
       2) TEST HORIZONTAL
       ============================================================ */

    int col_start_horizontal = col_match - k;

    if (!overflow_horizontal(m, L)) {

        int ok = 1;

        if (col_start_horizontal > 0 &&
            grid[row_match][col_start_horizontal - 1] != EMPTY_CELL)
            ok = 0;

        if (col_start_horizontal + L < COLS &&
            grid[row_match][col_start_horizontal + L] != EMPTY_CELL)
            ok = 0;

        for (int i = 0; i < L && ok; i++) {

            int r = row_match;
            int c = col_start_horizontal + i;

            if (grid[r][c] != EMPTY_CELL && grid[r][c] != word[i])
                ok = 0;

            if (i == k) continue;

            if (r > 0 && grid[r-1][c] != EMPTY_CELL) ok = 0;
            if (r < ROWS-1 && grid[r+1][c] != EMPTY_CELL) ok = 0;
        }

        if (ok) return HORIZONTAL;
    }

    return NO_DIRECTION;
}



//placement du mots correspond 

bool place_word_from_match(char grid[ROWS][COLS],
                           const char* word,
                           MatchInfo m,
                           Direction dir)
{
    int L = strlen(word);
    int k = m.index_mot;   // position de la lettre matchée dans le mot

    int row_start = m.row;
    int col_start = m.col;

    if (dir == VERTICAL) {
        /* On veut que :
              row_start + k == position de la lettre matchée dans la grille
           Donc :
              row_start_final = m.row - k
        */
        row_start = m.row - k;
        col_start = m.col;

        // sécurité débordement
        if (row_start < 0 || row_start + L > ROWS)
            return false;

        // Placement du mot verticalement
        for (int i = 0; i < L; i++)
            grid[row_start + i][col_start] = word[i];

        return true;
    }

    else if (dir == HORIZONTAL) {
        /* On veut :
              col_start + k == position de la lettre matchée
           Donc :
              col_start_final = m.col - k
        */
        col_start = m.col - k;
        row_start = m.row;

        // sécurité débordement
        if (col_start < 0 || col_start + L > COLS)
            return false;

        // Placement du mot horizontalement
        for (int j = 0; j < L; j++)
            grid[row_start][col_start + j] = word[j];

        return true;
    }

    return false;  // direction invalide
}














bool place_first_word(char grid[ROWS][COLS], const char* word, int row, int col, Direction dir) {
    int word_len = strlen(word);

    // Vérifier si le mot rentre dans la grille
    if (dir == HORIZONTAL) {
        if (col + word_len > COLS) return false;  // débordement
        for (int j = 0; j < word_len; j++)
            grid[row][col + j] = word[j];        // placer les lettres
    } else { // VERTICAL
        if (row + word_len > ROWS) return false;  // débordement
        for (int i = 0; i < word_len; i++)
            grid[row + i][col] = word[i];        // placer les lettres
    }

    return true;
}



int main() {
  srand(time(NULL)) ; 
    char grid[ROWS][COLS];
    char lexicon[MAX_WORDS][MAX_WORD_SIZE];
    int lexicon_size; 
    
    
    const char* word = "carnavale";
     int L = strlen(word);

    read_lexicon("lexique",lexicon,&lexicon_size);
    initialize_grid(grid);
    
    int  rand_index = rand() % lexicon_size;    
    const char* first_word = lexicon[rand_index];

    // Premier mot à placer
    // Placer horizontalement au centre de la grille
    int start_row = ROWS / 2;
    int start_col = (COLS - strlen(first_word)) / 2;
     
     while(!place_first_word(grid, first_word, start_row, start_col, HORIZONTAL)){
           rand_index = rand() % lexicon_size;    
           first_word = lexicon[rand_index];

    } 
     printf("Premier mot placé : %s\n", first_word);
    display_grid(grid);
 
    MatchInfo info =  find_first_match(grid,word) ;

  

    printf("found      = %d\n", info.found);
    printf("index_mot  = %d\n", info.index_mot);
    printf("row        = %d\n", info.row);
    printf("col        = %d\n", info.col);
    
    
    printf("Test placement du mot '%s'\n", word);

    // Test Horizontal
    if (overflow_horizontal(info, L))
        printf("-> Debordement horizontal détecté\n");
    else
        printf("-> Pas de debordement horizontal\n");

    // Test Vertical
    if (overflow_vertical(info, L))
        printf("-> Debordement vertical détecté\n");
    else
        printf("-> Pas de debordement vertical\n");
    

    

Direction d = can_place_from_match(grid, word, info);

if (d == NO_DIRECTION) {
    printf("Impossible de placer le mot %s\n", word);
} 
else if (d == VERTICAL) {
    printf("Le mot %s peut etre place verticalement\n", word);
    place_word_from_match(grid, word, info, d);

}
else if (d == HORIZONTAL) {
    printf("Le mot %s peut etre place horizontalement\n", word);
     place_word_from_match(grid, word, info, d);

}

display_grid(grid);


    return 0;
}




