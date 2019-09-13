
Level Structure and Level Editor
================================

All datas which a level contains stored at a structure called :code:`Level`.

.. code-block:: C

  typedef struct{
    int width;
    int height;
    int diamondsRequired;
    int time;
    double waterRate;
    Point playerStartPos;
    Point doorPos;
    Block ** blocks;
  }Level;

For saving this, i use fwrite and write them to a bin file. For create a new level
or edit an existing one, i created a level editor.

Guide to Level Editor
#####################

| If you want to create a new level, answer first question with n.
| Enter the sizes you want and than a window will pop up.
|
| If you want to edit an existing level, answer y. Enter the file name without *.bin*
| extension and than a window will pop up.
|
| It works like ms paint. You pick a brush from bottom than paint.
| After you done your work press save and check the terminal.
| Enter how many diamond is required for pass the level.
| Enter how much time do player got.
| Enter water spread rate, if you enter x it means water will spread every x second.
| Enter the file name you want to save as. (Don't add extension).

.. note::

  The values in parentheses shows previous value for editing existing level.
  Game only recognize the levels named **level[1-10].bin**.
