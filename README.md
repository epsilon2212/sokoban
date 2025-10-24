Introduction
Sokoban is a single-player logic game played on a two-dimensional board made up of square tiles.

Some tiles on the board are empty, while others contain walls or boxes.

A certain number of tiles are marked as target tiles. A target tile can be either an empty tile or one with a box on it.

One of the tiles contains the player character, which can move to tiles adjacent vertically or horizontally to its current position.

A tile in row *w* and column *k* is considered adjacent to:

* row *w - 1*, column *k*;
* row *w + 1*, column *k*;
* row *w*, column *k - 1*;
* row *w*, column *k + 1*.

The character can move onto a tile if it is empty or if there is a box that can be pushed.

A box can be pushed if the tile directly behind it, in the direction of movement, is empty. It is not possible to move the character or push a box outside the board. It is also not possible to push more than one box at a time.

The goal of the game is to place all boxes on the target tiles.

Task
Write a program that enables playing Sokoban.

The program reads a description of the initial state of the board, then reads and executes a sequence of commands.

The recognized commands include printing the current state of the board, pushing a box, and undoing a previously executed push.

Unlike typical Sokoban implementations, the user does not have to specify movements of the character over free tiles. The program automatically determines how to move the character to the position from which it can push a box in the specified direction.

Input format
The program input consists of a description of the initial board state and a non-empty sequence of commands, ending with a period (“.”). The first command is always a command to print the board state.

The program ignores any input after the terminating period.

The board description consists of non-empty lines containing single-character representations of the tile states:

* `-` : a free tile that is not a target tile;
* `+` : a free tile that is a target tile;
* `#` : a wall;
* `@` : a free tile (not a target tile) occupied by the character;
* `*` : a target tile occupied by the character;
* `[a .. z]` (lowercase letter): a tile (not a target tile) with a box labeled with that letter;
* `[A .. Z]` (uppercase letter): a target tile with a box labeled with the corresponding lowercase letter.

A valid board description must contain exactly one character. Each box, labeled with a Latin alphabet letter, may appear only once.

The program recognizes the following commands:

* End of line → print the current board state.
* `[a .. z][2 | 4 | 6 | 8]` (a lowercase letter followed by a digit 2, 4, 6, or 8) → push the box labeled with the given letter in the direction indicated by the digit.

  * 2 = down, 8 = up, 4 = left, 6 = right.
  * The character moves along a path of free tiles to reach the position from which the push can be made. The path cannot include walls or boxes.
  * If the push cannot be performed because the character cannot reach the box or the box cannot be moved, the board state remains unchanged.
* `0` → undo the last successfully executed and not yet undone push.

  * The character returns to the position it was in before the undone push.
  * If there are no pushes to undo, the command does not change the board state.

Output format
The program output consists of the results of executing the print commands, displaying the board state in the same format as the input description.
