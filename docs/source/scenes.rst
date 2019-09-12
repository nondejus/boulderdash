
Scenes
======

Intro
#####
There is nothing much. There are two sprites, cursor and background. When window size changes,
background streches, cursors' position is relative to window's size so it does not change.

The game has 2 themes, so there is a change theme option.


Pre-Stage
#########
This is shown to user everytime before the stage, it shows current level, score
and health. All components are relative to window size.


Stage
#####
This is the only complex scene in game. It is the game session. It contains two main
component sets, map and HUD. HUD is simple, on left there is a bar says how much diamond
you need to collect to open the door, on right there are score bar and time bar shows
you how much time left, on middle of the top there is a pause button and thats it.
The map part shows a part of the all map near to player, every block takes 30px30p space,
this is not changing, block sizes are fixed, when you change window size, it only effects
how much block is shown on screen. Everytime a stage starts, it initializates audio device
and sound and closing them when stage ends.

End
###
It says gameover and shows your final score. If you passed all levels it's green,
if you failed it's red and thats it.
