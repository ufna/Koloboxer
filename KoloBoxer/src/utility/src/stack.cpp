#include "stack.h"

Stack::Stack(int size)
{
	s = new Vect[size];
	top=0; s[top].isEmpty = true;
}

Stack::~Stack()
{
	delete[] s;
}

void Stack::clean()
{
	top=0;
	s[top].isEmpty = true;
}

void Stack::deleteLast()
{
	if(top > 0)
		top--;
}

void Stack::push(int x, int y, bool wB)
{
	/* if( (s[top].isEmpty == false) && (s[top].x == x) && 
		(s[top].y == y) && (s[top].withBox == wB) )  
	{
		top++; 
	} 
	else { */
		s[top].isEmpty = false;
		s[top].withBox = wB;
		s[top].x = x;
		s[top].y = y;
		//top++; 
		s[++top].isEmpty = true;
	// }
}

Vect Stack::pop()
{
	Vect ret;
	if(top == 0) {
		ret.x = 0;
		ret.y = 0;
		ret.t = 0;
		ret.withBox = false;
	}
	else {
		ret.t = top; top--;
		ret.x = s[top].x;
		ret.y = s[top].y;
		ret.withBox = s[top].withBox;
	}
	return ret;
}

/*
Vect Stack::up()
{
	Vect ret;
	if( (s[top].isEmpty == true) || (top == 0) ) {
		ret.x = 0;
		ret.y = 0;
	}
	else {
		ret.x = s[top-1].x;
		ret.y = s[top-1].y;
		top++;
	}
	return ret;
} */