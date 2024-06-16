#include <stdio.h>
#include <stdlib.h>
#define PLAYERS_ARRAY_SIZE 13
#define NUM_PLAYERS 4

struct Card {
    char value;
    char color;
};

int card_value(char value) {
    if (value >= '1' && value <= '9') return value - '0';
    if (value == 'J') return 10;
    if (value == 'Q') return 12;
    if (value == 'K') return 15;
    if (value == 'A') return 20;
}

int color_value(char color) {
    if(color == 'C') return 1;
    else if(color == 'D') return 2;
    else if(color == 'H') return 3;
    else return 4;
 }

int check_winner_by_4cards(int count[NUM_PLAYERS][13], struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE]) {
    int winner = -1;

    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < 13; j++) {
            if (count[i][j] == 4) {
                int card_value_winner = j;

                if (winner != -1) {
                    int current_value = card_value(players[winner][0].value);
                    if (card_value_winner > current_value) {
                        winner = i;
                    }
                } else {
                    winner = i;
                }

                break;
            }
        }
    }

    return winner;
}
int game(char* fileName, struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE], int player_card_count[NUM_PLAYERS]) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int count[NUM_PLAYERS][13] = {0};
    int winner_by_4_cards = -1;
    char value, color;
    int cards = 0;
    while(fscanf(file, "%c %c ", &value, &color) == 2){
        int player_index = cards % NUM_PLAYERS;
        int card_index = player_card_count[player_index];
        
        players[player_index][card_index].value = value;
        players[player_index][card_index].color = color;
        player_card_count[player_index]++;

        int value_index = card_value(value);
        count[player_index][value_index]++;
        cards++;

        if(cards % NUM_PLAYERS == 0){ 
            winner_by_4_cards = check_winner_by_4cards(count, players);
            if (winner_by_4_cards != -1) {
                fclose(file);
                return winner_by_4_cards;
            }
        }
    }

    fclose(file);

    int result[NUM_PLAYERS] = {0};
    int color_scores[NUM_PLAYERS] = {0};
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < player_card_count[i]; j++) {
            result[i] += card_value(players[i][j].value);
            color_scores[i] += color_value(players[i][j].color);
        }
    }

    int max_score = result[0];
    int winner = 0;
    int tie = 0;
    for (int i = 1; i < NUM_PLAYERS; i++) {
        if (result[i] > max_score) {
            max_score = result[i];
            winner = i;
            tie = 0;
        } else if (result[i] == max_score) {
            tie = 1;
        }
    }

    if (tie) {
        int max_color_score = color_scores[winner];
        for (int i = 0; i < NUM_PLAYERS; i++) {
            if (result[i] == max_score && color_scores[i] > max_color_score) {
                winner = i;
                max_color_score = color_scores[i];
            }
        }
    }

    return winner;
}

void sort_cards(struct Card cards[PLAYERS_ARRAY_SIZE], int turns) {
    for (int i = 0; i < turns; i++) {
        for (int j = i + 1; j < turns; j++) {
            if (card_value(cards[i].value) < card_value(cards[j].value) ||
                (card_value(cards[i].value) == card_value(cards[j].value) &&
                 color_value(cards[i].color) < color_value(cards[j].color))) {
                struct Card tmp = cards[i];
                cards[i] = cards[j];
                cards[j] = tmp;
            }
        }
    }
}

void print_winners_array(int winner_index, struct Card cards[PLAYERS_ARRAY_SIZE], int turns) {
    printf("Player %d wins: ", winner_index + 1);
    sort_cards(cards, turns);
    for (int i = 0; i < turns; i++) {
        printf("(%c %c); ", cards[i].value, cards[i].color);
    }
    printf("\n");
}

int main() {
    struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE] = {0};
    int player_card_count[NUM_PLAYERS] = {0};
    int winner_index = game("teste.txt", players, player_card_count);
    
    print_winners_array(winner_index, players[winner_index], player_card_count[winner_index]);
    
    return 0;
}