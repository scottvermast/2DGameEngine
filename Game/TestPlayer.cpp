#include "TestPlayer.h"
#include "PlayerCommands.h"

TestPlayer::TestPlayer()
	: m_pInputManager(InputManager::GetInstance())
	, m_pTestPlayer(nullptr)
{
}

TestPlayer::~TestPlayer()
{
}

void TestPlayer::Initialize()
{
	m_pTestPlayer = new GameObject();
	m_pTestPlayer->AddComponent(new TextureComponent("TestPlayer.png"));
	m_pTestPlayer->AddComponent(new CollisionComponent(32, 32));

	auto* pControllerComponent = new ControllerComponent();
	m_pTestPlayer->AddComponent(pControllerComponent);

	InputAction* pMoveLeft = new InputAction("PlayerMoveLeft", new MoveLeftCommand(pControllerComponent), KEY_LEFT, MOUSE_UNKNOWN, BUTTON_STATE::PRESSED);
	InputAction* pMoveRight= new InputAction("PlayerMoveRight", new MoveRightCommand(pControllerComponent), KEY_RIGHT, MOUSE_UNKNOWN, BUTTON_STATE::PRESSED);

	m_pInputManager->AddInputActions(pMoveLeft);
	m_pInputManager->AddInputActions(pMoveRight);

	m_pTestPlayer->GetTransform()->Translate(0, 416);
}

void TestPlayer::Update()
{

}

void TestPlayer::Render()
{

}

GameObject* TestPlayer::GetGameObject()
{
	return m_pTestPlayer;
}
