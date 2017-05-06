#include <stdlib.h>
#include <MT2D/MT2D.h>
#include <MT2D/MT2D_Display.h>
#include <MT2D/MessageBox/MT2D_MessageBox_With_Result.h>
#include <MT2D/MessageBox/MT2D_MessageBox.h>
#include <MT2D/MT2D_Keyboard.h>
#include "render.h"
#include "Texto.h"
#include "CPU.h"
#include <time.h>

/**
retorno
0 = nada acontece
-1 = empate
1 = jogador 1 vence
2 = jogador 2 vence
**/
int Vencedor(Peca **Tabuleiro) {
	int venceu = 0;//nada aconteceu
	if (Tabuleiro[0][0] == Tabuleiro[0][1] && Tabuleiro[0][0] == Tabuleiro[0][2] && Tabuleiro[0][0] != SemPeca) {
		venceu = Tabuleiro[0][0] + 1;
	}
	else if (Tabuleiro[1][0] == Tabuleiro[1][1] && Tabuleiro[1][0] == Tabuleiro[1][2] && Tabuleiro[1][0] != SemPeca) {
		venceu = Tabuleiro[1][0] + 1;
	}
	else if (Tabuleiro[2][0] == Tabuleiro[2][1] && Tabuleiro[2][0] == Tabuleiro[2][2] && Tabuleiro[2][0] != SemPeca) {
		venceu = Tabuleiro[2][0] + 1;
	}
	else if (Tabuleiro[0][0] == Tabuleiro[1][0] && Tabuleiro[0][0] == Tabuleiro[2][0] && Tabuleiro[0][0] != SemPeca) {
		venceu = Tabuleiro[0][0] + 1;
	}
	else if (Tabuleiro[0][1] == Tabuleiro[1][1] && Tabuleiro[0][1] == Tabuleiro[2][1] && Tabuleiro[0][1] != SemPeca) {
		venceu = Tabuleiro[0][1] + 1;
	}
	else if (Tabuleiro[0][2] == Tabuleiro[1][2] && Tabuleiro[0][2] == Tabuleiro[2][2] && Tabuleiro[0][2] != SemPeca) {
		venceu = Tabuleiro[0][2] + 1;
	}
	else if (Tabuleiro[0][0] == Tabuleiro[1][1] && Tabuleiro[0][0] == Tabuleiro[2][2] && Tabuleiro[0][0] != SemPeca) {
		venceu = Tabuleiro[0][0] + 1;
	}
	else if (Tabuleiro[0][2] == Tabuleiro[1][1] && Tabuleiro[0][2] == Tabuleiro[2][0] && Tabuleiro[0][2] != SemPeca) {
		venceu = Tabuleiro[0][2] + 1;
	}
	else if (Tabuleiro[0][0] != SemPeca && Tabuleiro[0][1] != SemPeca && Tabuleiro[0][2] != SemPeca &&
			 Tabuleiro[0][1] != SemPeca && Tabuleiro[1][1] != SemPeca && Tabuleiro[1][2] != SemPeca &&
			 Tabuleiro[0][2] != SemPeca && Tabuleiro[2][2] != SemPeca && Tabuleiro[2][2] != SemPeca) {
		venceu = -1;
	}
	return venceu;
}

extern char strBUFFER[200];

int main() {
// REGIÃO 1: VARIAVEIS
	time_t Tempo[2];
	bool CPU[2];
	bool FecharJogo = false;
	int Vitorias[2] = { 0,0 };
	int Vez = 0;
	int x, y;
	int PonteiroX, PonteiroY;
	int Posicao_Log = 0;
	unsigned char Tecla;
	Peca **Tabuleiro = (Peca**)malloc(3 * sizeof(Peca));;
	Texto *Log_Jogada = (Texto*)malloc(sizeof(Texto));
	Log_Jogada->Fonte = 0;
	Log_Jogada->Linhas = 0;
	Log_Jogada->Salvo = true;
	Log_Jogada->QntLinhas = 0;

	for (y = 0; y < 3; y++) {
		Tabuleiro[y] = (Peca*)malloc(3 * sizeof(Peca));
	}

//	REGIÃO 2: PREPARAÇÃO
	MT2D_Clear_Main_Window();

	CPU[0] = (1 - MT2D_MessageBox_With_Result("O QUE VAI SER O JOGADOR 1?", "", "HUMANO", "CPU"));
	CPU[1] = (1 - MT2D_MessageBox_With_Result("O QUE VAI SER O JOGADOR 2?", "", "HUMANO", "CPU"));


//	REGIÃO 3: LOOP JOGO
	while (!FecharJogo) {
		//inicia/reinicia tabuleiro
		for (y = 0; y < 3; y++) {
			for (x = 0; x < 3; x++) {
				Tabuleiro[y][x] = SemPeca;
			}

		}
		PonteiroY = 0;
		PonteiroX = 0;
		Vez = rand() % 2; //seleciona aleatoriamente quem vai iniciar o jogo
		sprintf(strBUFFER, "Jogo %d iniciado", Vitorias[0] + Vitorias[1] + 1);
		AdicionaLinha(Log_Jogada, strBUFFER);

		while (Vencedor(Tabuleiro) == 0 && !FecharJogo) {//faça jogo enquanto o jogo não acaba

			MT2D_Clear_Main_Window();

			Render_Titulo(&Vitorias[0], &Vitorias[1], &CPU[0], &CPU[1], &Vez);

			Render_Log(Log_Jogada, &Posicao_Log);

			Render_Tabuleiro();

			Render_Pecas(Tabuleiro);

			Render_Selecionado(PonteiroX, PonteiroY);

			MT2D_Draw_Window(DISPLAY_WINDOW1);


			if (CPU[Vez] == true) {
				//faz algo
				Tempo[0] = clock();
				PonteiroX = _CPU(Tabuleiro, Vez);
				Tempo[1] = clock();
				PonteiroY = PonteiroX / 3;
				PonteiroX = PonteiroX - PonteiroY * 3;
				sprintf(strBUFFER, "CPU %d levou %dms para processar", Vez + 1, Tempo[1] - Tempo[0]);
				AdicionaLinha(Log_Jogada, strBUFFER);
				sprintf(strBUFFER, "Jogador %d botou peca em %d%d", Vez + 1, PonteiroY, PonteiroX);
				AdicionaLinha(Log_Jogada, strBUFFER);
				Tabuleiro[PonteiroY][PonteiroX] = (Peca)Vez;
				Vez = !(bool)Vez;
				PonteiroY = 0;
				PonteiroX = 0;

				sprintf(strBUFFER, "Aperte qualquer tecla para continuar");
				AdicionaLinha(Log_Jogada, strBUFFER);
				Tecla = MT2D_Keyboard_keytouched();//trava o codigo até o jogador apertar algo

			}
			else {
				Tecla = MT2D_Keyboard_keytouched();//trava o codigo até o jogador apertar algo
				if (Tecla == escape_pressed) {
					FecharJogo = (bool)(1- MT2D_MessageBox_With_Result("Sair do jogo", "Voce deseja sair do jogo?", "NAO", "SIM"));
				}
				else if (Tecla == arrow_key_pressed) {
					Tecla = MT2D_Keyboard_keytouched();//trava o codigo até o jogador apertar algo
					switch(Tecla){
					case arrow_up_pressed:
						if (PonteiroY) {
							PonteiroY = (PonteiroY - 1) % 3;
						}
						break;
					case arrow_down_pressed:
						PonteiroY = (PonteiroY + 1) % 3;
						break;
					case arrow_left_pressed:
						if (PonteiroX) {
							PonteiroX = (PonteiroX - 1) % 3;
						}
						break;
					case arrow_right_pressed:
						PonteiroX = (PonteiroX + 1) % 3;
					}
				}
				else if (Tecla == enter_pressed) {
					if (Tabuleiro[PonteiroY][PonteiroX] != SemPeca) {
						Render_Posicao_Invalida_Efeito();
						sprintf(strBUFFER, "Posicao %d%d ja tem uma peca",PonteiroY,PonteiroX);
						AdicionaLinha(Log_Jogada, strBUFFER);
					}
					else {
						Tabuleiro[PonteiroY][PonteiroX] = (Peca)Vez;
						sprintf(strBUFFER, "Jogador %d botou peca em %d%d",Vez+1, PonteiroY, PonteiroX);
						AdicionaLinha(Log_Jogada, strBUFFER);

						Vez = !(bool)Vez;
						PonteiroY = 0;
						PonteiroX = 0;

					}
				}

			}

		}
		if (!FecharJogo) {
			Render_Pecas(Tabuleiro);
			MT2D_Draw_Window(DISPLAY_WINDOW1);

			//jogada finalizada, veja o que ocorreu e informe o jogador
			switch (Vencedor((Peca**)Tabuleiro)) {
			case -1:
				sprintf(strBUFFER, "Empate");
				AdicionaLinha(Log_Jogada, strBUFFER);
				Vitorias[0]++;
				Vitorias[1]++;
				break;
			case 1:
				sprintf(strBUFFER, "Jogador 1 venceu");
				AdicionaLinha(Log_Jogada, strBUFFER);
				Vitorias[0]++;
				break;
			case 2:
				sprintf(strBUFFER, "Jogador 2 venceu");
				AdicionaLinha(Log_Jogada, strBUFFER);
				Vitorias[1]++;
				break;
			}
			MT2D_MessageBox(strBUFFER);
		}
	}
	return 0;
}