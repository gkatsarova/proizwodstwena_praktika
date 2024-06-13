#include <stdio.h>
#include <stdlib.h>
#define PLAYERS_ARRAY_SIZE 13

struct Card {
    char value;
    char color;
};

int find_winner_by_4_cards(struct Card players[][PLAYERS_ARRAY_SIZE]) {
    int count[4][PLAYERS_ARRAY_SIZE] = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < PLAYERS_ARRAY_SIZE; j++) {
            int value = players[i][j].value;
            count[i][value]++;
            if (count[i][value] == 4) return i + 1;
        }
    }

    return -1;
}

int find_winner_by_value(struct Card players[][PLAYERS_ARRAY_SIZE]){
    int result[4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < PLAYERS_ARRAY_SIZE; j++) {
            result[i] = result[i] + players[i][j].value;
        }
    }

    int max = result[0];
    int winner;
    for (int i = 1; i < 4; i++) {
        if (result[i] > max) {
            max = result[i];
            winner = i + 1;
        }
    }
    return winner;
}

int game(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file");
        return -1;
    }

    struct Card players[4][PLAYERS_ARRAY_SIZE];

    char value;
    char color;
    for (int i = 0; fscanf(file, " %c %c", &value, &color) == 2; i = (i + 1) % 4) {
        if (value == 'J') value = 10;
        else if (value == 'Q') value = 12;
        else if (value == 'K') value = 15;
        else if (value == 'A') value = 20;
        else value -= '0';

        players[i][i].value = value;
        players[i][i].color = color;

        printf("player[%d] value %d color %c\n", i + 1, value, color);
    }

    fclose(file);

    int winner = find_winner_by_4_cards(players);
    if(winner = -1) winner = find_winner_by_value(players);
    return winner;
}

int main() {
    int winner = game("teste.txt");

    if (winner == 1) printf("The winner is player1!\n");
    else if (winner == 2) printf("The winner is player2!\n");
    else if (winner == 3) printf("The winner is player3!\n");
    else printf("The winner is player4!\n");
    

    return 0;
}
