
Controls, Game and Player Updates
=================================

Controls and Player
###################
Player updates within a period. It moves only when it updates, but program takes
events all the time. If player is not moving and player presses a key, the player
will move even when the user releases the key before update. It helps to play one
block a time each time. But if the player is moving and user releases a key before
the update player won't move. It improves smoothness on move.
Move events handling in two steps,

* Finding the keys user presses
* Take players move direction from these keys

If user presses up and down in the same time, player won't move. If user presses up
and down and left, player will move left.

Game Updates
############
Everything depends on time, there is no delay or fixed fps. Everytime the one loop ends,
i took the time difference between start and end and adding the result to timer variable.
Every situation except event handling has its own period and its controlled by timer.
Events are always handled independent from timer.

Animations
**********
Animations update faster than the game events. There is nothing much.

Rocks
*****
Rocks update when game updates. Rocks contains two situation, active and deactive.
If a rock is deactive and it is capable to fall, it becomes active.
If a rock is active and it is capable to fall, it falls, if it's not, it becomes deactive.
When player go under a deactive rock, rock won't fall and won't be activated.
Rocks are only animated when they are active.
Diamonds acts like rocks but they are always animated.


Mobs
****
When game updates, every mob in map tries to move in its own way.
