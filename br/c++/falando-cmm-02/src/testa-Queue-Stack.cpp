#include <stdio.h>
#include <assert.h>

// Classes da biblioteca STD
#include <stack>
#include <queue>

// Classes da biblioteca EASY
#include "Stack.h"
#include "Queue.h"

// Se você estiver usando o Visual Studio Code da Microsoft,
// o inspetor de código (IntelliSense) pode reclamar do operador
// lógico "not" dentro do laço "while". Isto porque estes operadores
// apareceram somente na versão 20 do padrão c++ e a configuração do
// IntelliSense (File > Settings > Pesquisar intellisense mode) deve
// ser alterada para "c++20".

// https://code.visualstudio.com/docs/cpp/config-mingw#_cc-configurations
// https://code.visualstudio.com/docs/editor/intellisense


const char* frases[] = {
	"1 - Ante-penúltima",
	"2 - Penúltima!",
	"3 - Última!!"
};

/// Retorna o número de itens dentro de uma lista
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(*(array)))

/// Preenche um container com elementos da lista usando push()
#define FILL_WITH(container, list) \
	for (auto i=0; i<ARRAY_SIZE(list); i++) \
		container.push(list[i]);


namespace easy {
void testa_Stack() {
	printf("===== Testando easy::Stack\n");
	Stack<const char*> pilha;
	FILL_WITH(pilha, frases);
	while (not pilha.empty()) {   // enquanto não estiver vazia...
		printf(pilha.pop());       // verifica e retira aqui
		printf("\n");
	}
	printf("\n");
}}

namespace std {
void testa_stack() {
	printf("===== Testando std::Stack\n");
	stack<const char*> pilha;
	FILL_WITH(pilha, frases);
	while (not pilha.empty()) {   // enquanto não estiver vazia...
		printf(pilha.top());       // verifica quem está no topo
		pilha.pop();               // retira aqui!
		printf("\n");
	}
	printf("\n");
}}

namespace easy {
void testa_Queue() {
	printf("===== Testando easy::Queue\n");
	Queue<const char *> fila;
	FILL_WITH(fila, frases);
	while (not fila.empty()) {    // enquanto não estiver vazia...
		printf(fila.pop());        // verifica e retira aqui
		printf("\n");
	}
	printf("\n");
}}

namespace std {
void testa_queue() {
	printf("===== Testando std::Stack\n");
	queue<const char*> fila;
	FILL_WITH(fila, frases);
	while (not fila.empty()) {    // enquanto não estiver vazia...
		printf(fila.front());      // verifica quem está atrás
		fila.pop();                // retira aqui!
		printf("\n");
	}
	printf("\n");
}}


// Exercício 1: Criar uma fila e uma pilha com números de 1 a 10
// e verificar se a função next() remete ao próximo elemento do
// container sem retirá-lo.
void exercicio1() {
	printf("===== EXERCÍCIO 1\n");
	int numeros[] = {1,2,3,4,5,6,7,8,9,10};
	
	// Note que, se as capacidades da fila e pilha não forem especificados,
	// os containers vão inundar e será gerado um erro do tipo:
	// Assertion failed: !_full, file <...>\Deque.h, line 72
	
	easy::Queue<int> fila(10);		// Capacidade 10 pois sabemos
	easy::Stack<int> pilha(10);	// a quantidade de números.
	FILL_WITH(fila, numeros);
	FILL_WITH(pilha, numeros);
	
	assert(fila.next() == 1 and fila.size() == 10);
	assert(pilha.next() == 10 and pilha.size() == 10);
	printf("Ok!\n");
}


int main()
{
	// Classes da biblioteca Std
	std::testa_stack();
	std::testa_queue();
	
	// Teste das nossas classes. Devem se comportar
	// exatamente da mesma maneira
	easy::testa_Stack();
	easy::testa_Queue();
	
	exercicio1();
	return 0;
}