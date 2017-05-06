#include "peca.h"
#include "Texto.h"
#include "Defines.h"
#include <stdlib.h>
#include <MT2D/MT2D.h>
#include <MT2D/MT2D_Display.h>
#include <MT2D/MT2D_Layout.h>
#include <MT2D/MT2D_System_Calls.h>

char strBUFFER[200];

static const char Peca_Desenho[2][6][7] = { { "x    x"," x  x ", "  xx  ", " x  x ", "x    x" },
											{ "  oo  "," o  o ", "o    o", " o  o ", "  oo  " } 
										  };
static const char Jogador_Tipo[2][7] = { "HUMANO","CPU   " };
static const char NomeJogo[] = "TIC TAC TOE 10000!!!";
static const char Jogadore[] = "JOGADOR";
static const char Vitorias[] = "Vitorias";
static const char VezDoJogador[] = "VEZ DO JOGADOR";

//static const char Peca1_Desenho[6][7] = { "x    x"," x  x ", "  xx  ", " x  x ", "x    x" };
//static const char Peca2_Desenho[6][7] = { "  oo  "," o  o ", "o    o", " o  o ", "  oo  " };

void Render_Titulo(int *Vitorias1, int *Vitorias2, bool *CPU1, bool *CPU2, int *Vez) {
	MT2D_Create_Window_Layout(0, 0, MAX_HOR-1, 4, true, false, false, ' ', DISPLAY_WINDOW1);
	insert_string_on_display((char*)NomeJogo, 0, 10, DISPLAY_WINDOW1);

	insert_string_on_display((char*)Jogadore, 1, 3, DISPLAY_WINDOW1);
	insert_string_on_display((char*)Jogador_Tipo[CPU1[0]], 2, 1, DISPLAY_WINDOW1);
	insert_string_on_display((char*)Jogador_Tipo[CPU2[0]], 3, 1, DISPLAY_WINDOW1);

	insert_string_on_display((char*)Vitorias, 1, 13, DISPLAY_WINDOW1);
	insert_number_on_display(Vitorias1[0], 2, 13, DISPLAY_WINDOW1);
	insert_number_on_display(Vitorias2[0], 3, 13, DISPLAY_WINDOW1);

	insert_string_on_display((char*)VezDoJogador, 3, 40, DISPLAY_WINDOW1);
	insert_number_on_display(Vez[0]+1, 3, 55, DISPLAY_WINDOW1);

}


void Render_Log(Texto *texto, int *Linha_Inicial) {
	MT2D_Create_Window_Layout(POS_HOR_ABA_LOG_TEXTO, POS_VER_ABA_LOG_TEXTO, TAM_HOR_ABA_LOG_TEXTO, TAM_VER_ABA_LOG_TEXTO, true, false, false, false, DISPLAY_WINDOW1);
	int linhainicial = 0;
	if (texto->QntLinhas - TAM_VER_ABA_LOG_TEXTO + 1 > 0) {
		linhainicial = texto->QntLinhas - TAM_VER_ABA_LOG_TEXTO + 1;
	}
	if (texto->QntLinhas) {
		for (int i = 0; i < texto->QntLinhas; i++, linhainicial++) {
			if (linhainicial == texto->QntLinhas) {
				i = texto->QntLinhas;
			}
			else {
				sprintf(strBUFFER, "%d) %s", linhainicial, texto->Linhas[linhainicial].Caracteres);
				insert_string_on_display(strBUFFER, i + POS_VER_ABA_LOG_TEXTO  + 1, POS_HOR_ABA_LOG_TEXTO + 1, DISPLAY_WINDOW1);
			}
		}
	}
}

void Render_Selecionado(int &PosX, int &PosY) {
	unsigned int A, B;
	for (unsigned char Y = POS_VER_TABULEIRO + PosY * TAM_VER_PECA + PosY; Y < POS_VER_TABULEIRO + (1 + PosY) * TAM_VER_PECA + PosY; Y++) {
		WINDOW1[Y][POS_HOR_TABULEIRO + PosX - 1 + (PosX + 0)* TAM_HOR_PECA] = '>';
		WINDOW1[Y][POS_HOR_TABULEIRO + PosX + 0 + (PosX + 1) * TAM_HOR_PECA] = '<';
	}
	for (unsigned char X = POS_HOR_TABULEIRO + PosX * TAM_HOR_PECA + PosX; X < POS_HOR_TABULEIRO + (1 +  PosX) * TAM_HOR_PECA + PosX; X++) {
		WINDOW1[POS_VER_TABULEIRO + PosY - 1 + (PosY + 0) * TAM_VER_PECA][X] = 'v';
		WINDOW1[POS_VER_TABULEIRO + PosY + 0 + (PosY + 1) * TAM_VER_PECA][X] = '^';
	}
}

void Render_Peca(unsigned char PosX, unsigned char PosY, Peca tipo) {
	unsigned int i =0 , j;
	char tmp;
	for (; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			WINDOW1[POS_VER_TABULEIRO + i + PosY + PosY * TAM_VER_PECA][POS_HOR_TABULEIRO + j + PosX + PosX * TAM_HOR_PECA] = Peca_Desenho[tipo][i][j];
		}
	}
}

void Render_Pecas(Peca **Tabuleiro) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (Tabuleiro[i][j] != SemPeca) {
				Render_Peca(j, i, Tabuleiro[i][j]);
			}
		}

	}

}

void Render_Tabuleiro() {//não desenha as peças, só o tabuleiro
	for (unsigned char Y = POS_VER_TABULEIRO; Y < POS_VER_TABULEIRO + TAM_VER_TABULEIRO; Y++) {
		WINDOW1[Y][POS_HOR_TABULEIRO + 0 + 1 * TAM_HOR_PECA] = CHAR_RETA_VERTICAL;
		WINDOW1[Y][POS_HOR_TABULEIRO + 1 + 2 * TAM_HOR_PECA] = CHAR_RETA_VERTICAL;
	}
	for (unsigned char X = POS_HOR_TABULEIRO; X < POS_HOR_TABULEIRO + TAM_HOR_TABULEIRO; X++) {
		WINDOW1[POS_VER_TABULEIRO + 0 + 1 * TAM_VER_PECA][X] = CHAR_RETA_HORIZONTAL;
		WINDOW1[POS_VER_TABULEIRO + 1 + 2 * TAM_VER_PECA][X] = CHAR_RETA_HORIZONTAL;
	}
	WINDOW1[POS_VER_TABULEIRO + 0 + 1 * TAM_VER_PECA][POS_HOR_TABULEIRO + 0 + 1 * TAM_HOR_PECA] = CHAR_LIGA_TODOS;
	WINDOW1[POS_VER_TABULEIRO + 0 + 1 * TAM_VER_PECA][POS_HOR_TABULEIRO + 1 + 2 * TAM_HOR_PECA] = CHAR_LIGA_TODOS;
	WINDOW1[POS_VER_TABULEIRO + 1 + 2 * TAM_VER_PECA][POS_HOR_TABULEIRO + 0 + 1 * TAM_HOR_PECA] = CHAR_LIGA_TODOS;
	WINDOW1[POS_VER_TABULEIRO + 1 + 2 * TAM_VER_PECA][POS_HOR_TABULEIRO + 1 + 2 * TAM_HOR_PECA] = CHAR_LIGA_TODOS;

}

void Render_Posicao_Invalida_Efeito() {
	int i = 0;
	for (int j = 0; j < 10; j++) {
		transfer_window1_to_window2();
		for (i = 0; i < 600; i++) {
			WINDOW2[rand() % (MAX_VER-1)][rand() % MAX_HOR] = CHAR_SOMBRA_25 + rand()%3;
		}
		MT2D_System_Beep(rand() % 1000, 100);
		MT2D_Draw_Window(DISPLAY_WINDOW2);
	}
	MT2D_Draw_Window(DISPLAY_WINDOW1);

}
