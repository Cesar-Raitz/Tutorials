//==============================================================================
//  Deque.h
//
//  Autor: Cesar Raitz Jr.
//  Data: 11-out-2021
//  Versão: 0.01
//
//  Visite o BLOG: https://cienciacomcafeina.blogspot.com
//
//  Utilize por sua conta e risco!
//==============================================================================
#pragma once

// Se você não incluir <assert.h>, a condição errada
// apenas gera uma mensagem e volta para a função
// que chamou. APENAS PARA TESTAR A SEGURANÇA!
#if !defined(assert) && !defined(NDEBUG)
#define assert(condition) if (!(condition)) { \
		printf("> " #condition " in %s works!\n", \
			 __FUNCTION__); \
		throw (1); \
	}
#endif

// Um laço for para olhar todos os elementos de um
// container.  A variável _i é usada como contador.
#define FOR_EACH(element, deque, action) \
	for (int _i=0; _i<deque.size(); _i++) { \
		auto& element = deque[_i]; \
		action; }

// Um laço for para olhar todos os elementos de um
// container de trás pra frente. A variável _i é
// usada como contador.
#define FOR_REVR(element, deque, action) \
	for (int _i=deque.size()-1; _i; _i--) { \
		auto& element = deque[_i]; \
		action; }


namespace easy
{	
	/// Implementação de filas e pilhas.
	template <class T> class Deque
	{
		T* array;		// espaço em memória pra fila
		int capacity;	// número máximo de elementos
		int i0;			// índice do primeiro elemento
		int i1;			// índice do último elemento
		bool _full;		// a fila está cheia?
		
	public:
		Deque(int capacity = 4) {		// ---- CONSTRUTOR ----
			assert(capacity >= 1);		// evita fazer m****
			this->capacity = capacity;	// guarda a capacidade
			array = new T[capacity];	// reserva memória
			i0 = i1 = capacity/2;		// índices iniciais
			_full = false;					// começa vazia
		};
		
		~Deque() { delete[] array; }	// ---- DESTRUTOR -----
		
		/// Retorna true se a fila estiver cheia.
		bool full() { return _full; }
		
		/// Retorna true se a fila estiver vazia.
		bool empty() { return !_full && (i0 == i1); }
		
		/// Adiciona um elemento atrás da fila.
		void push_back(T item)
		{
			assert(!_full);                // não deve estar cheia!
			array[i1] = item;              // coloca na posição i1
			if (++i1 >= capacity) i1 = 0;  // avança i1 mas não deixa transbordar
			if (i1 == i0) _full = true;    // se alcançar i0, então está cheia!
		}
		
		/// Remove um elemento da frente da fila e retorna sua referência.
		T& pop_front()
		{
			assert(!empty());             // não deve estar vazia!
			_full = false;                // também não vai ficar cheia
			int i = i0;                   // guarda a posição da frente
			if (++i0 >= capacity) i0 = 0; // avança i0 sem deixar transbordar
			return array[i];              // retorna o elemento da frente
		}
		
		/// Adiciona um elemento na frente da fila.
		void push_front(T item)			// ---- Resposta 1 ----
		{
			assert(!_full);
			if (i0 == 0) i0 = capacity;
			array[--i0] = item;
			if (i1 == i0) _full = true;
		}
		
		/// Remove um elemento de trás da fila e retorna sua referência.
		T& pop_back()						// ---- Resposta 1 ----
		{
			assert(!empty());
			_full = false;
			if (i1 == 0) i1 = capacity;
			return array[--i1];
		}
		
		/// Retorna o tamanho atual da fila.
		int size() {
			if (full()) return capacity;
			if (i0 <= i1) return i1-i0;
			return capacity-i0+i1;
		}
		
		/// Retorna a capacidade (número máximo de elementos na fila).
		int get_cap() {
			return capacity;
		}
		
		T& operator[](int i) {
			assert(i >= 0);      // índice deve estar nos limites
			assert(i < size());  // da fila pra não fazer m*******
			i += i0;             // começa do começo (0 = i0, etc.)
			if (i >= capacity)   // caso tenha transbordado
				i -= capacity;    // ... recomeça do início
			return array[i];     // devolve como referência
		}
				
		/// Retorna uma referência para o primeiro elemento da fila.
		T& front() {							// ---- Resposta 2 ----
			assert(!empty());
			return array[i0];
		}
		
		/// Retorna uma referência para o último elemento da fila.
		T& back() {								// ---- Resposta 2 ----
			assert(!empty());
			if (i1) return array[i1-1];	// retorna o elemento em i1-1
			return array[capacity-1];		// caso especial quando i1==0
		}
	};
}
