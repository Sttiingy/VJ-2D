#pragma once
#include <string>
class Sound
{
public:
	static Sound &instance()
	{
		static Sound G;
		return G;
	}

	Sound();
	~Sound();
	void playMusic(char *url);
	void stopMusic();
	void jumpEffect();
	void GameOverEffect();
	void SelectEffect();
	void dashEffect();
	void back2LifeEffect();
	void BoingEffect();
	void DeathEffect();
	void stopEffect();
	void WinEffect();
	void BeerEffect();
private:

	void init();



};
