#include <stdio.h>
#define ROCK 1
#define PAPER 2
#define SCISSORS 3

int main(){
	char player;
	char enemy;
	char guess;
	int points = 0;
	while((guess = getchar()) != EOF){
		if(guess == 'A') enemy=ROCK;
		else if(guess == 'B') enemy=PAPER;
		else if(guess == 'C') enemy=SCISSORS;
		else if(guess == 'X') player=ROCK;
		else if(guess == 'Y') player=PAPER;
		else if(guess == 'Z') player=SCISSORS;
		else if(guess == '\n'){
			if((player == ROCK && enemy == SCISSORS) ||
			   (player == PAPER && enemy == ROCK) ||
			   (player == SCISSORS && enemy == PAPER))
				points += 6;
			else if(player == enemy)
				points += 3;
			points += player;
		}
	}

	printf("Sum of points: %d\n", points);
	return 0;
}
