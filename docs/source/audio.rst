
Audio
=====

I used SDL_Audio for this project, the reason for i used this instead of SDL_Mixer
is i wanted to build this project with just using core sdl library.

I didn't use any sound file, i stored some frequencies equivalent to some notes in order to
make a melody. SDL_Audio does not have a function that allows us to play with the speed,
so i'm using different system. Notes are changing with time, when i change the changing rate,
i can change the speed.
A sound is basicly just a sine wave, so as a callback funtion for audio device, i wrote a callback
function which basicly does a triangle shaped sine wave.

.. code-block:: C

  void synth_triangle_callback(void *data, Uint8 *buffer, int length){
      Sound *sound = (Sound *)(data);
      for(int i = 0; i < length; ++i){
          buffer[i] = 2*(asin(sin(M_PI*sound->samplePos / sound->samplesPerSine)))/M_PI*AMPLITUDE;
          ++sound->samplePos;
      }
  }

It takes samples from a triangle shaped sine wave and pushes to the audio device buffer.

I store all sound information in a structure called Sound.

.. code-block:: C

  typedef struct{
    double sampleFreq;
    double samplesPerSine;
    int samplePos;
    int notePos;
    double speed;
    Note melody[MAX_MELODY_LEN];
    int melodyLen;
    int active;
  }Sound;

The first 3 variables are about making a sound, the other variables are about the melody.
:code:`int active` is just for making the sound more realistic, when the current note changes,
active becomes 0, and when it is 0 it does not make a sound. It really helps to get rid of
the continuous buzzer-like sound.
