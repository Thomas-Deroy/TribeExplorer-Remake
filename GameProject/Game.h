#pragma once
#include "BaseGame.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
#include "Character.h"
#include <vector>
#include "SoundStream.h"
#include "Map.h"
#include "Button.h"
#include "Enemy.h"
//Interactables
#include "Flag.h"
#include "Key.h"
#include "Portal.h"
#include "Door.h"
#include "Platform.h"

class Game : public BaseGame
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update(float elapsedSec) override;
	void Draw() const override;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:

	// ENUM
	enum class GameState { Game, Loading, LevelSelect, StartScreen };
	GameState m_GameState{ GameState::StartScreen };

	// GAME
	Character* m_Player;
	Map* m_Map;
	Map* m_MapDeadzone;
	SoundStream* m_Music;

	Door* m_Door1;
	Door* m_Door2;
	Key* m_Key1;
	Key* m_Key2;
	Portal* m_Portal;
	Flag* m_Flag;
	Platform* m_Platform;
	bool m_HasKey{ false };

	float EnemyHitCooldown{ 2.f };
	float AnimationDeadCoolDown{ 0 };
	float m_Lifes{ 2 };
	bool m_EnemyHits{ false };

	Point2f m_LastPosition;

	std::vector<Enemy*> m_Enemy;
	Enemy* m_Deadzone;

	// STARTSCREEN "GAMESTATE"
	Texture* m_StartScreen;
	Text* m_TribeExplorerText;
	Sprite* m_NonPlayableCharacter;
	TTF_Font* m_FontThin;
	Button* m_PlayButton;
	Button* m_ExitButton;
	SDL_Event* m_Quit{ new SDL_Event{ SDL_QUIT } };

	Text* m_GameWonTextPart1;
	Text* m_GameWonTextPart2;

	// LOADING "GAMESTATE"
	Sprite* m_LoadingBall;
	Text* m_LoadingText;
	TTF_Font* m_Font;

	int m_Counter{ 0 };

	bool m_DoneLoading{ false };

	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

};