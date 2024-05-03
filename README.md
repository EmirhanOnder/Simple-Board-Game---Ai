In this project you will write a two player board game.
- There is a 7x7 board, a parametric turn limit and a parametrical number of pieces for each player. Initial
position of these pieces must be random.
- On each turn current player makes a single move:Moving his piece to one of its four neighbours.
- Don’t forget the board is the same board for both players.
- Game ends when one of the players has no place to move a piece or the turn limit is reached.
- Your game will be played between AI and a user. In the beginning, your program will ask if the user will
be Player1 or 2. If user is Player1, the first move will be user’s. Otherwise computer will make the first
move. If user decides to play first, the input must be 1 otherwise it is 2.
- At the start of the game the number of pieces for a single player and the maximum number of turns will
be given by the user.
- After board is initialized and at the end of each turn you need to print the coordinate system and the
squares occupied with pieces labeled by their player indicator.

Computer’s turn
- Computer will move its piece to an available square and print the new position as shown below.
    ** Computer moves the piece at g4 to g5

User’s turn
- If it is user’s turn, user will type the desired move in same structure as the computer. First the coordinate
of the piece that is wanted to be moved and then the new coordinate of the piece.
    ** Choose piece to move: c4
    ** Choose the new position for c4: d4
    ** Player moves the piece at c4 to d4
- You must check whether the desired moves and the piece coordinates are valid or not! If not, ask user for
a valid move or a valid piece coordinate again.

Move Rules
- You need to define Player1’s indicator as “X” and Player2’s as “O” in the printed representation.
- Players can not move outside of the field. Players can not move to the squares occupied by the their
opponent or by their own pieces.
- Players can only move their piece to one of its four neighbours. Move can be vertical or horizontal.

Coordinate System
- Your coordinate system must meet the following rules:
- Columns are represented as numbers between “1 and 7”.
- Rows are represented as letters between “a-g”. (“a1” is top left square and “g7” is bottom right.)

Ending & Winning
- The game ends when one of the players can not make a valid move or the turn limit has been reached.
- When one of the players can not make a valid move, the game ends and the other player will be the
winner.
- When the turn limit is reached, the game ends and the player with more valid moveable space will be the
winner.


