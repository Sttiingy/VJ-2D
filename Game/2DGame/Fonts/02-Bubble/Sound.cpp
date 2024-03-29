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

void Sound::GameOverEffect()
{
	effect = engineEffects->play2D("sounds/GameOverFX.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Sound::SelectEffect()
{
	effect = engineEffects->play2D("sounds/SelectFX.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Sound::dashEffect()
{
	effect = engineEffects->play2D("sounds/DashFX.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Sound::back2LifeEffect()
{
	effect = engineEffects->play2D("sounds/Back2Life.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Sound::BoingEffect() {
	effect = engineEffects->play2D("sounds/Boing.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Sound::DeathEffect() {
	effect = engineEffects->play2D("sounds/DeathFX.wav", false, false, true);
	effect->setVolume(0.35f);
}

void Sound::BaloonEffect() {
	effect = engineEffects->play2D("sounds/BaloonFX.wav", false, false, true);
	effect->setVolume(0.35f);
}

void Sound::godModeOn() {
	effect = engineEffects->play2D("sounds/GodModeOnFX.wav", false, false, true);
	effect->setVolume(0.75f);
}

void Sound::godModeOff() {
	effect = engineEffects->play2D("sounds/GodModeOffFX.wav", false, false, true);
	effect->setVolume(0.75f);
}

void Sound::dashGodModeOff() {
	effect = engineEffects->play2D("sounds/DashGodOff.wav", false, false, true);
	effect->setVolume(0.75f);
}

void Sound::dashGodModeOn() {
	effect = engineEffects->play2D("sounds/DashGodOn.wav", false, false, true);
	effect->setVolume(0.75f);
}

void Sound::FlowerEffect() {
	effect = engineEffects->play2D("sounds/FlowerFX.wav", false, false, true);
	effect->setVolume(0.75f);
}

void Sound::init() {
	engineMusic = createIrrKlangDevice();
	engineEffects = createIrrKlangDevice();
}

void Sound::stopEffect() {
	engineEffects->stopAllSounds();
}

void Sound::WinEffect() {
	effect = engineEffects->play2D("sounds/Win.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Sound::BeerEffect() {
	effect = engineEffects->play2D("sounds/getBeer.wav", false, false, true);
	effect->setVolume(0.35f);
}


