#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 30
#define MAX_WORDS 10

#define ROWS 15            // nombre de lignes de la grille
#define COLS 15            // nombre de colonnes de la grille
#define EMPTY_CELL '.'     // caractère représentant une case vide



/* Loads a lexicon from a file.
 *    filename: the path to the file containing the lexicon
 *    lexicon: contains the lexicon when function terminates
 *    lexicon_size: contains the number of words of the lexicon when function terminates
 */
void read_lexicon(const char* filename, char lexicon[MAX_WORDS][MAX_WORD_SIZE], int* lexicon_size) {
  FILE* lex_file;

  if((lex_file=fopen(filename, "r")) == NULL) {
    fprintf(stderr,"Cannot open file %s\n", filename);
    exit(1);
  }
  *lexicon_size=0;
  while(*lexicon_size<MAX_WORDS && fscanf(lex_file, "%s",lexicon[*lexicon_size])!=EOF)
    (*lexicon_size)++;
}
typedef enum { HORIZONTAL, VERTICAL } Direction;

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

#include <string.h>
#include <stdbool.h>

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
    char grid[ROWS][COLS];
    initialize_grid(grid);

    // Premier mot à placer
    const char* first_word = "banana";

    // Placer horizontalement au centre de la grille
    int start_row = ROWS / 2;
    int start_col = (COLS - strlen(first_word)) / 2;

    if (place_first_word(grid, first_word, start_row, start_col, HORIZONTAL)) {
        printf("Premier mot placé : %s\n", first_word);
    } else {
        printf("Impossible de placer le premier mot.\n");
    }

    display_grid(grid);

    return 0;
}




