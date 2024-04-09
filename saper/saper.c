#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define ROWS 9
#define COLS 9
#define MINES 10

//Struktura przechowująca stan pola
typedef struct {
  int value;
  int is_mine;
  int is_revealed;
  int is_marked;
} Field;

//Funkcja inicjująca planszę
void initBoard(Field board[ROWS][COLS]) {
  //Ustawienie wartości pól na 0
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      board[i][j].value = 0;
      board[i][j].is_mine = 0;
      board[i][j].is_revealed = 0;
      board[i][j].is_marked = 0;
    }
  }

  //Losowanie rozmieszczenia min
  srand(time(NULL));
  int mines_placed = 0;
  while (mines_placed < MINES) {
    int row = rand() % ROWS;
    int col = rand() % COLS;
    if (!board[row][col].is_mine) {
      board[row][col].is_mine = 1;
      mines_placed++;
    }
  }

  //Obliczanie liczb wokół min
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (board[i][j].is_mine) {
        for (int k = i - 1; k <= i + 1; k++) {
          for (int l = j - 1; l <= j + 1; l++) {
            if (k >= 0 && k < ROWS && l >= 0 && l < COLS && !board[k][l].is_mine) {
              board[k][l].value++;
            }
          }
        }
      }
    }
  }
}

//Funkcja wyświetlająca planszę
void displayBoard(Field board[ROWS][COLS]) {
  printf("\n");
  printf("  ");
  for (int i = 0; i < COLS; i++) {
    printf("%d ", i);
  }
  printf("\n");

  for (int i = 0; i < ROWS; i++) {
    printf("%d ", i);
    for (int j = 0; j < COLS; j++) {
      if (!board[i][j].is_revealed) {
        if (board[i][j].is_marked) {
          printf("? ");
        } else {
          printf("# ");
        }
      } else {
        if (board[i][j].is_mine) {
          printf("* ");
        } else {
          printf("%d ", board[i][j].value);
        }
      }
    }
    printf("\n");
  }
}

//Funkcja odkrywająca pole
void revealField(Field board[ROWS][COLS], int row, int col) {
  if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
    return;
  }

  if (board[row][col].is_marked) {
    return;
  }

  if (board[row][col].is_revealed || board[row][col].is_mine) {
    return;
  }

  board[row][col].is_revealed = 1;

  if (board[row][col].value == 0) {
    revealField(board, row - 1, col - 1);
    revealField(board, row - 1, col);
    revealField(board, row - 1, col + 1);
    revealField(board, row, col - 1);
    revealField(board, row, col + 1);
    revealField(board, row + 1, col - 1);
    revealField(board, row + 1, col);
    revealField(board, row + 1, col + 1);
  }
  system("clear");
}

//funkcja oznaczajaca miny
void markField(Field board[ROWS][COLS], int row, int col) {
  if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
    return;
  }

  board[row][col].is_marked = !board[row][col].is_marked;
  system("clear");
}

//funcja sprawdzajaca wygrana
int checkWin(Field board[ROWS][COLS]) {
  int fields_revealed = 0;
  int mines_marked = 0;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (board[i][j].is_revealed) {
        fields_revealed++;
      }
      if (board[i][j].is_mine && board[i][j].is_marked) {
        mines_marked++;
      }
    }
  }
  return fields_revealed == ROWS * COLS - MINES || mines_marked == MINES;
}

int main() {
  char command1[20];
  while(1) {
    //opcje menu
    printf("Wybierz opcje:\n");
    printf("1.Graj\n");
    printf("2.Najlepszy wynik\n");
    printf("3.Wyjscie\n");
    scanf("%s", command1);
    system("clear");
    //opcja sprawdzenie najlepszego wyniku
    if ((strcmp(command1, "2") == 0) || (strcmp(command1, "2.") == 0) || (strcmp(command1, "Najlepszy wynik") == 0) || (strcmp(command1, "2.Najlepszy wynik") == 0)) {
      FILE* f = fopen("NajlepszyWynik.txt", "r+");
      char w[10];
      if (f == NULL) {
        printf("Brak najlepszego wyniku\n");
        printf("Wroc do menu wpisujac: wyjdz");
        printf("\n");
        scanf("%s", w);
        while(1){
          if ((strcmp(w, "wyjdz") == 0) || (strcmp(w, "Wyjdz") == 0)){
            system("clear");
            break;
          } else {
            printf("Niepoprawna opcja\n");
          }
        }
      } else {
        double r;
        fscanf(f, "%lf", &r);
        fclose(f);
        printf("Najlepszy wynik to: %lf \n", r);
        printf("Wroc do menu wpisujac: wyjdz");
        printf("\n");
        scanf("%s", w);
        while(1){
          if ((strcmp(w, "wyjdz") == 0) || (strcmp(w, "Wyjdz") == 0)){
            system("clear");
            break;
          } else {
            printf("Niepoprawna opcja\n");
          }
        }
      }
      //opcja graj
    } else if ((strcmp(command1, "1") == 0) || (strcmp(command1, "1.") == 0) || (strcmp(command1, "Graj") == 0) || (strcmp(command1, "1.Graj") == 0)) {
      struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    Field board[ROWS][COLS];
    initBoard(board);

    int game_over = 0;
    while (game_over == 0) {
      displayBoard(board);

      printf("Podaj polecenie (odkryj/zaznacz): ");
      char command2[10];
      scanf("%s", command2);

      if (strcmp(command2, "odkryj") == 0) {
        printf("Podaj wiersz i kolumnę: ");
        int row, col;
        scanf("%d %d", &row, &col);

        revealField(board, row, col);

        if ((board[row][col].is_mine) && (!board[row][col].is_marked)) {
          game_over = 1;
          printf("Przegrałeś!\n");
        } else {
          system("clear");
        }
        if (checkWin(board)) {
          game_over = 1;
          printf("Wygrałeś!\n");
          //mierzenie czasu i wprowadzanie najlepszego do pliku
          clock_gettime(CLOCK_MONOTONIC, &end);
          double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
          printf("Twoj czas to %f\n", elapsed_time);

          FILE* f = fopen("NajlepszyWynik.txt", "r+");
          if (f == NULL) {
            FILE* f = fopen("NajlepszyWynik.txt", "w+");
            fprintf(f, "%f", elapsed_time);
            fclose(f);
          }else {
              double N;
              fscanf(f, "%lf", &N);
              if (elapsed_time < N) {
                fclose(f);
                FILE* f = fopen("NajlepszyWynik.txt", "w");
                fprintf(f, "%f", elapsed_time);
                fclose(f);
              } else {
                fclose(f);
              }
          }
        }
      }else if (strcmp(command2, "zaznacz") == 0){
        printf("Podaj wiersz i kolumnę: ");
        int row, col;
        scanf("%d %d", &row, &col);

        markField(board, row, col);
      }
    }
    //opcja wyjscia
    } else if ((strcmp(command1, "3") == 0) || (strcmp(command1, "3.") == 0) || (strcmp(command1, "Wyjscie") == 0) || (strcmp(command1, "3.Wyjscie") == 0)) {
      break;
    } else {
      printf("Nieprawidlowa opcja\n");
    }
  }
}