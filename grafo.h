/*Estrutura de dados para o grafo*/
typedef struct VERTICE{

	bool UBS;
	char* codigo;
	int grau;

}vertice;

typedef struct ARESTA{

	bool liberado;
	vertice** pontos; //Ponteiro que aponta para pontos usados na aresta
	int minutos; // Peso da aresta

}aresta;

/*Estrutura de dados para a lista*/
typedef struct NO_VERTICE{

	vertice* vertice;
	struct NO_VERTICE* proximo_vertice;

}no_vertice;

typedef struct LISTA_VERTICE{

	no_vertice* inicio;
	no_vertice* fim;

}lista_vertice;

typedef struct NO_ARESTA{

	aresta* aresta;
	struct NO_ARESTA* proxima_aresta;

}no_aresta;

typedef struct LISTA_ARESTA{

	no_aresta* inicio;
	no_aresta* fim;


}lista_aresta;

/*Grafo*/
typedef struct GRAFO{

	unsigned nro_vertices;
	lista_vertice* vertices; //Organizada em lista
	lista_aresta* arestas; //Organizada em lista

}grafo;

/*Funções para manipulção e interação com as listas*/

//Para a lista de vértices. É a função PONTO
void alocar_na_lista_v(grafo* grafo_da_cidade ,lista_vertice* lista, char* novo_codigo, bool se_eh_UBS){

	//Verificação se já existe o vértice(ponto).
	no_vertice* no_aux = lista -> inicio;

	while (no_aux != NULL)
	{
		if ( strcmp(no_aux -> vertice -> codigo, novo_codigo) == 0)
		{
			printf("DUPLICADO\n");
			return;
		}	
	
		no_aux = no_aux -> proximo_vertice;
	}
	
	//Inicialização e alocação de um novo vértice (ponto).
	no_vertice* novo_vertice = malloc(sizeof(no_vertice));
	novo_vertice -> vertice = malloc(sizeof(vertice));
	
	novo_vertice -> vertice -> codigo = novo_codigo; 
	novo_vertice -> vertice -> UBS = se_eh_UBS;
	novo_vertice -> vertice -> grau = 0;

	novo_vertice -> proximo_vertice = NULL;

	if (lista -> inicio == NULL){
	
		lista -> inicio = novo_vertice;
		lista -> fim = novo_vertice;
	
	}

	else{
	
		lista -> fim -> proximo_vertice = novo_vertice;	
		lista -> fim = novo_vertice;

	}

	grafo_da_cidade -> nro_vertices += 1;
	printf("OK\n");

	return;
}

//Para a lista de arestas.É a função TRECHO.
void alocar_na_lista_a( lista_aresta* lista_a, lista_vertice* lista_v,char* ponto_a, char* ponto_b, int peso){
	
	//Criação de um vetor char para os pontos

	char **pontos = malloc(2*sizeof(char**)); 
	*(pontos + 0) = ponto_a;
	*(pontos + 1) = ponto_b;
	
	//Verificação se o tempo não é 0 ou negativo e se não há tentativa de criação de um laço.
	if (peso <= 0 || strcmp(ponto_a,ponto_b) == 0){
	
		printf("INVALIDO\n");
		return;
	
	}
	
	//Validação se os vértices (pontos) já foram cadastrados.
	vertice** guard_aux = malloc(2*sizeof(vertice*));// Aproveitar o loop para a alocação dos vértices na aresta mais tarde.
	
	for(int index_p = 0; index_p < 2; ++index_p)
	{ 
		no_vertice* no_aux = lista_v -> inicio;

		while (true)
		{
			if (no_aux == NULL)
			{
				free(guard_aux);
				printf("NÃO EXISTE\n");
				return;	
			}
			
			if ( strcmp(no_aux -> vertice -> codigo,*(pontos + index_p)) == 0)
			{
				*(guard_aux + index_p) = no_aux -> vertice;
				break;
			}
			
			no_aux = no_aux -> proximo_vertice;

		}
	}

	//verificação se não há tentativa de criação de arestas paralelas
	
	no_aresta* no_aux = lista_a -> inicio;
	while(no_aux != NULL){
	
		if( strcmp(no_aux -> aresta -> pontos[0] -> codigo,ponto_a) == 0 && strcmp(no_aux -> aresta -> pontos[1] -> codigo,ponto_b) == 0 || strcmp(no_aux -> aresta -> pontos[0] -> codigo,ponto_b) && strcmp(no_aux -> aresta -> pontos[1] -> codigo,ponto_a) == 0){
		
			printf("DUPLICADO\n");
			return;
		
		}	
	
	}

	free(no_aux);

	//Inicialização do nó da aresta 
	no_aresta* nova_aresta = malloc(sizeof(no_aresta));
	nova_aresta -> aresta = malloc(sizeof(aresta));
	
	nova_aresta -> aresta -> pontos = malloc(2*sizeof(vertice*));
	nova_aresta -> aresta -> pontos[0] = *(guard_aux + 0);
	nova_aresta -> aresta -> pontos[1] = *(guard_aux + 1);
	
	free(guard_aux);

	nova_aresta -> aresta -> liberado = true;
	nova_aresta -> aresta -> minutos = peso;

	if (lista_a -> inicio == NULL){
	
		lista_a -> inicio = nova_aresta;
		lista_a -> fim = nova_aresta;
	
	}

	else{
	
		lista_a -> fim -> proxima_aresta = nova_aresta;	
		lista_a -> fim = nova_aresta;

	}

	printf("OK\n");

	return;
}

//Função que altera o estado da aresta(trecho). É as funções ALAGA e LIBERA.
void modificar_estado_aresta(lista_aresta* lista_a, lista_vertice* lista_v, char* ponto_a, char* ponto_b, bool libera){

	no_aresta* no_aux = lista_a -> inicio;
	
	while(no_aux != NULL)
	{
	
		if( (strcmp(no_aux -> aresta -> pontos[0] -> codigo,ponto_a) == 0 && strcmp(no_aux -> aresta -> pontos[1] -> codigo,ponto_b) == 0) || (strcmp(no_aux -> aresta -> pontos[0] -> codigo,ponto_b) == 0 && strcmp(no_aux -> aresta -> pontos[1] -> codigo,ponto_a)) == 0){
		
			if (no_aux -> aresta -> liberado != libera)
			{
				no_aux -> aresta -> liberado = libera;
				printf("OK\n");
				return;
			}
			
			else
			{
			
				printf("JA\n");
				return;
			}
		
		}	
	
		no_aux = no_aux -> proxima_aresta;
	
	}

	printf("NÃO EXISTE\n");
	return;
}

//Algoritmo de dijkstra e pilha de visitados. É as funções PASSO,TEMPO e ROTA.
/*
typedef struct NO_VISITADOS{

	vertice** vetor_v;
	struct NO_VISITADOS* proximo_visitado;

}no_visitado;

int procura_menor_distancia( int* distancia, no_visitado* visitado, int nro_vertices) {

	int menor = -1;
	int primeiro = 1;

	for (; indice < nro_vertices; ++indice){
	
		if (distancia[indice] >= 0 && visitado[indice] == 0){
		
			if (primeiro){
			
				menor = indice;
				primeiro = 0;
			
			}	
		
			else {
				if (distancia[menor] > distancia[indice])
					menor = indice;
			}
		}
	}
	
	return menor;

}

void menor_caminho(grafo *grafo_da_cidade, int* anterior, int* distancia, char* ponto_a, char* ponto_b){

	int contador = 0;
	int count = 0;
	int nro_vertices = 0;	
	no_visitado* visitado = NULL; 
	int u = 0;

	count = nro_vertices = grafo_da_cidade -> nro_vertices;

	visitado -> vetor_v = malloc(nro_vertices*sizeof(vertice*));

	for (contador = 0; contador < nro_vertices; ++contador){
	
		anterior[indice] = -1;
		distancia[indice] = -1;
		visitado[indice] = 0;
	
	}

	distancia[vertice_inicial] = 0;

	while (count > 0){
	
		u = procura_menor_distancia( distancia, visitado, nro_vertices);
	
		if (u == -1)
			break;
	
		visitado[u] = 1;
	
		count--;

		for (indice = 0; indice < gr -> grau[u]; indice++){
		
			ind = gr -> arestas[u][indice];
		
			if(distancia[ind] < 0){
			
				//distancia[ind] = distancia[u] + 1;
			
				distancia[ind] = distancia[u] + gr -> pesos[u][indice];
			
				anterior[ind] = u;
			}	
	
			else{
		
				if (distancia[ind] > distancia[u] + 1){
		
					//distancia[ind] = distancia[u] + 1;	
				
					distancia[ind] = distancia[u] + gr -> pesos[u][indice];

					anterior[ind] = u;
			
				}
			}
		}
	}

	free(visitado);

	return;
}
*/

/*funções de manipulação e interação com o grafo*/

grafo* criar_grafo(void){

	grafo* grafo_da_cidade = malloc(sizeof(grafo));
	
	grafo_da_cidade -> nro_vertices = 0;
	grafo_da_cidade -> vertices = malloc(sizeof(vertice)); //Inicializados com 2 para uma margem
	grafo_da_cidade -> arestas = malloc(sizeof(vertice));

	return grafo_da_cidade;
}

void libera_grafo(grafo* grafo_da_cidade){

	free(grafo_da_cidade -> arestas);
	free(grafo_da_cidade -> vertices);
	free(grafo_da_cidade);

	return;
}


