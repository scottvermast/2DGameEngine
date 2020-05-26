#include "BobblePlayer.h"

#include "BobbleIdle.h"
#include "BobbleJump.h"
#include "BobbleWalking.h"
#include "PlayerCommands.h"

BobblePlayer::BobblePlayer()
	: m_pInputManager(InputManager::GetInstance())
	, m_pBobblePlayer(nullptr)
	, m_pSpriteComponent(nullptr)
	, m_pState()
{
	m_pStateMap.insert(std::make_pair<std::string, BobbleState*>("idle", new BobbleIdle(this)));
	m_pStateMap.insert(std::make_pair<std::string, BobbleState*>("walking", new BobbleWalking(this)));
	m_pStateMap.insert(std::make_pair<std::string, BobbleState*>("jumping", new BobbleJump(this)));

	m_pState = m_pStateMap.at("idle");
}

BobblePlayer::~BobblePlayer()
{
	for (auto state : m_pStateMap)
	{
		delete state.second;
	}
	m_pStateMap.clear();
}

void BobblePlayer::Initialize()
{
	m_pBobblePlayer = new GameObject();
	m_pBobblePlayer->AddComponent(new CollisionComponent(32, 32));
	auto* pControllerComponent = new ControllerComponent();
	m_pBobblePlayer->AddComponent(pControllerComponent);

	// ---------------------------------------------------------------------------------------------------- //

	m_pSpriteComponent = new SpriteComponent("Player.png", 4, 2, 32);
	m_pSpriteComponent->AddClip(2, true);
	m_pSpriteComponent->AddClip(2, true);
	m_pSpriteComponent->AddClip(2, false);
	m_pSpriteComponent->AddClip(2, false);
	m_pSpriteComponent->SetClipIndex(0);

	m_pBobblePlayer->AddComponent(m_pSpriteComponent);

	// ---------------------------------------------------------------------------------------------------- //

	InputAction* pMoveLeft = new InputAction("PlayerMoveLeft", new MoveLeftCommand(pControllerComponent, this), KEY_LEFT, MOUSE_UNKNOWN, BUTTON_STATE::PRESSED);
	InputAction* pMoveRight = new InputAction("PlayerMoveRight", new MoveRightCommand(pControllerComponent, this), KEY_RIGHT, MOUSE_UNKNOWN, BUTTON_STATE::PRESSED);
	InputAction* pStopMoveLeft = new InputAction("PlayerStopMoveLeft", new StopMoveLeftCommand(pControllerComponent, this), KEY_LEFT, MOUSE_UNKNOWN, BUTTON_STATE::RELEASED);
	InputAction* pStopMoveRight = new InputAction("PlayerStopMoveRight", new StopMoveRightCommand(pControllerComponent, this), KEY_RIGHT, MOUSE_UNKNOWN, BUTTON_STATE::RELEASED);
	InputAction* pJump = new InputAction("PlayerJump", new JumpCommand(pControllerComponent, this), KEY_SPACE, MOUSE_UNKNOWN, BUTTON_STATE::PRESSED);

	m_pInputManager->AddInputActions(pMoveLeft);
	m_pInputManager->AddInputActions(pMoveRight);
	m_pInputManager->AddInputActions(pJump);
	m_pInputManager->AddInputActions(pStopMoveLeft);
	m_pInputManager->AddInputActions(pStopMoveRight);

	m_pBobblePlayer->SetTag("BobblePlayer");
}

void BobblePlayer::SetPosition(const glm::vec2& position)
{
	m_pBobblePlayer->GetTransform()->Translate(position.x, position.y);
}

void BobblePlayer::SetAnimationClip(int index) const
{
	m_pSpriteComponent->SetClipIndex(index);
}

void BobblePlayer::ChangeState(const std::string& newState)
{
	if (m_pState == m_pStateMap[newState])
		return;

	m_pState->OnExit();
	m_pState = m_pStateMap.at(newState);
	m_pState->OnEnter();
}

void BobblePlayer::SetFlipState(const SDL_RendererFlip& flip) const
{
	m_pSpriteComponent->SetFlip(flip);
}

void BobblePlayer::Update() const
{
	m_pState->Update();
}

void BobblePlayer::Render() const
{

}

GameObject* BobblePlayer::GetGameObject() const
{
	return m_pBobblePlayer;
}

