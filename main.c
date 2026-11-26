#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // O booleano fica mais legivel
#include <string.h>
#include "grafo.h" //Biblioteca criada para o grafo

int main(void){

	//Main não trabalhado ainda
	//As funções chamadas aqui são para testar a integridade do grafo e das listas

	grafo* grafo_da_cidade = criar_grafo();//Cria o grafo

	alocar_na_lista_v(grafo_da_cidade, grafo_da_cidade -> vertices, "AAA", false);// chamada PONTO
	alocar_na_lista_v(grafo_da_cidade, grafo_da_cidade -> vertices, "BBB", false);// chamada PONTO

	alocar_na_lista_a(grafo_da_cidade -> arestas, grafo_da_cidade -> vertices, "BBB","AAA",1); //chamda TRECHO

	modificar_estado_aresta(grafo_da_cidade -> arestas, grafo_da_cidade -> vertices, "AAAA","BBB",false);// chamda ALAGA ou LIBERA

	libera_grafo(grafo_da_cidade);//Lberado o grafo

	return 0;
}
