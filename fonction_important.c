bool can_place_word(char grid[ROWS][COLS], const char* word, int row_start, int col_start, Direction dir) {
    int word_len = strlen(word);

    // Vérifier débordement
    if (dir == HORIZONTAL && (col_start < 0 || col_start + word_len > COLS)) return false;
    if (dir == VERTICAL && (row_start < 0 || row_start + word_len > ROWS)) return false;

    // Vérifier chaque case
    for (int i = 0; i < word_len; i++) {
        int r = row_start;
        int c = col_start;

        if (dir == HORIZONTAL) c += i;
        else r += i;

        if (grid[r][c] != EMPTY_CELL && grid[r][c] != word[i]) return false;

        // Vérifier cases autour
        if (dir == HORIZONTAL) {
            if (r > 0 && grid[r-1][c] != EMPTY_CELL) return false;
            if (r < ROWS-1 && grid[r+1][c] != EMPTY_CELL) return false;
        } else {
            if (c > 0 && grid[r][c-1] != EMPTY_CELL) return false;
            if (c < COLS-1 && grid[r][c+1] != EMPTY_CELL) return false;
        }
    }

    return true;
}


//raisonnement inverse au lieu de tester  quand on peut placer le mots ; on teste quannd on ne peut pas le faire , et si toujour pas de false ; on envoie true qui signeferais que l'on peut placer le mots ; 


//il faut que tu ajoute une fonction qui se chargera de trouver , row_start, col_start; 
//la présence de la direction est aussi ambiguie (dassn ton programme ) 
