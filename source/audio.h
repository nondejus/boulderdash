#ifndef AUDIO_H
#define AUDIO_H

#define STOCK_BPM 150
#define MELODY_LEN 15

typedef struct{
  int len;
  double freq;
}Note;

typedef struct{
  double sampleFreq;
  double samplesPerSine;
  int samplePos;
  int notePos;
  double speed;
  int active;
  Note melody[MELODY_LEN];
}Sound;

double get_speed(double bpm);
void init_sound(Sound * sound);
void sound_update(Sound * sound, double timer,double *soundCounter);
void synth_sin_callback(void *data, Uint8 *buffer, int length);
void synth_triangle_callback(void *data, Uint8 *buffer, int length);

#endif
