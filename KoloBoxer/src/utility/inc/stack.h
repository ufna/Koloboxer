#ifndef STACK_H
#define STACK_H

struct Vect {
	int x,y,t;
	bool withBox;
	bool isEmpty;
};

class Stack {
	Vect *s;
	int top;
public:
	Stack(int size = 10000);
	~Stack();

	void clean();
	void deleteLast();
	void push(int x, int y, bool wB);
	Vect pop();
	// Vect up();
};

#endif // STACK_H
