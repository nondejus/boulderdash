#include <SDL2/SDL.h>
#include <math.h>
#include "audio.h"


#define AMPLITUDE 32
#define SAMPLE_RATE 44100

#define NOTE_DELAY 3

#define N_C4 261.63
#define N_CS4 277.18
#define N_D4 293.66
#define N_DS4 311.13
#define N_E4 329.63
#define N_F4 349.23
#define N_FS4 369.99
#define N_G4 392.00
#define N_GS4 415.30
#define N_A4 440.00
#define N_AS4 466.16
#define N_B4 493.88
#define N_SILENCE 99999999

double get_speed(double bpm){
  return (60.0/bpm)/2;
}

void init_melody(Sound * sound){
  Note * melody = sound->melody;
  melody[0].freq=N_A4;
  melody[0].len=2;
  melody[1].freq=N_B4;
  melody[1].len=2;
  melody[2].freq=N_CS4*2;
  melody[2].len=2;
  melody[3].freq=N_E4*2;
  melody[3].len=2;
  melody[4].freq=N_FS4*2;
  melody[4].len=2;
  melody[5].freq=N_GS4*2;
  melody[5].len=1;
  melody[6].freq=N_FS4*2;
  melody[6].len=2;
  melody[7].freq=N_E4*2;
  melody[7].len=2;
  melody[8].freq=N_CS4*2;
  melody[8].len=6;
  melody[9].freq=N_SILENCE;
  melody[9].len=2;
  melody[10].freq=N_B4;
  melody[10].len=2;
  melody[11].freq=N_CS4*2;
  melody[11].len=1;
  melody[12].freq=N_B4;
  melody[12].len=2;
  melody[13].freq=N_A4;
  melody[13].len=2;
  melody[14].freq=N_A4;
  melody[14].len=1;
}

void init_sound(Sound * sound){
  sound->sampleFreq = SAMPLE_RATE;
  sound->samplesPerSine = sound->sampleFreq/N_SILENCE;
  sound->samplePos = 0;
  sound->notePos = 0;
  sound->active = 0;
  sound->speed = get_speed(STOCK_BPM);
  init_melody(sound);
}

void nextNote(Sound * sound){
  sound->notePos+=1;
  if(sound->notePos>=MELODY_LEN)sound->notePos=0;
}

void sound_update(Sound * sound, double timer,double *soundCounter){
  if(sound->active < NOTE_DELAY){
    sound->samplesPerSine = sound->sampleFreq/N_SILENCE;
    sound->active++;
  }
  else{
    sound->samplesPerSine = sound->sampleFreq/sound->melody[sound->notePos].freq;
    if(timer>*soundCounter){
      sound->active = 0;
      nextNote(sound);
      (*soundCounter)+=sound->speed*sound->melody[sound->notePos].len;
    }
  }
}

void synth_sin_callback(void *data, Uint8 *buffer, int length){
    Sound *sound = (Sound *)(data);
    for(int i = 0; i < length; ++i){
        buffer[i] = (sin(sound->samplePos / sound->samplesPerSine) + 1)*AMPLITUDE;
        ++sound->samplePos;
    }
}
void synth_triangle_callback(void *data, Uint8 *buffer, int length){
    Sound *sound = (Sound *)(data);
    for(int i = 0; i < length; ++i){
        buffer[i] = 2*(asin(sin(M_PI*sound->samplePos / sound->samplesPerSine)))/M_PI*AMPLITUDE;
        ++sound->samplePos;
    }
}
/*

typedef struct{
  int ToneHz;
  int ToneVolume;
  int WavePeriod;
  int SampleIndex;
  int SamplesPerSecond;
  int BytesPerSample;
}platform_audio_config;

typedef struct{
  Uint8* Buffer;
  int Size;
  int ReadCursor;
  int WriteCursor;
  platform_audio_config* AudioConfig;
}platform_audio_buffer;
*/
/*
void loop_callback(void *data, Uint8 *buffer, int bufferLen){
  platform_audio_buffer* audio = (platform_audio_buffer*)data;
  int size1 = bufferLen;
  int size2 = 0;

  if (audio->ReadCursor + bufferLen > audio->Size){
    size1 = audio->Size - audio->ReadCursor;
    size2 = bufferLen - size1;
  }

  SDL_memcpy(buffer,(audio->Buffer + audio->ReadCursor),size1);
  SDL_memcpy(&buffer[size1],audio->Buffer,size2);

  audio->ReadCursor = (audio->ReadCursor + bufferLen) % audio->Size;
}
*/
