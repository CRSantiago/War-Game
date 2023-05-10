//Name: Christopher Santiago
//Dr. Steinberg
//COP3502 Spring 2022
//Programming Assignment 3

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
	int rank;    //number
	char * type; //type of card
	struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck


int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);
	
	srand(seed); //seed set
	rules();
	
	int player; //1 or 2
	int result;

	
	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); //choose player
	
	for(int game = 1; game <= 5; ++game) //simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");

		result = playRound(); //play game
		
		if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
			printf("You won game %d.\n", game);
		else
			printf("I won game %d.\n", game);
	}
	
	return 0;
}

void rules()
{
	printf("Welcome to the card game war!\n");
	printf("Here are the rules.\n");
	printf("You have a pile of cards and I have a pile of cards.\n");
	printf("We will each pull the top card off of our respective deck and compare them.\n");
	printf("The card with the highest number will win the round and take both cards.\n");
	printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
	printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
	printf("Ready? Here we go!\n");
}


/*
	The openCardDeck function will setup the round by “opening the card deck pack”. The function will 
	open a text file called deck.txt and read the information to store in a linked list.
*/
card_t * openCardDeck()
{
	FILE *fptr = fopen("deck.txt", "r");
	
	char *name = (char *) malloc(sizeof(char) * 20);
	
	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}
	
	card_t * head = NULL;
	
	int cardrank = 13;
	int tracker = 1;
	while(fscanf(fptr, "%s", name) == 1)
	{			
		strcat(name, " ");
		
		if(tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 4)
		{
			strcat(name, "of Spades");
			head = insertBackSetup(head, name, cardrank);
			tracker = 0;
			--cardrank;
		}
		
		++tracker;
		
	}
	
	free(name);
	fclose(fptr);
	
	return head;
}


/*
	The insertBackSetup function will insert a card to the back of the original deck for the setup 
	purposes. The function takes three arguments. 
	The first argument is an address to a typedef struct 
	card_t which represents the head of the linked list that represents the original deck before being split 
	between the players. 
	The second argument is the name of the card (examples include “Ace of Hearts”, “2 of Clubs”, etc...). 
	The last argument is the rank of card that will be used for the game to determine who wins each card draw. The function returns an address to a typedef struct card_t that represents 
	the head of a linked list. 
*/
card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{
	
    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));
		
		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);
			
			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}
			
			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return node;  //terminate
		}
      
    }
	
	//here we know that the list has at least one node
	
	card_t *head = node; //keep pointer to head since we will need to return this address
	
	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;
	
	node->nextptr = (card_t *) malloc(sizeof(card_t)); 
	
	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}
	
	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);
	
	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}
	
	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; //very important
			
	return head; //return head node since we need our starting point of the linked list
}

// The empty function will determine if the linked list is empty. 
int empty(card_t * node)
{
	return (node == NULL); //return condition result
}

//The playRound function will simulate an ENTIRE round of the card game, returning the winning player
int playRound(){

	card_t * deckhead = NULL;
	card_t * p1head = NULL;
	card_t * p2head = NULL;
	card_t * temp = NULL;
	card_t * searchVal =  NULL;

	deckhead = openCardDeck();

	int size, rand_int, count = 0; // count is used to alternate which players receives a card

	size = deckSize(deckhead);
	printf("There are %d cards in the deck.\n", size);

	while(!(empty(deckhead))){
		size = deckSize(deckhead);
		rand_int = rand() % (size);
		
		// search for random card from original deck
		searchVal = search(deckhead, rand_int);

		// copy card to temp 
		temp = copyCard(searchVal);

		// remove card from original deck
		deckhead = removeCard(deckhead, rand_int);

		// insert the temp to players deck. even goes to player 2, odds to player 1
		if (count % 2 == 0){
			p2head = insertBackDeck(p2head, temp);
		} else {
			p1head = insertBackDeck(p1head, temp);
		}

		count++;

	}
	
	int result;
	//while player 1 or player 2 deck are not empty
	while(!(empty(p1head)) && !(empty(p2head))){

		// retrieve result from card comparison
		result = compareCard(p1head, p2head);
		
		// print out the "type" pulled from each player
		printf("Player 1 pulled out %s. \t ", p1head->type);
		printf("Player 2 pulled out %s.\n", p2head->type);
		temp = NULL;

		// Perform card actions based on result
		if(result == 1){
			printf("Player 1 won that round.\n");
			// copy player 2 card drawn
			temp = copyCard(p2head);

			// remove card from player 2
			p2head = removeCard(p2head, 0);

			// move card played by player 1 to the back of their deck
			p1head = moveCardBack(p1head);

			// move the card copied from player 2's deck to the back of player 1's deck.
			p1head = insertBackDeck(p1head, temp);
			//temp = NULL;
		} else if(result == 2){
			printf("Player 2 won that round.\n");
			// copy player 1 card drawn
			temp = copyCard(p1head);

			// remove card from player 1
			p1head = removeCard(p1head, 0);

			// move card played by player 2 to the back of their deck
			p2head = moveCardBack(p2head);

			// move the card copied from player 2's deck to the back of player 1's deck.
			p2head = insertBackDeck(p2head, temp);
			//temp = NULL;
		} else if (result == 0) {
			printf("Ugh oh...We have a tie! W-A-R!\n");
			int countTies = 1;
			int size1, size2;
			size1 = deckSize(p1head);
			size2 = deckSize(p2head);

			// while loop's condition accounts for multiple ties
			while(result == 0){
				
				//check how many ties have occured and each players deck size
				if(countTies == 1 && size1 >= 3 && size2 >= 3){
					// result from each player's third card
					result = compareCard(p1head->nextptr->nextptr, p2head->nextptr->nextptr);
				} else if (countTies == 1 && (size1 <= 2 || size2 <= 2)) {
					// one player has two or less cards, then result from each player's second cards
					result = compareCard(p1head->nextptr, p2head->nextptr);
				}else if (countTies == 2 && size1 >= 3 && size2 >= 3) {
					// if a second tie, result from each player's fifth card
					result = compareCard(p1head->nextptr->nextptr->nextptr->nextptr, p2head->nextptr->nextptr->nextptr->nextptr);
				} else if (countTies == 2  && (size1 <= 2 || size2 <= 2)){
					result = compareCard(p1head->nextptr, p2head->nextptr);
				} else if (countTies == 3 && size1 >= 3 && size2 >= 3) {
					// if a second tie, result from each player's seventh card
					result = compareCard(p1head->nextptr->nextptr->nextptr->nextptr->nextptr->nextptr, p2head->nextptr->nextptr->nextptr->nextptr->nextptr->nextptr);
				} else if (countTies == 3  && (size1 <= 2 || size2 <= 2)){
					result = compareCard(p1head->nextptr, p2head->nextptr);
				} 

				// if we don't have a tie
				if(result != 0){

					int tieBroken = 1;
					while(countTies >= 1){
					
						//used to recheck size if double tie
						size1 = deckSize(p1head);
						size2 = deckSize(p2head);
						if(size1 >= 3 && size2 >= 3){
							// prints if only 1 tie
							if(countTies == 1 && tieBroken == 1){
								printf("Player 1 pulled out %s. \t ", p1head->nextptr->nextptr->type);
								printf("Player 2 pulled out %s.\n", p2head->nextptr->nextptr->type);	
								printf("Player %d won that W-A-R!\n", result);
							} else if(countTies == 2 && tieBroken == 1) {
								printf("Player 1 pulled out %s. \t ", p1head->nextptr->nextptr->nextptr->nextptr->type);
								printf("Player 2 pulled out %s.\n", p2head->nextptr->nextptr->nextptr->nextptr->type);	
								printf("Player %d won that W-A-R!\n", result);
							} else if(countTies == 3 && tieBroken == 1){
								printf("Player 1 pulled out %s. \t ", p1head->nextptr->nextptr->nextptr->nextptr->nextptr->nextptr->type);
								printf("Player 2 pulled out %s.\n", p2head->nextptr->nextptr->nextptr->nextptr->nextptr->nextptr->type);	
								printf("Player %d won that W-A-R!\n", result);
							}

							tieBroken = 0;

							// if more than one tie, move the two first cards depending on winner
							if(countTies > 1 && result == 1){
								// move card from the first tie, to the back
								p1head = moveCardBack(p1head);

								// move card facedown card from player one to the back
								p1head = moveCardBack(p1head);

								/*
								copy player 2's card from first tie
								remove from player two's deck
								insert at the back of player 1's deck
								*/
								temp = copyCard(p2head);
								p2head = removeCard(p2head, 0);
								p1head = insertBackDeck(p1head, temp);
								
								/*
								copy player 2's first facedown card
								remove from player two's deck
								insert at the back of player 1's deck
								*/
								temp = copyCard(p2head);
								p2head = removeCard(p2head, 0);
								p1head = insertBackDeck(p1head, temp);

							} else if (countTies > 1 && result == 2){
								
								/*
								copy player 1's card from first tie
								remove from player 1's deck
								insert at the back of player 2's deck
								*/
								temp = copyCard(p1head);
								p1head = removeCard(p1head, 0);
								p2head = insertBackDeck(p2head, temp);

								/*
								copy player 1's first facedown card
								remove from player 1's deck
								insert at the back of player 2's deck
								*/
								temp = copyCard(p1head);
								p1head = removeCard(p1head, 0);
								p2head = insertBackDeck(p2head, temp);

								// move player 2's first tie, and first facedown to the back accordingly
								p2head = moveCardBack(p2head);
								p2head = moveCardBack(p2head);
							}
							if(result == 1 && countTies == 1) {
								//p1 (tie) -> p1 (facedown - nextptr) -> p2 (tie) -> p2(facedown - nextptr) - > p1 (win - nextptr->nextptr) -> p2 (lose - nextptr->nextptr)
								p1head = moveCardBack(p1head);
								p1head = moveCardBack(p1head);
								//move over tie card from p2
								temp = copyCard(p2head);
								p2head = removeCard(p2head, 0);
								p1head = insertBackDeck(p1head, temp);

								//temp = NULL;
								//move facedowncard
								temp = copyCard(p2head);
								p2head = removeCard(p2head, 0);
								p1head = insertBackDeck(p1head, temp);

								p1head = moveCardBack(p1head);

								temp = copyCard(p2head);
								p2head = removeCard(p2head, 0);
								p1head = insertBackDeck(p1head, temp);

								//temp = NULL;
							} else if(result == 2 && countTies == 1){
								
								temp = copyCard(p1head);
								p1head = removeCard(p1head, 0);
								p2head = insertBackDeck(p2head, temp);

								//temp = NULL;

								temp = copyCard(p1head);
								p1head = removeCard(p1head, 0);
								p2head = insertBackDeck(p2head, temp);

								p2head = moveCardBack(p2head);
								p2head = moveCardBack(p2head);

								//temp = NULL;

								temp = copyCard(p1head);
								p1head = removeCard(p1head, 0);
								p2head = insertBackDeck(p2head, temp);
								
								//temp = NULL;

								p2head = moveCardBack(p2head);

							}
							countTies--;
						} else {
							printf("Player 1 pulled out %s. \t ", p1head->nextptr->type);
							printf("Player 2 pulled out %s.\n", p2head->nextptr->type);
							printf("Player %d won that W-A-R!\n", result);
							if(result == 1) {
								//p1 (tie) -> p2 (tie) -> p1 (win ->nextptr) -> p2 (lose ->nextptr)
								p1head = moveCardBack(p1head);
								//move over tie card from p2
								temp = copyCard(p2head);
								p2head = removeCard(p2head, 0);
								p1head = insertBackDeck(p1head, temp);

								//temp = NULL;

								p1head = moveCardBack(p1head);

								temp = copyCard(p2head);
								p2head = removeCard(p2head, 0);
								p1head = insertBackDeck(p1head, temp);

								//temp = NULL;
							} else if(result == 2){

								//p1 (tie) -> p2 (tie) -> p1 (lose ->nextptr) -> p2 (win ->nextptr)

								temp = copyCard(p1head);
								p1head = removeCard(p1head, 0);
								p2head = insertBackDeck(p2head, temp);
								
								p2head = moveCardBack(p2head);

								temp = copyCard(p1head);
								p1head = removeCard(p1head, 0);
								p2head = insertBackDeck(p2head, temp);
								//temp = NULL;

								p2head = moveCardBack(p2head);

								//temp = NULL;
							}
							countTies--;
						}
					}
				} else{
					countTies++;
					if(countTies == 2){
						printf("Player 1 pulled out %s. \t ", p1head->nextptr->nextptr->type);
						printf("Player 2 pulled out %s.\n", p2head->nextptr->nextptr->type);
						size = deckSize(p1head);
						printf("Player 1 has %d cards.\n", size-2);

						size = deckSize(p2head);	
						printf("Player 2 has %d cards.\n", size-2);
					} else if (countTies == 3){
						printf("Player 1 pulled out %s. \t ", p1head->nextptr->nextptr->nextptr->nextptr->type);
						printf("Player 2 pulled out %s.\n", p2head->nextptr->nextptr->nextptr->nextptr->type);
						size = deckSize(p1head);
						printf("Player 1 has %d cards.\n", size-4);

						size = deckSize(p2head);	
						printf("Player 2 has %d cards.\n", size-4);
					}
					printf("Ugh oh...We have a tie! W-A-R!\n");
				}
					
			}
		}
		size = deckSize(p1head);
		printf("Player 1 has %d cards.\n", size);

		size = deckSize(p2head);
		printf("Player 2 has %d cards.\n", size);
		
	}


	// determine winner based on empty deck, free winner's decks memory
	if(!empty(p1head)){
		result = 1;
		cleanUp(p1head);
	} else {
		result = 2;
		cleanUp(p2head);
	}

	return result;
}

/*
	The deckSize function will count the number of cards in the linked list. The functions takes a reference 
	to the head of the linked list and returns the number of cards in the linked list. 
*/
int deckSize(card_t * head){
	int count = 0;
	card_t * temp = head;
	while(temp != NULL){
		temp = temp->nextptr;
		count++;
	}
	return count;
}

/*
	The search function will traverse the linked list for a specific card in the pile. The function takes two 
	arguments. 
	The first argument is the reference to the linked list 
	The second argument is an integer which is the location of the list called spot. 
	The location starts with 0 (like indexing with an array) and goes up to the total number of nodes in the linked list minus 1.
*/
card_t * search(card_t * node, int spot){
	card_t * temp = node;
	int count = 0;
	while(temp != NULL){
		if(count == spot){
			return temp;
		}
		count++;
		temp = temp->nextptr;
	}
	return NULL;
}

/*
	The removeCard function will remove a node from the linked list. The function has two parameters. 
	The first one is the reference to the linked list (head specifically).
	The second argument is an integer which represents the location (like indexing with an array). 
	The function will deallocate the node in the respective spot from memory. 
	The function returns the head of the modified linked list. 
*/
card_t * removeCard(card_t * node, int spot) {

	if(empty(node)){
		return NULL;
	}

	card_t * head = node;
	card_t * temp = search(node, spot);

	if(temp == NULL){
		return head;
	}

	if(head == temp){
		head = head->nextptr;
		free(temp->type);
		free(temp);
		return head;
	}

	while(node->nextptr != temp){
		node = node->nextptr;
	}

	node->nextptr = temp->nextptr;
	free(temp->type);
	free(temp);
	return head;
	
}

// The copyCard function will create a deep copy of a node in a linked list and return it. The parameter in the function header is the node that will be copied and returned.
card_t * copyCard(card_t * node){
	card_t * copy = (card_t *) malloc(sizeof(card_t));
	copy->type = (char *) malloc(sizeof(char) * 20);
	copy->rank = node->rank;
	strcpy(copy->type, node->type);
	copy->nextptr = NULL;
	return copy;
}

/*
	The insertBackDeck function will place a card to the back of the deck. In coding terms, place the 
	node at the end of the linked list. There are two parameters in the function header.
	The first parameter is 
	the reference to the head of the linked list and 
	the second is the node that will be inserted to the end. This function simulates putting a card at the bottom of a pile. 
*/
card_t * insertBackDeck(card_t *head, card_t *node){
	if(head == NULL){
		head = node;
	}
	else if(head->nextptr == NULL){
		head->nextptr = node;
	} else {
		insertBackDeck(head->nextptr, node);
	}
	return head;
}

/*
	The compareCard function will compare the rankings of two cards. 
	This simulates when players are drawing the cards from the respective piles. 
*/
int compareCard(card_t * cardp1, card_t * cardp2){
	if(cardp1->rank > cardp2->rank){
		return 1;
	} else if (cardp1->rank < cardp2->rank){
		return 2;
	} else {
		return 0;
	}
}


/*
	The moveCardBack function will take the card in front of the pile and place it in the back. 
	The function has one parameter which is the head of the linked list. 
	After moving the card to the back, the function returns the new head of the linked list
*/
card_t * moveCardBack(card_t *head){
	card_t * first = head;
	card_t * last = head;

	while(last->nextptr != NULL){
		last = last->nextptr;
	}

	last->nextptr = first;
	head = head->nextptr;
	last->nextptr->nextptr = NULL;

	return head;
}

// The cleanUp function will free the allocated memory used.
void cleanUp(card_t * head){

	card_t * temp = head;
	while(head != NULL){
		temp = head->nextptr;
		free(head->type);
		free(head);
		head = temp;
	}
}