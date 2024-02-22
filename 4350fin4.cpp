#include <iostream>
#include <cmath>
#include <omp.h>
// Jared Corman
//David Stothers 0643608

// COIS 4350 Asst. 5
// This program solves Chopsticks for a given number of fingers.

#define Max 5 // # of fingers

struct GameState
{
	int a;
	int b;
	int c;
	int d;
	bool p1;
	bool operator==(GameState x) const{
		if(a == x.a && b == x.b && c == x.c && d == x.d && p1 == x.p1){
			return true;
		}else{
			return false;
		}
	}
	bool compareStates(GameState other)
	{// This copies the above operator as a troubleshooting thing. True if identical, false if not.
    //This was also expanded upon to handle *functionally* identical gamestates as well as *actually* identical ones.
		if(p1==other.p1&&((a==other.a && b==other.b && c==other.c && d==other.d)||(a==other.b&&b==other.a&&c==other.c&&d==other.d)||(a==other.a&&b==other.b&&c==other.d&&d==other.c)||(a==other.b&&b==other.a&&c==other.d&&d==other.c)))
		{
			return true;
		}
		return false;
	}
    GameState operator=(GameState * x){
		return *x;
	}
};
struct Node{
	int val;
	int depth;
	int attack;
	GameState state;
	struct Node* parent;
	bool isRoot = false;

	struct Node * ac;
	struct Node * ad;
	struct Node * bc;
	struct Node * bd;
	struct Node * split;
	Node operator=(Node * x){
		return *x;
	}
};


int solve(Node state);
int solve(Node state, int alpha, int beta);
void print(GameState state);
void print(Node node);

int main(){

	// Origin state to mess with.
	GameState game;
	game.a = 1;
	game.b = 1;
	game.c = 1;
	game.d = 1;
	game.p1 = true;

	Node root;
	root.parent = NULL;
	root.isRoot=true;
	root.depth = 0;
	root.state = game;
	int fin = solve(root);
	printf("\n\n ===== Game Solved ===== \n  [score = %i]\n\n",fin);
	return 1;
}

void print(GameState state){
	printf("[%i, %i | %i, %i]\n",state.a,state.b,state.c,state.d);
}

void print(Node node){
	printf("depth: %i, val: %i",node.depth,node.val);
	print(node.state);
}

bool isTie(Node child){
	Node past = child;

	while(!child.isRoot && past.parent != NULL){
		past = *past.parent;

		if(past.state.compareStates(child.state)){
			return true;
		}
	}
	return false;
}

int solve(Node node){
	return solve(node, -2, 2);
}

int solve(Node node, int alpha, int beta){
	// Leaf cases (endstates)
	if( isTie(node) ){
		//printf("Tie,");
		node.val = 0;
		//print(node.state);
		return 0;
	}else if(node.state.a == 0 && node.state.b == 0){
		//printf("Win,");
		//print(node.state);
		node.val = -1;
		return -1;
	} else if (node.state.c == 0 && node.state.d == 0){
        //printf("Loss,");
		//print(node.state);
		node.val = 1;
		return 1;
    	}else {
		//printf(" .. ");
	}


	// non-leaf case
	Node next[5];
	int hold;
	if(node.state.p1){// Player One's Possible moves:
		#pragma omp parallel for
		for(int i=0;i<4;i++)
		{
        	next[i].state=node.state;
			switch (i)
			{
				case 0:// A attcks C
				next[i].attack=node.state.a;
        		next[i].state.c=node.state.a+node.state.c;
        		if(next[i].state.c>Max)
        		{
            		next[i].state.c=0;
        		}
				break;
				case 1:// A attcks D
				next[i].attack=node.state.a;
        		next[i].state.d=node.state.a+node.state.d;
        		if(next[i].state.d>Max)
        		{
            		next[i].state.d=0;
        		}
				break;
				case 2:// B attcks C
				next[i].attack=node.state.b;
        		next[i].state.c=node.state.b+node.state.c;
        		if(next[i].state.c>Max)
        		{
            		next[i].state.c=0;
        		}
				break;
				case 3:// B attcks D
				next[i].attack=node.state.b;
        		next[i].state.d=node.state.b+node.state.d;
        		if(next[i].state.d>Max)
        		{
            		next[i].state.d=0;
        		}
				break;
			}
			next[i].state.p1=false;
		}
	}else{// Player Two Possible moves:
		#pragma omp parallel for
		for(int i=0;i<4;i++)
		{
			next[i].state=node.state;
			switch(i)
			{
				case 0:// C attacks A
				next[i].attack=node.state.c;
        		next[i].state.a=node.state.a+node.state.c;
        		if(next[i].state.a>Max)
        		{
            		next[i].state.a=0;
        		}
				break;
				case 1:// D attacks A
				next[i].attack=node.state.d;
        		next[i].state.a=node.state.a+node.state.d;
        		if(next[i].state.a>Max)
        		{
            		next[i].state.a=0;
        		}
				break;
				case 2:// C attacks B
				next[i].attack=node.state.c;
                next[i].state.b=node.state.b+node.state.c;
                if(next[i].state.b>Max)
                {
                	next[i].state.b=0;
                }
				break;
				case 3:// D attacks B
				next[i].attack=node.state.d;
                next[i].state.b=node.state.b+node.state.d;
                if(next[i].state.b>Max)
                {
                	next[i].state.b=0;
                }
				break;
			}
			next[i].state.p1=true;
		}   
	}
	// Both Sides
	node.ac=&next[0];
	node.ad=&next[1];
	node.bc=&next[2];
	node.bd=&next[3];

	#pragma omp parallel for
	for(int i =0; i < 4; i++){
		if(next[i].state.a < next[i].state.b){// Ensure a >= b
                        hold = next[i].state.a;
                        next[i].state.a = next[i].state.b;
                        next[i].state.b = hold;
                }
                if(next[i].state.c < next[i].state.d){// Ensire c >= d
                        hold = next[i].state.c;
                        next[i].state.c = next[i].state.d;
                        next[i].state.d = hold;
                }
		// attaching parent, and depth.
                next[i].parent = &node;
                next[i].depth = node.depth+1;
	}

	int bestScore;
	int curScore;
	bool endEarly = false;
	if(node.state.p1){ // p1 evaluation (max player)
		bestScore = -2;
		#pragma omp parallel for private(curScore) shared(bestScore, alpha, beta)
		for(int i=0; i < 4; i++){ // each child
			if(  !(endEarly || next[i].attack == 0 )){

				curScore = solve(next[i], alpha, beta);

				if( curScore > bestScore ){
					bestScore = curScore;
				}
				if( curScore > alpha){
					alpha = curScore;
				}
				if( beta <= alpha ){
					endEarly = true;
				}
			}
		}
	}else{ // p2 evaluation (min player)
		bestScore = 2;
		#pragma omp parallel for private(curScore) shared(bestScore, alpha, beta)
		for(int i=0; i < 4; i++){
			if( !(endEarly || next[i].attack == 0) ){
				curScore = solve(next[i], alpha, beta);
				if( curScore < bestScore ){
					bestScore = curScore;
				}
				if( curScore < beta ){
					beta = curScore;
				}
				if( beta <= alpha ){
					endEarly = true;
				}
			}
		}
	}
	node.val = bestScore;
	//print(node);
	return bestScore;
}
