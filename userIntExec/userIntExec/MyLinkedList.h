#include <stdio.h>
#include <stdlib.h>

typedef double tData;

typedef struct elem {
	struct elem * next;
	tData data;
} tElement, *tList;

/* Funcoes da Lista Encadeada */
tList listInit();				// Inicia uma Lista Encadeada
int addElement(tList*, tData);	// Adiciona um elemento na Lista
int remElement(tList*, int);	// Remove um elemento da Lista
int indexOf(tList, tData);		// Busca o indice de um elemento na Lista
tData dataOf(tList, int);		// Busca um elemento na lista a partir do indice
int getSize(tList);				// Retorna o tamanho atual da Lista Encadeada
void printList(tList);			// Exibe a Lista Completa na Tela
int remList(tList*);			// Exclui uma Lista Encadeada

								/**
								*  Inicia uma Lista Encadeada.
								*  @return Retorna um ponteiro nulo.
								*/
tList listInit() {
	return NULL;
}

/**
* Adiciona um Elemento na Lista.
* @param list é um ponteiro para a lista.
* @param data é o dado a ser armazenado.
* @return retorna 1 se foi adicionado com sucesso e 0 se ocorreu um erro.
*/
int addElement(tList *list, tData data) {
	tList aux = NULL;

	if (*list) {
		aux = *list;

		while (aux->next)
			aux = aux->next;

		aux->next = (tList)malloc(sizeof(tElement));
		if (aux->next)
			aux = aux->next;
	}
	else {
		*list = (tList)malloc(sizeof(tElement));
		aux = *list;
	}

	if (aux) {
		aux->data = data;
		aux->next = NULL;
		return 1;
	}

	return 0;
}

/**
* Remove um Elemento da Lista.
* @param list é um ponteiro para a lista.
* @param index é o indice do elemento a ser removido.
* @return retorna 1 se foi removido com sucesso e 0 se ocorreu um erro.
*/
int remElement(tList *list, int index) {
	int i;
	tList aux1 = NULL, aux2 = NULL;

	aux1 = *list;

	for (i = 0; aux1 && i < index; i++) {
		aux2 = aux1;
		aux1 = aux1->next;
	}

	if (aux1) {
		if (aux1->next)
			aux2->next = aux1->next;

		free(aux1);

		return 1;
	}

	return 0;
}

/**
* Busca o indice de um elemento na Lista .
* @param list é a lista para busca.
* @param data é o dado a ser buscado.
* @return retorna o indice da primeira ocorrencia do elemento na lista,
* 		   se o dado nao for encontrado, retornara -1.
*/
int indexOf(tList list, tData data) {
	int i;
	tList aux = list;

	for (i = 0; aux && aux->data != data; i++)
		aux = aux->next;

	return aux ? i : -1;
}

/**
* Busca um elemento na lista a partir do indice.
* @param list é a lista para busca.
* @param index é o indice do elemento.
* @return retorna o elemento buscado, se o elemento nao for encontrado,
* 		   retornara -1.
*/
tData dataOf(tList list, int index) {
	int i;
	tData data;
	tList aux = list;

	for (i = 0; aux && i < index; i++)
		aux = aux->next;

	if (aux) data = aux->data;
	else return -1;

	return data;
}

/**
* Retorna o tamanho atual da Lista Encadeada.
* @param list é a lista.
* @return retorna o tamanho atual da Lista.
*/
int getSize(tList list) {
	int i = 0;

	while (list) {
		i++;
		list = list->next;
	}

	return i;
}

/**
* Exibe a Lista completa na tela.
* @param list é a lista a ser exibida.
*/
void printList(tList list) {
	tList aux = list;

	while (aux) {
		printf("%f ", aux->data);
		aux = aux->next;
	}

	printf("\n");
}

/**
* Exclui uma Lista Encadeada.
* @param list é um ponteiro para a lista a ser excluida.
* @return retorna 1 se foi excluida com sucesso, e 0 se o contrario.
*/
int remList(tList *list) {
	tList aux = NULL;

	do {
		aux = *list;
		*list = (*list)->next;
		free(aux);
	} while (*list);

	return !*list ? 1 : 0;
}
