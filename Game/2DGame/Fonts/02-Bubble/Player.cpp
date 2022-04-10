#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Sound.h"

#define DASH_ANGLE_STEP 4
#define JUMP_ANGLE_STEP 4
#define DASH_HEGIHT 96
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	bJumping = false;
	bDashing = false;
	bBouncing = false;
	death = false;
	godMode = false;
	dashGodMode = false;
	win = false;
	bWallJumping = false;
	wallJumpLeft = false;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));	
}

void Player::update(int deltaTime){
	sprite->update(deltaTime);
	if (posPlayer.y - 20 == 0) win = true;
	if (win) {
		Game::instance().goNextLvl(Game::instance().getActualLvl());
		win = false;
	}
	else {
		if (Game::instance().getKey('g')) godMode = !godMode;
		if (Game::instance().getKey('d')) dashGodMode = !dashGodMode;
		if (godMode) death = false;
		if (death) {
			death = false;
			posPlayer.x = Game::instance().getInitialX();
			posPlayer.y = Game::instance().getInitialY();
			Sound::instance().back2LifeEffect();
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			if (bDashing) {
				posPlayer.y -= 6;
			}
			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bDashing)) {
				posPlayer.y += 6;
				if (bDashing) {
					bDashing = false;
				}
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !bWallJumping) {
			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), death, bDashing)) {
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
				if (bDashing) {
					bDashing = false;
				}
				if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
					bClimbing = true;
					wallJumpLeft = true;
					bounceAngle = 0;
				}
				else bClimbing = false;
			}
			else bClimbing = false;
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !bWallJumping) {
			if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), death, bDashing)) {
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
				if (bDashing) {
					bDashing = false;
				}
				if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
					bClimbing = true;
					wallJumpLeft = false;
					bounceAngle = 0;
				}
				else bClimbing = false;
			}
			else bClimbing = false;
		}
		else {
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
		if (bJumping) {
			jumpAngle += JUMP_ANGLE_STEP;
			int currentY = posPlayer.y;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else {
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) {
					bJumping = false;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
						posPlayer.y = currentY;
					}
				}
				else bJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bDashing);
			}
		}
		if (bDashing) {
			if (dashAngle == 48) {
				bDashing = false;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				posPlayer.y -= 6;
				dashAngle += DASH_ANGLE_STEP;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bDashing)) {
					posPlayer.y += DASH_ANGLE_STEP;
					bDashing = false;
				}
			}
			else if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
				posPlayer.x -= DASH_ANGLE_STEP;
				dashAngle += DASH_ANGLE_STEP;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), death, bDashing)) {
					posPlayer.x += DASH_ANGLE_STEP;
					dashAngle += DASH_ANGLE_STEP;
				}
			}
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
				posPlayer.x += DASH_ANGLE_STEP;
				dashAngle += DASH_ANGLE_STEP;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), death, bDashing)) {
					posPlayer.x -= DASH_ANGLE_STEP;
				}
			}
		}
		if (bBouncing) {
			bounceAngle += JUMP_ANGLE_STEP;
			int currentY = posPlayer.y;
			if (bounceAngle == 180)
			{
				bBouncing = false;
				posPlayer.y = startY;
			}
			else {
				posPlayer.y = int(startY - 96 * sin(3.14159f * bounceAngle / 180.f));
				if (bounceAngle > 90) {
					bBouncing = false;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
						posPlayer.y = currentY;
					}
				}
				else bBouncing = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bDashing);
			}
		}
		if (bWallJumping) {
			bounceAngle += JUMP_ANGLE_STEP;
			if(wallJumpLeft) posPlayer.x += 2;
			else posPlayer.x -= 2;
			
			int currentY = posPlayer.y;
			if (bounceAngle == 180)
			{
				bWallJumping = false;
				posPlayer.y = startY;
			}
			else {
				posPlayer.y = int(startY - 96 * sin(3.14159f * bounceAngle / 180.f));
				if (bounceAngle > 90) {
					bWallJumping = false;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
						posPlayer.y = currentY;
					}
				}
				else bWallJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bDashing);
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), death, bDashing)) {
					bWallJumping = false;
				}
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), death, bDashing)) {
					bWallJumping = false;
				}
			}
		}
		else {//Estoy cayendo
			if (bClimbing) {
				posPlayer.y += 1;
				if (Game::instance().getKey('x') && !bBouncing) {
					Sound::instance().jumpEffect();
					bClimbing = false;
					bWallJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
			if (!bDashing && !bClimbing) posPlayer.y += FALL_STEP;
			if (Game::instance().getKey('c') && canDash) {
				Sound::instance().dashEffect();
				bDashing = true;
				dashAngle = 0;
				if (!dashGodMode) canDash = false;
				bJumping = false;
			}
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
				bClimbing = false;
				if (bBouncing) {
					bounceAngle = 0;
					startY = posPlayer.y;
					canDash = true;
				}
				canDash = true;
				posPlayer.y -= FALL_STEP;
				if (Game::instance().getKey('x') && !bBouncing) {
					Sound::instance().jumpEffect();
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
				if (Game::instance().getKey('c') && canDash) {
					Sound::instance().dashEffect();
					bDashing = true;
					dashAngle = 0;
					canDash = false;
					startY = posPlayer.y;
					bJumping = false;
				}
			}
			if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT) && bClimbing) {
				if(!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
					bClimbing = false;
				}
			}
			if (!Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && bClimbing) {
				if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, death, bBouncing)) {
					bClimbing = false;
				}
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}