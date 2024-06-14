#include <stdio.h>
#include <stdlib.h>
#define PLAYERS_ARRAY_SIZE 13
#define NUM_PLAYERS 4

struct Card {
    char value;
    char color;
};

int find_winner_by_4_cards(struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE]) {
    int count[NUM_PLAYERS][PLAYERS_ARRAY_SIZE] = {0};

    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < PLAYERS_ARRAY_SIZE; j++) {
            int value = players[i][j].value;
            count[i][value]++;
            if (count[i][value] == 4) return i + 1;
        }
    }

    return -1;
}

int find_winner_by_value(struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE]){
    int result[NUM_PLAYERS] = {0};
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < PLAYERS_ARRAY_SIZE; j++) {
            result[i] = result[i] + players[i][j].value;
        }
    }

    int max = result[0];
    int winner;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (result[i] > max) {
            max = result[i];
            winner = i + 1;
        }
    }
    return winner;
}

int game(char* fileName, struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE]) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file");
        exit(-1);
    }

    char value;
    char color;
    int cards = 0;
    while (fscanf(file, " %c %c", &value, &color) == 2) {
        int player_index = cards % NUM_PLAYERS;
        int card_index = cards / NUM_PLAYERS;

        if (value == 'J') value = 10;
        else if (value == 'Q') value = 12;
        else if (value == 'K') value = 15;
        else if (value == 'A') value = 20;
        else value -= '0';

        players[player_index][card_index].value = value;
        players[player_index][card_index].color = color;

        //printf("player[%d] value %d color %c\n", player_index + 1, value, color);
        cards++;
    }

    fclose(file);

    int winner = find_winner_by_4_cards(players);
    if(winner == -1) winner = find_winner_by_value(players);
    return winner;
}

int check_value(int value){
    if(value >= 1 && value <= 9) return (value + '0');
    else if(value == 10) return 'J';
    else if(value == 12) return 'Q';
    else if(value == 15) return 'K';
    else if(value == 20) return 'A';

}

void print_winners_array(int winner, struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE]) {
    int flag = 0;
    for(int i = 1; i < winner + 1; i++){
        if (winner == i) {
            flag = 1;
            printf("Player %d wins: ", winner);
            for (int i = 0; i < PLAYERS_ARRAY_SIZE; i++) {
                players[winner][i].value = check_value(players[winner][i].value);
                printf("(%c) (%c); ", players[winner][i].value, players[winner][i].color);
            }
            printf("\n");
        } 
        if(flag == 1) break;
    }
}

int main() {
    struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE] = {0};
    int winner = game("teste.txt", players);
    
    print_winners_array(winner, players);
    return 0;
}
