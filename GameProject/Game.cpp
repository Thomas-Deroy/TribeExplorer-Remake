#include "pch.h"
#include "Game.h"
#include <iostream>

Game::Game(const Window& window)
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	// - GAME - 
	m_Player		= new Character(Point2f{ 100.f, 500.f });
	m_Map			= new Map("Map1.svg", "Map.png");
	m_MapDeadzone	= new Map("DeadZoneMap.svg");

	// - Interactables -
	m_Door1			= new Door("Door.png", Point2f{ 1825.f, 850.f });
	m_Door2			= new Door("Door.png", Point2f{ 5080, 850 });
	m_Key1			= new Key("Key.png", Point2f{ 1775.f, 300.f });
	m_Key2			= new Key("Key.png", Point2f{ 3800, 600.f });
	m_Portal		= new Portal("Portal.png", Point2f{ 6900.f, 600.f }, "PortalReverse.png", Point2f{ 7600.f, 700.f });
	m_Flag			= new Flag("EndingFlag.png", Point2f{ 9160.f,690.f });
	m_Platform		= new Platform(Point2f{ 5565, 970 }, 340.f);

	// - Enemy -
	m_Deadzone = new Enemy();
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 550,600 }, 400));
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 840,800 }, 200));
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 1125,800 }, 200));
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 1575,500 }, 300));
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 2750,875 }, 450, false));
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 8750,800 }, 200));
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 8850,800 }, 200));
	m_Enemy.emplace_back(new Enemy("GhostSprite.png", 10, Point2f{ 8950,800 }, 200));
	m_Enemy.emplace_back(new Enemy("BatSprite.png", 7, Point2f{ 3320, 850 }, 200));
	m_Enemy.emplace_back(new Enemy("BatSprite.png", 7, Point2f{ 4100, 850 }, 200));
	m_Enemy.emplace_back(new Enemy("BatSprite.png", 7, Point2f{ 4450, 950 }, 150));
	m_Enemy.emplace_back(new Enemy("BatSprite.png", 7, Point2f{ 4800, 800 }, 200));
	m_Enemy.emplace_back(new Enemy("BatSprite.png", 7, Point2f{ 3800, 675 }, 200, false));
	m_Enemy.emplace_back(new Enemy("BatSprite.png", 7, Point2f{ 5710, 1100 }, 200, false));
	m_Enemy.emplace_back(new Enemy("BatSprite.png", 7, Point2f{ 8150, 800 }, 200));
	m_Enemy.emplace_back(new Enemy("CrawlerSprite.png", 14, Point2f{ 3700, 970 }, 200, false));
	m_Enemy.emplace_back(new Enemy("CrawlerSprite.png", 14, Point2f{ 4450, 680 }, 200, false));
	m_Enemy.emplace_back(new Enemy("CrawlerSprite.png", 14, Point2f{ 6000, 800 }, 100, false));
	m_Enemy.emplace_back(new Enemy("CrawlerSprite.png", 14, Point2f{ 6500, 1025 }, 160, false));

	// - LOADING "GAMESTATE" - 
	m_Font				= TTF_OpenFont("Font.ttf", 120);
	m_LoadingText		= new Text("LOADING", Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, m_Font);
	m_LoadingBall		= new Sprite("LoadingAnimationBall.png", 10, 10, 0.03f, 1.f);

	// - STARTSCREEN "GAMESTATE" - 
	m_FontThin				= TTF_OpenFont("FontThin.ttf", 50);
	m_StartScreen			= new Texture("StartScreen.png");
	m_TribeExplorerText		= new Text("TRIBE EXPLORER", Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, m_FontThin);
	m_NonPlayableCharacter	= new Sprite("IdleCharactarSprite.png", 1, 11, 0.03f, 1.f);
	m_ExitButton			= new Button(Point2f{ GetViewPort().width / 6,GetViewPort().height / 3 }, "EXIT", Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, m_FontThin);
	m_PlayButton			= new Button(Point2f{ GetViewPort().width/6,GetViewPort().height/2 }, "PLAY", Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, m_FontThin);
	
	m_GameWonTextPart1		= new Text("Thank you", Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, m_FontThin);
	m_GameWonTextPart2		= new Text("for playing the game", Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, m_FontThin);

	// - MUSIC -
	m_Music = new SoundStream("TribeExplorerMusic.mp3");
}

void Game::Cleanup()
{
	// - GAME -
	delete m_Player;		m_Player = nullptr;
	// - Music -					
	delete m_Music;			m_Music = nullptr;
	// - Interactables -			
	delete m_Door1;			m_Door1 = nullptr;
	delete m_Door2;			m_Door2 = nullptr;
	delete m_Key1;			m_Key1 = nullptr;
	delete m_Key2;			m_Key2 = nullptr;
	delete m_Portal;		m_Portal = nullptr;
	delete m_Flag;			m_Flag = nullptr;
	delete m_Platform;		m_Platform = nullptr;
	// - Map/Deadzone's	-	
	delete m_MapDeadzone;	m_MapDeadzone = nullptr;
	delete m_Deadzone;		m_Deadzone = nullptr;
	delete m_Map;			m_Map = nullptr;

	for (int idx = 0; idx < m_Enemy.size(); idx++) {
		delete m_Enemy[idx];
		m_Enemy[idx] = nullptr;
	}
	m_Enemy.clear();

	// - LOADING "GAMESTATE" - 
	delete m_LoadingText;	m_LoadingText = nullptr;
	delete m_LoadingBall;	m_LoadingBall = nullptr;
	// - STARTSCREEN "GAMESTATE" - 
	delete m_StartScreen;			m_StartScreen = nullptr;
	delete m_TribeExplorerText;		m_TribeExplorerText = nullptr;
	delete m_NonPlayableCharacter;	m_NonPlayableCharacter = nullptr;
	delete m_ExitButton;			m_ExitButton = nullptr;
	delete m_PlayButton;			m_PlayButton = nullptr;
	delete m_GameWonTextPart1;		m_GameWonTextPart1 = nullptr;
	delete m_GameWonTextPart2;		m_GameWonTextPart2 = nullptr;
	delete m_Quit;

	if (m_Font) {
		TTF_CloseFont(m_Font);
		m_Font = nullptr;
	}
	if (m_FontThin) {
		TTF_CloseFont(m_FontThin);
		m_FontThin = nullptr;
	}
}

void Game::Update(float elapsedSec)
{
	const float ENEMYHITCOOLDOWN_MAX{ 2.f };
	Rectf leftWorldBorder{ -10.f, 400.f, 10.f, 200.f };
	switch (m_GameState)
	{
	case Game::GameState::Game:

		//World border in the beginning
		if(IsOverlapping(m_Player->GetPlayerHitBox(), leftWorldBorder)) m_Player->CollisionWall(true, true);
		else m_Player->CollisionWall(false, true);

		// - Player -
		m_Player->HandleCollision(elapsedSec, m_Map);
		m_Player->Update(elapsedSec);

		// - Map -
		m_Deadzone->DeadzoneCollisions(elapsedSec, m_MapDeadzone, m_Player->GetPlayerHitBox());

		// - Interactables - 
		m_Door1->DoorCollisions(m_Player->GetPlayerHitBox(), m_HasKey, elapsedSec);
		if (!m_Lifes == 0) m_Key1->KeyCollisions(m_Player->GetPlayerHitBox(), elapsedSec); //Prevent to get key while in dead animation
		if (!m_HasKey) if (m_Key1->HasKey()) m_HasKey = m_Key1->HasKey();
		if (m_HasKey) if (m_Door1->UsedKey()) m_HasKey = !m_Door1->UsedKey();

		m_Door2->DoorCollisions(m_Player->GetPlayerHitBox(), m_HasKey, elapsedSec);
		if (!m_Lifes == 0) m_Key2->KeyCollisions(m_Player->GetPlayerHitBox(), elapsedSec); //Prevent to get key while in dead animation
		if (!m_HasKey) if (m_Key2->HasKey()) m_HasKey = m_Key2->HasKey();
		if (m_HasKey) if (m_Door2->UsedKey()) m_HasKey = !m_Door2->UsedKey();

		//Collisions (for interactables)
		if (!m_Door2->HitDoor()) m_Player->CollisionWall(m_Door1->HitDoor(), false);
		if (!m_Door1->HitDoor()) m_Player->CollisionWall(m_Door2->HitDoor(), false);

		m_Portal->PortalCollisions(m_Player->GetPlayerHitBox(), elapsedSec);
		m_Player->GetWorldSwitch(m_Portal->GravitySwitch());
		if(m_Portal->HitPortal()) m_Player->SetVelocity(m_Portal->VelocityChange());

		m_Flag->FlagCollisions(m_Player->GetPlayerHitBox());

		m_Platform->PlatformCollisions(m_Player->GetPlayerHitBox(), m_Player->GetVelocity());
		m_Player->SetIsGrounded(m_Platform->IsOnPlatform());

		// - Music - 
		if (m_Music->IsPlaying() == false) m_Music->Play(true);

		// - Enemy - 
		for (int idx = 0; idx < m_Enemy.size(); idx++)
		{
			m_Enemy[idx]->WaveMovement(elapsedSec);
			m_Enemy[idx]->EnemyCollisions(elapsedSec, m_Player->GetPlayerHitBox());
			if (!m_Lifes == 0) m_Player->ApplyForce(m_Enemy[idx]->Force());

			//HitAnimation
			m_EnemyHits = m_Enemy[idx]->IsHit();
			m_Player->IsHitAnimation(m_EnemyHits);
			if (m_EnemyHits && EnemyHitCooldown >= ENEMYHITCOOLDOWN_MAX) {
				m_Lifes--;
				EnemyHitCooldown = 0.f;
			}
		}
		if (EnemyHitCooldown < ENEMYHITCOOLDOWN_MAX) EnemyHitCooldown += elapsedSec;

		// - Dead -
		if (m_Deadzone->IsDead()) m_Lifes = 0;

		// - Lives - 
		if (m_Lifes == 0) {
			m_Player->DeadAnimation(true);
			m_Portal->GravitySwitchChange(false);
			AnimationDeadCoolDown += (10.f * elapsedSec);;
			if (AnimationDeadCoolDown >= 15.f) {
				AnimationDeadCoolDown = 0;
				m_Lifes = 2;
				m_Player->DeadAnimation(false);
			}
			else if (AnimationDeadCoolDown >= 14.f) m_Player->SetPosition(Point2f(100.f, 500.f)); //Bringing back to start
			else if (AnimationDeadCoolDown >= 1.2f);
			else if (AnimationDeadCoolDown >= 0.8f) {
				m_Player->SetPosition(Point2f(m_Player->GetPosition().x, m_Player->GetPosition().y)); //To set my velocity to 0
				m_Player->ApplyForce(Vector2f{ 0.f, 200.f }); //Applying the jump animation
			}
			else if (AnimationDeadCoolDown >= 0.5f) m_LastPosition = m_Player->GetCenter();
			else if (AnimationDeadCoolDown >= 0.1f) m_Player->SetVelocity(Vector2f{ 0.f,0.f });
		}
		else m_LastPosition = m_Player->GetCenter();

		if (m_Flag->GameWon() && !m_DoneLoading) m_GameState = GameState::Loading;
		break;
	case Game::GameState::Loading:
		m_LoadingBall->Update(elapsedSec);
		m_Counter++;
		if (m_Counter >= 180 && !m_Flag->GameWon()) { m_GameState = GameState::Game; m_Counter = 0; }
		else if (m_Counter >= 180 && m_Flag->GameWon()) { m_GameState = GameState::StartScreen; m_Counter = 0; m_DoneLoading = true; }
		break;
	case Game::GameState::LevelSelect:
		break;
	case Game::GameState::StartScreen:
		m_NonPlayableCharacter->Update(elapsedSec);
		if (m_PlayButton->IsPressed()) m_GameState = GameState::Loading;
		if (m_ExitButton->IsPressed()) SDL_PushEvent(m_Quit);
		break;
	default:
		break;
	}
}

void Game::Draw() const
{
	ClearBackground();
	const Point2f screenCenter{ GetViewPort().width / 2.f, GetViewPort().height / 2.f };

	switch (m_GameState)
	{
	case Game::GameState::Game:
		{	
			//Camera movement
			const float minX{ 0.f };
			const float maxX{ 8650.f };
			const float minY{ 0.f };
			const float maxY{ 1050.f };
			const float CAMERA_SMOOTH_MOVEMENT{ 500.f };

			Point2f playerCenter;
			if (m_Lifes == 0) {
				playerCenter = m_LastPosition;
			}
			else {
				playerCenter = m_Player->GetCenter();
			}

			Point2f cameraPosition{ playerCenter.x - screenCenter.x, playerCenter.y - screenCenter.y };

			// Apply boundaries
			if (cameraPosition.x < minX)
				cameraPosition.x = minX;
			else if (cameraPosition.x > maxX - CAMERA_SMOOTH_MOVEMENT)
				cameraPosition.x = maxX - CAMERA_SMOOTH_MOVEMENT;

			if (cameraPosition.y < minY)
				cameraPosition.y = minY;
			else if (cameraPosition.y > maxY - CAMERA_SMOOTH_MOVEMENT)
				cameraPosition.y = maxY - CAMERA_SMOOTH_MOVEMENT;

			// Move the camera to focus on the player
			glPushMatrix();
				glTranslatef(-cameraPosition.x, -cameraPosition.y, 0.f);
				m_Door1->Draw();
				m_Door2->Draw();
				m_Map->Draw(true); 
				m_Player->Draw(true); 
				for (int idx = 0; idx < m_Enemy.size(); idx++) { m_Enemy[idx]->Draw(); }
				m_Portal->Draw();
				m_Key1->Draw();
				m_Key2->Draw();
				m_Flag->Draw();
			glPopMatrix();

			break;
		}
	case Game::GameState::Loading:
		{
			const Rectf loadingballRect(screenCenter.x, screenCenter.y , m_LoadingBall->GetSize().x, m_LoadingBall->GetSize().y);
			m_LoadingBall->Draw(loadingballRect);
			m_LoadingText->Draw(Point2f{ screenCenter.x, screenCenter.y + 200 });
		}
		break;
	case Game::GameState::LevelSelect:
		break;
	case Game::GameState::StartScreen:
		{
			const Rectf NPC_rect(screenCenter.x + 250, screenCenter.y - 100, m_NonPlayableCharacter->GetSize().x * 2.5f, m_NonPlayableCharacter->GetSize().y * 2.5f);
			m_StartScreen->Draw();
			m_NonPlayableCharacter->Draw(NPC_rect);
			m_TribeExplorerText->Draw(Point2f{ screenCenter.x - 400, screenCenter.y + 200 });
			
			if (m_Flag->GameWon()) {
				m_GameWonTextPart1->Draw(Point2f{ screenCenter.x - 400, screenCenter.y });
				m_GameWonTextPart2->Draw(Point2f{ screenCenter.x - 400, screenCenter.y - 50 });
			}
			else {
				m_PlayButton->Draw();
				m_ExitButton->Draw();
			}
		break;
		}
	default:
		break;
	}

	
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{

}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	m_Player->ProcessKeyUpEvent(e);
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_PlayButton->ProcessMouseMotionEvent(e);
	m_ExitButton->ProcessMouseMotionEvent(e);
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	m_PlayButton->ProcessMouseDownEvent(e);
	m_ExitButton->ProcessMouseDownEvent(e);
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	m_PlayButton->ProcessMouseUpEvent(e);
	m_ExitButton->ProcessMouseUpEvent(e);
}

void Game::ClearBackground() const
{
	switch (m_GameState)
	{
	case Game::GameState::Game:
		glClearColor(0.06f, 0.01f, 0.08f, 1.0f);
		break;
	case Game::GameState::Loading:
		glClearColor(1.0f, 0.6f, 0.7f, 1.0f);
		break;
	case Game::GameState::LevelSelect:
		break;
	case Game::GameState::StartScreen:
		break;
	default:
		break;
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
}
