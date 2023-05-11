# War-Game

A simulation of the classic card game war using a singly linked list. 
Rules for the game can be found at this website: https://gamerules.com/rules/war-card-game/

In this program we will always assume only two players will play. This means each player will represent
its own linked list. As the rules state there are a total of 52 cards.
The following number list shows each set of cards in the deck. Each type contains 4 (♠, ♣, ♥, and ♦) cards
(hence 4 ∗ 13 = 52 cards total).
1. Ace
2. King
3. Queen
4. Jack
5. 10
6. 9
7. 8
8. 7
9. 6
10. 5
11. 4
12. 3
13. 2

To run the game, I used a for loop to simulate five rounds, calling the playRound function during each iteration to determine the winner. 
This function acts as the driver for the game logic and is responsible for maintaining the linked list appropriately. 
