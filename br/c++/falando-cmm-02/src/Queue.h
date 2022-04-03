//==============================================================================
//  Stack.h - Implementação da classe Queue.
//
//  Autor: Cesar Raitz Jr.
//  Data: xx-Mar-2022
//  Versão: 0.01
//
//  Visite o BLOG: https://cienciacomcafeina.blogspot.com
//
//  Utilize por sua conta e risco!
//==============================================================================
#pragma once
#include "Deque.h"  // container usados em Stack e Queue


namespace easy
{
	/// Implementação de Filas.
	template <class T, class Container = Deque<T>>
	class Queue
	{
		Container c;  // container para enfileirar
	
	public:
		Queue(int capacity=4): c(capacity) {} // CONSTRUTOR
		
		bool full() { return c.full(); }
		bool empty() { return c.empty(); }
		
		void push(T t) { c.push_back(t); }
		T& pop() { return c.pop_front(); }
		
		int size() { return c.size(); }
		int get_cap() { return c.get_cap(); }
		T& operator[](int i) { return c[i]; }
		
		// Exercício 1: Implementar uma função next() para olhar
		// o próximo elemento da fila/pilha sem removê-lo.
		T& next() { return c.front(); }
	};
}