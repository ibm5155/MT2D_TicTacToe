#pragma once
#include "Texto.h"

void Render_Pecas(Peca **Tabuleiro);
void Render_Titulo(int *Vitorias1, int *Vitorias2, bool *CPU1, bool *CPU2,int *Vez);

void Render_Log(Texto *texto, int *Linha_Inicial);
void Render_Selecionado(int &X, int &Y);
void Render_Tabuleiro();//não desenha as peças, só o tabuleiro
void Render_Posicao_Invalida_Efeito();