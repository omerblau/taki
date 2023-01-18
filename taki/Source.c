#define _CRT_SECURE_NO_WARNINGS	//omer blau 209364280
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_CARD_NAME 6
#define MAX_NAME_LEN 21
#define STARTING_CARDS 5
#define BEGINNING_CARDS 4

//special cards ids
#pragma region CARD_IDS
#define COLOR_CARD_ID 0
#define CARD_ID_OF_1 1
#define CARD_ID_OF_2 2
#define CARD_ID_OF_3 3
#define CARD_ID_OF_4 4
#define CARD_ID_OF_5 5
#define CARD_ID_OF_6 6
#define CARD_ID_OF_7 7
#define CARD_ID_OF_8 8
#define CARD_ID_OF_9 9
#define CHANGE_CARD_ID 10
#define PLUS_CARD_ID 11
#define STOP_CARD_ID 12
#define TAKI_CARD_ID 13
#pragma endregion

#pragma region COLOR_IDS
#define COLORLESS_ID 0
#define COLOR_YELLOW_ID 1
#define COLOR_RED_ID 2
#define COLOR_BLUE_ID 3
#define COLOR_GREED_ID 4
#pragma endregion

#pragma region TURN_MOVMENT
#define REGULAR_ORDER 1
#define BACKWARDS_ORDER -1
#define NEXT_PLAYER 1	
#define PREVIOUS_PLAYER -1
#define SKIP_PLAYER 2
#define SKIP_BACKWARDS_PLAYER -2
#pragma endregion

#pragma region GAME_VALUES
#define WINNING_CARD_AMOUNT 0
#define LAST_CARD 1
#define END_TAKI 0
#define DRAW_CARD 0
#define FIRST_TURN 1
#define ANOTHER_TURN 0
#define BEGINNING_VALUE 1000
#pragma endregion




typedef struct Card {
	int cardId;
	int color;
}CARD;

typedef struct Player {
	char name[MAX_NAME_LEN];
	int turnOrder;
	int numOfCards;
	int maxCards;
	CARD* cards;
} PLAYER;

CARD generateCardData();
PLAYER* getPlayersData(int numOfPlayers);
void printCard(CARD card);
void printPlayersCards(PLAYER* player);
void drawCard(PLAYER* player);
void swapUpperCard(PLAYER* player, CARD* upperCard, int cardIndex);;
void colorSwapUpperCard(PLAYER* player, CARD* upperCard, int cardIndex);
void playerMove(PLAYER* player, CARD* upperCard, int* nextPlayersTurn, int* regularOrder);
void game(PLAYER* players, int numOfPlayers, CARD* upperCard);
void validateCardChoice(PLAYER* player, int* choice, CARD* upperCard);
void takiValidateCardChoice(PLAYER* player, CARD* upperCard, int* choice);
void takiMove(PLAYER* player, CARD* upperCard, int* choice, int* nextPlayersTurn, int* regularTurnOrder);

int main()
{
	CARD upperCard;
	int numOfPlayers;
	srand(2);

	//pre turn 1.
	printf("************  Welcome to TAKI game !!! ***********\n");
	printf("Please enter the number of players: \n");
	scanf("%d", &numOfPlayers);

	PLAYER* players = getPlayersData(numOfPlayers);
	upperCard = generateCardData();//generates the upper card
	while (upperCard.cardId < 1 || upperCard.cardId >9) {// if uppercard is specail, regenerate it.
		upperCard = generateCardData();
	}

	game(players, numOfPlayers, &upperCard);

}







//randomly generates a card
CARD generateCardData()
{
	CARD card;
	card.cardId = rand() % 14;
	if (card.cardId == COLOR_CARD_ID)
		card.color = COLORLESS_ID;
	else
		card.color = (rand() % 3) + 1;
	return card;
}

//gets the players data
PLAYER* getPlayersData(int numOfPlayers)
{
	int i, j;
	PLAYER* players = NULL;
	players = (PLAYER*)malloc(numOfPlayers * sizeof(PLAYER)); //creates an arry with the specifeid number of players.
	if (players == NULL) {
		printf("ERROR! Memory allocation failed for plaers\n");
		exit(1);
	}

	for (i = 0; i < numOfPlayers; i++) //gives players names, turn id and initial 4 cards.
	{
		printf("Please enter the first name of player #%d:\n", i + 1);
		scanf("%s", players[i].name);
		players[i].turnOrder = i;
		players[i].numOfCards = BEGINNING_CARDS;
		players[i].maxCards = BEGINNING_CARDS;
		players[i].cards = NULL;
		players[i].cards = (CARD*)malloc(BEGINNING_CARDS * sizeof(CARD));

		if (players == NULL) {
			printf("ERROR! Memory allocation failed for cards\n");
			exit(2);
		}
		for (j = 0; j < BEGINNING_CARDS; j++) //generates random cards
		{
			players[i].cards[j] = generateCardData();
		}
	}
	return players;
}

//draw a card functiom
void drawCard(PLAYER* player)
{
	player->numOfCards++;
	if (player->numOfCards > player->maxCards) {
		player->maxCards++;
		player->cards = realloc(player->cards, player->maxCards * sizeof(CARD));
	}
	if (player->cards == NULL) {
		printf("ERROR! Memory allocation failed for increasing cards size\n");
		exit(3);
	}
	player->cards[player->numOfCards - 1] = generateCardData();
}

//prints a specific card
void printCard(CARD card)
{
	char cardname[6];
	char cardcolor;

	switch (card.cardId)
	{
	case COLOR_CARD_ID:
		strcpy(cardname, "COLOR");
		break;
	case 1:
		strcpy(cardname, "1");
		break;
	case 2:
		strcpy(cardname, "2");
		break;
	case 3:
		strcpy(cardname, "3");
		break;
	case 4:
		strcpy(cardname, "4");
		break;
	case 5:
		strcpy(cardname, "5");
		break;
	case 6:
		strcpy(cardname, "6");
		break;
	case 7:
		strcpy(cardname, "7");
		break;
	case 8:
		strcpy(cardname, "8");
		break;
	case 9:
		strcpy(cardname, "9");
		break;
	case CHANGE_CARD_ID:
		strcpy(cardname, "<->");
		break;
	case PLUS_CARD_ID:
		strcpy(cardname, "+");
		break;
	case STOP_CARD_ID:
		strcpy(cardname, "STOP");
		break;
	case TAKI_CARD_ID:
		strcpy(cardname, "TAKI");
		break;
	default:
		printf("ERROR IN CARD PRINT!!");
		exit(4);
		break;
	}
	switch (card.color)
	{
	case COLORLESS_ID:
		cardcolor = ' ';
		break;
	case COLOR_RED_ID:
		cardcolor = 'R';
		break;
	case COLOR_BLUE_ID:
		cardcolor = 'B';
		break;
	case COLOR_GREED_ID:
		cardcolor = 'G';
		break;
	case COLOR_YELLOW_ID:
		cardcolor = 'Y';
		break;
	default:
		printf("ERROR IN CARD PRINT!!");
		exit(4);
		break;
	}

	if ((card.cardId <= 9 && card.cardId >= 1) || card.cardId == PLUS_CARD_ID)
		printf("*********\n*       *\n*   %s   *\n*   %c   *\n*       *\n*********\n", cardname, cardcolor);
	else if (card.cardId == CHANGE_CARD_ID)
		printf("*********\n*       *\n*  %s  *\n*   %c   *\n*       *\n*********\n", cardname, cardcolor);
	else if (card.cardId == STOP_CARD_ID || card.cardId == TAKI_CARD_ID)
		printf("*********\n*       *\n*  %s *\n*   %c   *\n*       *\n*********\n", cardname, cardcolor);
	else
		printf("*********\n*       *\n* %s *\n*   %c   *\n*       *\n*********\n", cardname, cardcolor);
	printf("\n");
}

//prints all of a given players cards
void printPlayersCards(PLAYER* player)
{
	for (int i = 0; i < player->numOfCards; i++)
	{
		printf("Card #%d\n", i + 1);
		printCard(player->cards[i]);
	}
}


void game(PLAYER* players, int numOfPlayers, CARD* upperCard)
{
	int i = BEGINNING_VALUE * numOfPlayers;
	int regularTurnOrder = REGULAR_ORDER;
	int nextPlayersTurn;
	while (players[i % numOfPlayers].numOfCards != WINNING_CARD_AMOUNT)
	{
		nextPlayersTurn = 0;
		printf("\nUpper card:\n");
		printCard(*upperCard);
		printf("%s's turn:\n\n", players[i % numOfPlayers].name);
		printPlayersCards(&players[i % numOfPlayers]);
		playerMove(&players[i % numOfPlayers], upperCard, &nextPlayersTurn, &regularTurnOrder);
		i = i + nextPlayersTurn;
	}
	exit(0);
}


void validateCardChoice(PLAYER* player, int* choice, CARD* upperCard) {

	printf("Please enter 0 if you want to take a card from the deck\n");
	printf("or 1-%d if you want to put one of your cards in the middle:\n", player->numOfCards);
	scanf("%d", choice);
	while ((upperCard->cardId != player->cards[(*choice) - 1].cardId && upperCard->color != player->cards[(*choice) - 1].color && (*choice) != DRAW_CARD && player->cards[(*choice) - 1].cardId != COLOR_CARD_ID) || (*choice > player->numOfCards || *choice < DRAW_CARD))
	{
		if (*choice > player->numOfCards || *choice < DRAW_CARD) {
			printf("Invalid choice! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", player->numOfCards);
			scanf("%d", choice);
		}
		else {
			printf("Invalid card! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", player->numOfCards);
			scanf("%d", choice);
		}
	}
}


//the players options in his move
void playerMove(PLAYER* player, CARD* upperCard, int* nextPlayersTurn, int* regularTurnOrder)
{
	int choice;
	validateCardChoice(player, &choice, upperCard);
	if (choice == DRAW_CARD) { // if chose to draw card, draws a card and end the turn
		drawCard(player);
		if (*regularTurnOrder == REGULAR_ORDER)
			*nextPlayersTurn = NEXT_PLAYER;
		else
			*nextPlayersTurn = PREVIOUS_PLAYER;
	}
	else if (player->cards[choice - 1].cardId == COLOR_CARD_ID) { // if chose to use color card, ask for color and end the turn
		colorSwapUpperCard(player, upperCard, choice - 1);
		if (*regularTurnOrder == REGULAR_ORDER)
			*nextPlayersTurn = NEXT_PLAYER;
		else
			*nextPlayersTurn = PREVIOUS_PLAYER;
	}
	else {
		if (player->cards[choice - 1].cardId <= CARD_ID_OF_9) {//dealing with normal cards requires a normal swap
			swapUpperCard(player, upperCard, choice - 1);
			if (*regularTurnOrder == REGULAR_ORDER)
				*nextPlayersTurn = NEXT_PLAYER;
			else
				*nextPlayersTurn = PREVIOUS_PLAYER;
		}
		else if (player->cards[choice - 1].cardId == TAKI_CARD_ID) {
			swapUpperCard(player, upperCard, choice - 1);
			takiMove(player, upperCard, &choice, nextPlayersTurn, regularTurnOrder);
		}
		else if (player->cards[choice - 1].cardId == CHANGE_CARD_ID) {
			swapUpperCard(player, upperCard, choice - 1);
			(*regularTurnOrder) *= BACKWARDS_ORDER;
			if (*regularTurnOrder == REGULAR_ORDER)
				*nextPlayersTurn = NEXT_PLAYER;
			else
				*nextPlayersTurn = PREVIOUS_PLAYER;
		}
		else if (player->cards[choice - 1].cardId == STOP_CARD_ID) {
			swapUpperCard(player, upperCard, choice - 1);
			if (*regularTurnOrder == REGULAR_ORDER)
				*nextPlayersTurn = SKIP_PLAYER;
			else
				*nextPlayersTurn = SKIP_BACKWARDS_PLAYER;
		}
		else { // only alternative is plus card
			if (player->numOfCards == LAST_CARD) {
				swapUpperCard(player, upperCard, choice - 1);
				drawCard(player);
				if (*regularTurnOrder == REGULAR_ORDER)
					*nextPlayersTurn = NEXT_PLAYER;
				else
					*nextPlayersTurn = PREVIOUS_PLAYER;
			}
			else {	//give another turn, if dosent have valid card, draw one,
				swapUpperCard(player, upperCard, choice - 1);
			}
		}
	}
}


void takiMove(PLAYER* player, CARD* upperCard, int* choice, int* nextPlayersTurn, int* regularTurnOrder)
{
	int cadsWhenTakiPlaced = player->numOfCards;
	printf("\nUpper card:\n");
	printCard(*upperCard);
	printf("%s's turn:\n\n", player->name);
	printPlayersCards(player);
	takiValidateCardChoice(player, upperCard, choice);
	while (*choice != END_TAKI)
	{
		swapUpperCard(player, upperCard, (*choice) - 1);
		printf("\nUpper card:\n");
		printCard(*upperCard);
		printf("%s's turn:\n\n", player->name);
		printPlayersCards(player);
		takiValidateCardChoice(player, upperCard, choice);
	}
	if (cadsWhenTakiPlaced > player->numOfCards) // meaining a playerd play a card after the taki was placed.
	{	
		if (upperCard->cardId == COLOR_CARD_ID) {
			int colorCoice;
			printf("Please enter your color choice:\n");
			printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
			scanf("%d", &colorCoice);
			while (colorCoice > 4 || colorCoice < 1) // not on of the colors
			{
				printf("Invalid color! Try again.");
				printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
				scanf("%d", &colorCoice);
			}
			upperCard->color = colorCoice;
			if (*regularTurnOrder == REGULAR_ORDER)
				*nextPlayersTurn = NEXT_PLAYER;
			else
				*nextPlayersTurn = PREVIOUS_PLAYER;
		}
		else if (upperCard->cardId == CHANGE_CARD_ID) {
			(*regularTurnOrder) *= BACKWARDS_ORDER;
			if (*regularTurnOrder == REGULAR_ORDER)
				*nextPlayersTurn = NEXT_PLAYER;
			else
				*nextPlayersTurn = PREVIOUS_PLAYER;
		}
		else if (upperCard == STOP_CARD_ID) {
			if (*regularTurnOrder == REGULAR_ORDER)
				*nextPlayersTurn = SKIP_PLAYER;
			else
				*nextPlayersTurn = SKIP_BACKWARDS_PLAYER;
		} 
		else if (upperCard->cardId == PLUS_CARD_ID) {
			if (player->numOfCards == WINNING_CARD_AMOUNT) {
				drawCard(player);
				if (*regularTurnOrder == REGULAR_ORDER)
					*nextPlayersTurn = NEXT_PLAYER;
				else
					*nextPlayersTurn = PREVIOUS_PLAYER;
			}
			else {
				// do nothing, just gets anotherturn
			}
		}
		else if (upperCard->cardId == TAKI_CARD_ID) {
			takiMove(player, upperCard, choice, nextPlayersTurn, regularTurnOrder);
		}
		else {//regular cards
			if (*regularTurnOrder == REGULAR_ORDER)
				*nextPlayersTurn = NEXT_PLAYER;
			else
				*nextPlayersTurn = PREVIOUS_PLAYER;
		}
	}
}


void takiValidateCardChoice(PLAYER* player, CARD* upperCard, int* choice)
{
	printf("Please enter 0 if you want to finish your turn\n");
	printf("or 1-%d if you want to put one of your cards in the middle:\n", player->numOfCards);
	scanf("%d", choice);
	while ((*choice != END_TAKI && player->cards[(*choice) - 1].cardId != COLOR_CARD_ID && upperCard->color != player->cards[*choice - 1].color) || (*choice > player->numOfCards || *choice < DRAW_CARD))
	{
		if (*choice > player->numOfCards || *choice < DRAW_CARD) {
			printf("Invalid choice! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", player->numOfCards);
			scanf("%d", choice);
		}
		else {
			printf("Invalid card! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", player->numOfCards);
			scanf("%d", choice);
		}
	}
}

//places a card from the players hand to the middle.
void swapUpperCard(PLAYER* player, CARD* upperCard, int cardIndex)
{
	upperCard->cardId = player->cards[cardIndex].cardId;
	upperCard->color = player->cards[cardIndex].color;
	if (cardIndex != player->numOfCards - 1) {
		player->cards[cardIndex].cardId = player->cards[player->numOfCards - 1].cardId;
		player->cards[cardIndex].color = player->cards[player->numOfCards - 1].color;
		player->cards[player->numOfCards - 1].cardId = 15;
		player->cards[player->numOfCards - 1].color = 15;
	}
	player->numOfCards--;
}


void colorSwapUpperCard(PLAYER* player, CARD* upperCard, int cardIndex)
{
	int colorCoice;
	printf("Please enter your color choice:\n");
	printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
	scanf("%d", &colorCoice);
	while (colorCoice > 4 || colorCoice < 1) // not on of the colors
	{
		printf("Invalid color! Try again.");
		printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
		scanf("%d", &colorCoice);
	}
	swapUpperCard(player, upperCard, cardIndex);
	upperCard->color = colorCoice;
}

