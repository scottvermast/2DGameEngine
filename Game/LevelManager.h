#pragma once
#include <glm.hpp>

#include "Singleton.h"
#include "SceneManager.h"
#include <string>

#include "GameTime.h"

class BobblePlayer1;
class BobblePlayer2;
class MaitaPlayer;
class Wall;
class Maita;
class ZenChan;
class Bubble;
class GameObject;
class Fries;
class WaterMelon;

enum class GameMode
{
	Solo = 0,
	Versus = 1,
	Coop = 2
};

class LevelManager : public Singleton<LevelManager>
{
public:
	LevelManager();
	~LevelManager();

	LevelManager(const LevelManager& other) = delete;
	LevelManager(LevelManager&& other) = delete;
	LevelManager& operator=(const LevelManager& other) = delete;
	LevelManager& operator=(LevelManager&& other) = delete;

	void Initialize(int levelNumber, GameMode gameMode);
	void InitializeLevel();
	void Update();
	void Render();

	BobblePlayer1* GetBobblePlayer() { if (m_pBobblePlayer1) return m_pBobblePlayer1; return nullptr; }

	void AddMaita(Maita* pMaita);
	void DestroyMaita(GameObject* pGameObject);

	void AddZenChan(ZenChan* pBubble);
	void DestroyZenChan(GameObject* pGameObject);

	void AddBubble(Bubble* pBubble);
	void DestroyBubble(GameObject* pGameObject);

	void SetMaitaPlayer(MaitaPlayer* pMaitaPlayer);
	void DestroyMaitaPlayer();

	void AddFries(Fries* pFries);
	void DestroyFries(GameObject* pGameObject);
	
	void AddWaterMelon(WaterMelon* pWaterMelon);
	void DestroyWaterMelon(GameObject* pGameObject);

	bool CheckLevel();

private:
	wchar_t GetTile(int x, int y);
	void SetTile(int x, int y, char c);

	void MergeWallColliders();

	void CountHorizontalColliders();
	void CheckVerticalMatch(const glm::vec2& startPosition, int horizontalCount);

	void ClearWalls(const glm::vec2& startPosition, int horizontalCount, int verticalCount);

	void CheckGameMode(GameMode gameMode, std::ifstream& file);
	void CheckLevelNumber(int levelNumber, std::ifstream& file);
	void SearchLevelNumber(const std::string& level, std::ifstream& file);
	void SearchGameMode(const std::string& gameMode, std::ifstream& file);

	GameTime* m_pGameTime;
	SceneManager* m_pSceneManager;
	BobblePlayer1* m_pBobblePlayer1;
	BobblePlayer2* m_pBobblePlayer2;
	MaitaPlayer* m_pMaitaPlayer;
	std::vector<Wall*> m_Walls;
	std::vector<Maita*> m_EnemyMaita;
	std::vector<ZenChan*> m_EnemyZenChan;
	std::vector<Bubble*> m_Bubbles;
	std::vector<Fries*> m_Fries;
	std::vector<WaterMelon*> m_WaterMelon;

	float m_WinTimer;
	float m_WinTime;
	int m_EnemiesInBubbles;

	int m_LevelWidth;
	int m_LevelHeight;

	int m_EnemyCount;

	int m_CurrentLevelNumber;

	bool m_LevelInitialized;

	std::string m_LevelString;
};

