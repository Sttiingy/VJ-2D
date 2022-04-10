#include "Sound.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <irrKlang.h>
#include <vector>

using namespace irrklang;



ISoundEngine* engineMusic;
ISoundEngine* engineEffects;
ISound* music;
ISound* effect;

Sound::Sound()
{
	init();
}

Sound::~Sound()
{
	music->drop();
	engineEffects->drop();
	engineMusic->drop();

}


void Sound::playMusic(char *url)
{
	engineMusic->stopAllSounds();
	music = engineMusic->play2D(url, true, false, true);
	music->setVolume(0.1f);
}

void Sound::stopMusic() {
	engineMusic->stopAllSounds();
}

void Sound::jumpEffect()
{
	effect = engineEffects->play2D("sounds/JumpFX.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Sound::init() {
	engineMusic = createIrrKlangDevice();
	engineEffects = createIrrKlangDevice();
}

void Sound::stopEffect() {
	engineEffects->stopAllSounds();
}
