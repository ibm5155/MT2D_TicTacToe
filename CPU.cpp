

//Tic-tac-toe playing AI. Exhaustive tree-search. WTFPL
//Matthew Steel 2009, www.www.repsilat.com

#include <stdio.h>
#include "peca.h"

int win(const int board[9]) {
	//determines if a player has won, returns 0 otherwise.
	unsigned wins[8][3] = { { 0,1,2 },{ 3,4,5 },{ 6,7,8 },{ 0,3,6 },{ 1,4,7 },{ 2,5,8 },{ 0,4,8 },{ 2,4,6 } };
	int i;
	for (i = 0; i < 8; ++i) {
		if (board[wins[i][0]] != 0 &&
			board[wins[i][0]] == board[wins[i][1]] &&
			board[wins[i][0]] == board[wins[i][2]])
			return board[wins[i][2]];
	}
	return 0;
}

int minimax(int board[9], int player) {
	//How is the position like for player (their turn) on board?
	int winner = win(board);
	if (winner != 0) return winner*player;

	int move = -1;
	int score = -2;//Losing moves are preferred to no move
	int i;
	for (i = 0; i < 9; ++i) {//For all moves,
		if (board[i] == 0) {//If legal,
			board[i] = player;//Try the move
			int thisScore = -minimax(board, player*-1);
			if (thisScore > score) {
				score = thisScore;
				move = i;
			}//Pick the one that's worst for the opponent
			board[i] = 0;//Reset board after try
		}
	}
	if (move == -1) return 0;
	return score;
}

int _CPU(Peca **Tabuleiro, bool WhoAmI) {
	int board[9];
	int move = -1;
	int score = -2;
	int i,j;
	//cria tabuleiro pra cpu
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (Tabuleiro[i][j] == Peca1) {
				board[i * 3 + j] = (WhoAmI == 0 ? 1 : -1);
			}
			else if (Tabuleiro[i][j] == Peca2) {
				board[i * 3 + j] = (WhoAmI == 1 ? 1 : -1);
			}
			else {
				board[i * 3 + j] = 0;
			}
		}
	}
	for (i = 0; i < 9; ++i) {
		if (board[i] == 0) {
			board[i] = 1;
			int tempScore = -minimax(board, -1);
			board[i] = 0;
			if (tempScore > score) {
				score = tempScore;
				move = i;
			}
		}
	}
	return move;
}

/*
int main() {
	int board[9] = { 0,0,0,0,0,0,0,0,0 };
	//computer squares are 1, player squares are -1.
	printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
	int player = 0;
	scanf("%d", &player);
	printf("\n");
	unsigned turn;
	for (turn = 0; turn < 9 && win(board) == 0; ++turn) {
		if ((turn + player) % 2 == 0)
			computerMove(board);
		else {
			draw(board);
			playerMove(board);
		}
	}
	switch (win(board)) {
	case 0:
		printf("A draw. How droll.\n");
		break;
	case 1:
		draw(board);
		printf("You lose.\n");
		break;
	case -1:
		printf("You win. Inconceivable!\n");
		break;
	}
}
*/