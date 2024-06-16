#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PLAYERS_ARRAY_SIZE 13
#define NUM_PLAYERS 4

struct Card
{
    char value;
    char color;
};

int card_value(char value)
{
    if (value >= '1' && value <= '9')
        return value - '0';
    if (value == 'J')
        return 10;
    if (value == 'Q')
        return 12;
    if (value == 'K')
        return 15;
    if (value == 'A')
        return 20;
}

int color_value(char color)
{
    if (color == 'C')
        return 1;
    else if (color == 'D')
        return 2;
    else if (color == 'H')
        return 3;
    else if (color == 'S')
        return 4;
}

int check_winner_by_4cards(int count[NUM_PLAYERS][PLAYERS_ARRAY_SIZE], struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE])
{
    int winner = -1;

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        for (int j = 0; j < PLAYERS_ARRAY_SIZE; j++)
        {
            if (count[i][j] == 4)
            {
                int card_value_winner = j + 1;

                if (winner != -1)
                {
                    int current_value = card_value(players[winner][0].value);
                    if (card_value_winner > current_value)
                    {
                        winner = i;
                    }
                }
                else
                {
                    winner = i;
                }

                break;
            }
        }
    }

    return winner;
}
int game(char *fileName, struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE], int player_card_count[NUM_PLAYERS])
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    int count[NUM_PLAYERS][PLAYERS_ARRAY_SIZE] = {0};
    int winner_by_4_cards = -1;
    char value, color;
    int cards = 0;
    while (fscanf(file, "%c %c ", &value, &color) == 2)
    {
        int player_index = cards % NUM_PLAYERS;
        int card_index = player_card_count[player_index];

        players[player_index][card_index].value = value;
        players[player_index][card_index].color = color;
        player_card_count[player_index]++;

        int value_index = card_value(value);
        count[player_index][value_index]++;
        cards++;

        if (cards % NUM_PLAYERS == 0)
        {
            winner_by_4_cards = check_winner_by_4cards(count, players);
            if (winner_by_4_cards != -1)
            {
                fclose(file);
                return winner_by_4_cards;
            }
        }
    }

    fclose(file);

    int result[NUM_PLAYERS] = {0};
    int color_scores[NUM_PLAYERS] = {0};
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        for (int j = 0; j < player_card_count[i]; j++)
        {
            result[i] += card_value(players[i][j].value);
            color_scores[i] += color_value(players[i][j].color);
        }
    }

    int max_score = result[0];
    int winner = 0;
    int tie = 0;
    for (int i = 1; i < NUM_PLAYERS; i++)
    {
        if (result[i] > max_score)
        {
            max_score = result[i];
            winner = i;
            tie = 0;
        }
        else if (result[i] == max_score)
        {
            tie = 1;
        }
    }

    if (tie)
    {
        int max_color_score = color_scores[winner];
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            if (result[i] == max_score && color_scores[i] > max_color_score)
            {
                winner = i;
                max_color_score = color_scores[i];
            }
        }
    }

    return winner;
}

void sort_cards(struct Card cards[PLAYERS_ARRAY_SIZE], int turns)
{
    for (int i = 0; i < turns; i++)
    {
        for (int j = i + 1; j < turns; j++)
        {
            if (card_value(cards[i].value) < card_value(cards[j].value) ||
                (card_value(cards[i].value) == card_value(cards[j].value) &&
                 color_value(cards[i].color) < color_value(cards[j].color)))
            {
                struct Card tmp = cards[i];
                cards[i] = cards[j];
                cards[j] = tmp;
            }
        }
    }
}

void print_winners_array(int winner_index, struct Card cards[PLAYERS_ARRAY_SIZE], int turns)
{
    printf("Player %d wins: ", winner_index + 1);
    sort_cards(cards, turns);
    for (int i = 0; i < turns; i++)
    {
        printf("(%c, %c), ", cards[i].value, cards[i].color);
    }
    printf("\n");
}

void append(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

char *get_file_content(char *file_name)
{
    int BUZZ_SIZE = 1024;
    char buff[BUZZ_SIZE];
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        printf("Error opening file %s", file_name);
        exit(-1);
    }

    char *result = fgets(buff, BUZZ_SIZE, f);
    // printf("String read: %s\n", buff);
    fclose(f);

    return result;
}

char *generate_key(char *plaintext, char *key)
{
    int plaintext_len = strlen(plaintext);
    char *gen_key = malloc((plaintext_len) * sizeof(char));
    int key_len = strlen(key);

    for (int i = 0, j = 0; i < plaintext_len; i++, j++)
    {
        if (key_len == i || key[j] == '\0')
        {
            j = 0;
        }

        gen_key[i] = key[j];
    }
    gen_key[plaintext_len] = '\0';
    return gen_key;
}

char *encrypt_text(char *plain_text, char *key)
{
    int len = strlen(plain_text);
    char *cipher_text = (char *)malloc((len) * sizeof(char));
    if (cipher_text == NULL)
    {
        printf("Error allocating memory for array");
    }
    for (int i = 0; i < len; i++)
    {
        if (isalpha(plain_text[i]) != 0)
        {
            //vigenere cipher
            char x = (plain_text[i] + key[i]) % 26;
            x += 'A';
            cipher_text[i] = x;
        }
        else
        {
            cipher_text[i] = plain_text[i];
        }
    }
    cipher_text[len] = '\0';
    return cipher_text;
}

char *decrypt_text(char *cipher_text, char *key)
{
    int len = strlen(cipher_text);
    char *plain_text = (char *)malloc((len) * sizeof(char));

    for (int i = 0; i < strlen(cipher_text); i++)
    {
        if (isalpha(cipher_text[i]) != 0)
        {
            char x = (cipher_text[i] - key[i] + 26) % 26;
            x += 'A';
            plain_text[i] = x;
        }
        else
        {
            plain_text[i] = cipher_text[i];
        }
    }
    plain_text[len] = '\0';
    return plain_text;
}

void write_to_file(char *file_name, char *data)
{
    FILE *f = fopen(file_name, "w");
    if (f == NULL)
    {
        printf("Error opening file %s", file_name);
        exit(-1);
    }
    fprintf(f, data);

    fclose(f);
}

int main()
{
    char* key;
    printf("Enter key (uppercase only!): ");
    scanf("%s", key);

    char *content = get_file_content("teste.txt");

    char *gen_key = generate_key(content, key);
    // printf("GEN KEY %s\n", gen_key);

    char *encrypted = encrypt_text(content, gen_key);
    // printf("ENCRIPTED %s\n", encrypted);

    write_to_file("teste_encrypted.txt", encrypted);

    char *decrypted = decrypt_text(encrypted, gen_key);
    // printf("DECRIPTED %s\n", decrypted);

    write_to_file("game_data.txt", decrypted);

    struct Card players[NUM_PLAYERS][PLAYERS_ARRAY_SIZE] = {0};
    int player_card_count[NUM_PLAYERS] = {0};
    int winner_index = game("game_data.txt", players, player_card_count);

    print_winners_array(winner_index, players[winner_index], player_card_count[winner_index]);

    free(gen_key);
    free(encrypted);
    free(decrypted);
    return 0;
}