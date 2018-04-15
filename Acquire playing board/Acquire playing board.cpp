// Acquire playing board.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BOARD_SIZE 108

struct hotel_chain{
	int nb_hotel; // nb d'hotel de la chaine sur le plateau 
	int chain_name; // mettre un nom sur les chiffres 
	// 1-2 airport-festival
	// 3-4-5 Imperial-Luxor-Oriental
	// 6-7 Prestige-Continental
	int tile[BOARD_SIZE];
	bool status; // vrai si libre faux sinon
	bool on_board; // chaine sur le plateau ou pas 
};

typedef struct hotel_chain S_hotel;

bool card_left(int board[], int card);
bool card_right(int board[], int card);
int hotel_chain(int board[], int chain);
bool card_up(int board[], int card);
bool card_down(int board[], int card);
void hotel_tile(int board[], S_hotel hotel[], int chain);
void hotel_status(S_hotel hotel[], int chain);

int hotel_choice(S_hotel hotel[], int choice_1, int choice_2);
void print_chain_name(S_hotel hotel[], int chain_num);

int _tmain(int argc, _TCHAR* argv[])
{
	int playing_board[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++)
		playing_board[i] = -1;
	playing_board[1] = 1;
	S_hotel hotel[7];
	hotel[0].nb_hotel = 1;
	hotel[0].tile[0] = 0;
	hotel_tile(playing_board, hotel, 0);
	printf("\n%d", hotel[0].tile[0]);
	printf("\n%d", hotel[0].tile[1]);
	printf("\n%d", hotel[0].tile[2]);
	if (card_right(playing_board, hotel[0].tile[0]))
		printf("\ntrue");
	else printf("\nfalse");
	
	
	return 0;
}

bool card_left(int board[], int card)
{
	if (board[card - 1] != -1 && card > 0)
		return true;
	else
		return false;
}

bool card_right(int board[], int card)
{
	if (board[card + 1] != -1 && card < 12)
		return true;
	else
		return false;
}

bool card_up(int board[], int card)
{
	if (board[card + 12] != -1 && card <= 96)
		return true;
	else
		return false;
}

bool card_down(int board[], int card) // "bas" du plateau = case 0,0
{
	if (board[card - 12] != -1 && card >= 12)
		return true;
	else
		return false;
}

int hotel_chain(int board[], int chain, S_hotel hotel[]) //compte le nb d'hotel dans la chaine
{
	for (int i = 0; i < 108; i++)
	{
		if (card_down(board, i) || card_up(board, i) || card_left(board, i) || card_right(board, i))
		{
			hotel[chain].nb_hotel++;
			return hotel[chain].nb_hotel;
		}
	}
}

void hotel_tile(int board[], S_hotel hotel[], int chain) //ajoute la coordonné de la carte au tableau des coordonnés des cartes de la chaine, ajoute +1 a la taille si une carte apartient a la chaine
{
	for (int i = 0; i < hotel[chain].nb_hotel; i++)
	{
		if (card_up(board, hotel[chain].tile[i]))
		{
			hotel[chain].tile[hotel[chain].nb_hotel] = board[hotel[chain].tile[i] + 12];
			hotel[chain].nb_hotel++;
		}
			

		if (card_down(board, hotel[chain].tile[i]))
		{
			hotel[chain].tile[hotel[chain].nb_hotel] = board[hotel[chain].tile[i] - 12];
			hotel[chain].nb_hotel++;
		}

		if (card_right(board, hotel[chain].tile[i]))
		{
			hotel[chain].tile[hotel[chain].nb_hotel] = board[hotel[chain].tile[i] + 1];
			hotel[chain].nb_hotel++;
		}

		if (card_left(board, hotel[chain].tile[i]))
		{
			hotel[chain].tile[hotel[chain].nb_hotel] = board[hotel[chain].tile[i] - 1];
			hotel[chain].nb_hotel++;
		}
	}

}

void hotel_status(S_hotel hotel[], int chain) // retourne true si l'hotel est libre
{
	if (hotel[chain].nb_hotel < 11)
		hotel[chain].status = true;
	else hotel[chain].status = false;
}

bool placing_card(int board[], S_hotel hotel[], int card)  // retourne true si la carte peut etre placé et la place sinon retourne false
{
	bool right = false, left = false, up = false, down = false;
	int hotel_right, hotel_left, hotel_up, hotel_down;
	
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < hotel[i].nb_hotel; j++)
		{ 
			if (hotel[i].tile[j] == card - 1 && card>0)
			{
				hotel_left = i;
				left = true;
			}
			if (hotel[i].tile[j] == card + 1 && card<108)
			{
				hotel_right = i;
				right = true;
			}
			if (hotel[i].tile[j] == card - 12 && card>=12)
			{
				hotel_down = i;
				down = true;
			}
			if (hotel[i].tile[j] == card + 12 && card<=96)
			{
				hotel_up = i;
				up = true;
			}
		}	
	}
	if (left && right && hotel[hotel_left].status == false && hotel[hotel_right].status == false)
		return false;
	if (left && up && hotel[hotel_left].status == false && hotel[hotel_up].status == false)
		return false;
	if (left && down && hotel[hotel_left].status == false && hotel[hotel_down].status == false)
		return false;
	if (right && up && hotel[hotel_right].status == false && hotel[hotel_up].status == false)
		return false;
	if (right && down && hotel[hotel_right].status == false && hotel[hotel_down].status == false)
		return false;
	if (down && up && hotel[hotel_down].status == false && hotel[hotel_up].status == false)
		return false;
	else
	{
		if (left && right)
		{
			if (hotel[hotel_left].status && hotel[hotel_right].status == false)
				hotel[hotel_left].tile[hotel[hotel_left].nb_hotel] = card;
			if (hotel[hotel_left].status==false && hotel[hotel_right].status)
				hotel[hotel_right].tile[hotel[hotel_right].nb_hotel] = card;
			if (hotel[hotel_left].status == false && hotel[hotel_right].status == false)
			{
					int choice = hotel_choice(hotel, hotel[hotel_left].chain_name, hotel[hotel_right].chain_name);
					print_chain_name(hotel, choice);
					hotel[choice].tile[hotel[choice].nb_hotel] = card;
			}
			return true;
		}
		if (left && up)
		{
			if (hotel[hotel_left].status && hotel[hotel_up].status == false)
				hotel[hotel_left].tile[hotel[hotel_left].nb_hotel] = card;
			if (hotel[hotel_left].status == false && hotel[hotel_up].status)
				hotel[hotel_up].tile[hotel[hotel_up].nb_hotel] = card;
			if (hotel[hotel_left].status == false && hotel[hotel_up].status == false)
			{
				int choice = hotel_choice(hotel, hotel[hotel_left].chain_name, hotel[hotel_up].chain_name);
				print_chain_name(hotel, choice);
				hotel[choice].tile[hotel[choice].nb_hotel] = card;
			}
			return true;
		}
		if (left && down)
		{
			if (hotel[hotel_left].status && hotel[hotel_down].status == false)
				hotel[hotel_left].tile[hotel[hotel_left].nb_hotel] = card;
			if (hotel[hotel_left].status == false && hotel[hotel_down].status)
				hotel[hotel_down].tile[hotel[hotel_down].nb_hotel] = card;
			if (hotel[hotel_left].status == false && hotel[hotel_down].status == false)
			{
				int choice = hotel_choice(hotel, hotel[hotel_left].chain_name, hotel[hotel_down].chain_name);
				print_chain_name(hotel, choice);
				hotel[choice].tile[hotel[choice].nb_hotel] = card;
			}
			
			return true;
		}
		if (right && up)
		{
			if (hotel[hotel_right].status && hotel[hotel_up].status == false)
				hotel[hotel_right].tile[hotel[hotel_right].nb_hotel] = card;
			if (hotel[hotel_right].status == false && hotel[hotel_up].status)
				hotel[hotel_up].tile[hotel[hotel_up].nb_hotel] = card;
			if (hotel[hotel_right].status == false && hotel[hotel_up].status == false)
			{
				int choice = hotel_choice(hotel, hotel[hotel_right].chain_name, hotel[hotel_up].chain_name);
				print_chain_name(hotel, choice);
				hotel[choice].tile[hotel[choice].nb_hotel] = card;
			}
			return true;
		}
		if (right && down)
		{
			if (hotel[hotel_right].status && hotel[hotel_down].status == false)
				hotel[hotel_right].tile[hotel[hotel_right].nb_hotel] = card;
			if (hotel[hotel_right].status == false && hotel[hotel_down].status)
				hotel[hotel_down].tile[hotel[hotel_down].nb_hotel] = card;
			if (hotel[hotel_right].status == false && hotel[hotel_down].status == false)
			{
				int choice = hotel_choice(hotel, hotel[hotel_right].chain_name, hotel[hotel_down].chain_name);
				print_chain_name(hotel, choice);
				hotel[choice].tile[hotel[choice].nb_hotel] = card;
			}
			return true;
		}
		if (up && down)
		{
			if (hotel[hotel_up].status && hotel[hotel_down].status == false)
				hotel[hotel_up].tile[hotel[hotel_up].nb_hotel] = card;
			if (hotel[hotel_up].status == false && hotel[hotel_down].status)
				hotel[hotel_down].tile[hotel[hotel_down].nb_hotel] = card;
			if (hotel[hotel_up].status == false && hotel[hotel_down].status == false)
			{
				int choice = hotel_choice(hotel, hotel[hotel_up].chain_name, hotel[hotel_down].chain_name);
				print_chain_name(hotel, choice);
				hotel[choice].tile[hotel[choice].nb_hotel] = card;
			}
			return true;
		}
	}
}

void print_chain_name(S_hotel hotel[], int chain_num)
{
	switch (hotel[chain_num].chain_name)
	{ 
		case 1: printf("Airport \n");
			break;
		case 2: printf("Festival \n");
			break;
		case 3: printf("Imperial \n");
			break;
		case 4: printf("Luxor \n");
			break;

		case 5: printf("Oriental \n");
			break;

		case 6: printf("Prestige \n");
			break;

		case 7: printf("Contiental \n");
			break; 
		default: printf(" \nnumero invalide \n");
	}
}

int hotel_choice(S_hotel hotel[], int choice_1, int choice_2)
{
	int user_choice;
	printf("choix de l'hotel a aggrandir \n");
	printf("1 - 2 airport - festival \n");
	printf("3 - 4 - 5 Imperial - Luxor - Oriental \n");
	printf(" 6 - 7 Prestige - Continental \n");
	printf("choisissez l'hotel: \n");
	print_chain_name(hotel, choice_1);
	print_chain_name(hotel, choice_2);
	scanf_s("%d", &user_choice);
	return user_choice;
}