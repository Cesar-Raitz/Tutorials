//==============================================================================
//  main.cpp - Testa a classe easy::Deque.
//
//  Autor: Cesar Raitz Jr.
//  Data: 11-out-2021
//  Versão: 0.01
//
//  Visite o BLOG: https://cienciacomcafeina.blogspot.com
//
//  Utilize por sua conta e risco!
//==============================================================================
#include <stdio.h>
// #include <assert.h>
#include "Deque.h"

using namespace easy;

void preenche_assim(Deque<int>& q) {
	for (int i = 0; i < q.get_cap(); i++)
		q.push_back(i);
}

void retira_assim(Deque<int>& q) {
	do { printf(" %d", q.pop_back());
	} while (!q.empty());
}

void preenche_assado(Deque<int>& q) {
	for (int i = 0; i < q.get_cap(); i++)
		q.push_front(i);
}

void retira_assado(Deque<int>& q) {
	do { printf(" %d", q.pop_front());
	} while (!q.empty());
}

void testa_vazia(Deque<int>& q)
{
	printf("Deque q: ");
	if (q.empty()) printf("empty");
	else if (q.full()) printf("full");
	else printf("neither");
	printf(" (size = %d)\n", q.size());
}

void testa_queue()
{
	printf("Testando easy::Deque\n");
	printf("--------------------\n");
	Deque<int> q(10);
	
	// Teste simples de uma FIFO
	printf("FIFO 1:");
	preenche_assim(q);
	retira_assado(q);
	printf("\n");
	
	printf("FIFO 2:");
	preenche_assado(q);
	retira_assim(q);
	printf("\n");
	
	// Teste simples de uma FILO
	printf("filo 1:");
	preenche_assim(q);
	retira_assim(q);
	printf("\n");
	
	printf("filo 2:");
	preenche_assado(q);
	retira_assado(q);
	printf("\n");
	
	testa_vazia(q); // deve estar vazia
	
	// Verifica erro ao retirar um item de uma lista vazia
	try { q.pop_front(); } catch (...) {}
	try { q.pop_back();  } catch (...) {}
	
	// Verifica erro ao colocar um item numa lista cheia
	preenche_assado(q);
	try { q.push_front(999); } catch (...) {}
	try { q.push_back(999);  } catch (...) {}
	
	testa_vazia(q); // deve estar cheia
	
	// Verifica o cálculo do tamanho da fila
	int i = q.get_cap();
	do {
		q.pop_back();
		assert(q.size() == --i);
		if (i == 7 || i == 2) testa_vazia(q);
	} while (!q.empty());

	// Testa acesso dos elementos um a um	
	preenche_assim(q);
	printf("FOR_EACH: ");
	FOR_EACH(x, q,
		printf("%d ", x);
		x = _i*2
		)
	printf("\nFOR_REVR: ");
	FOR_REVR(x, q, printf("%d ", x));
	printf("\n");
	
	// Verifica erro ao acessar um elemento inexistente
	try { q[20]; } catch (...) {}
}

void exercicio3()
{
	printf("\n");
	printf("Exercício 3\n");
	printf("-----------\n");
	// Inicializa as posições
	struct pos_t {
		int x, y;
		void print() {
			printf("x=%d  y=%d\n", x, y);
		}
	};
	Deque<pos_t> snake_pos(16);
	for (int x=5; x<9; x++)
		snake_pos.push_front(pos_t{x, 5});
	printf("Tamanho: %d\n", snake_pos.size());
	
	// Imprime as posições
	printf("Antes:\n");
	snake_pos.front().print();  // da cabeça
	snake_pos.back().print();   // do rabo
	
	// Avança uma posição para a direita
	pos_t& head = snake_pos.front();		// referencia a posição da cabeça
	pos_t next_pos{head.x+1, head.y};	// cria uma nova posição à direita
	snake_pos.push_front(next_pos);		// coloca na frente
	snake_pos.pop_back();					// retira a última posição
	
	// Imprime as posições
	printf("Depois:\n");
	snake_pos.front().print();  // da cabeça
	snake_pos.back().print();   // do rabo
}


int main()
{
	testa_queue();
	exercicio3();
	return 0;
}