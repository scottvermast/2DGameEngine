#include "Fries.h"

Fries::Fries()
	: m_pFries(nullptr)
{
	
}

void Fries::Initialize()
{
	m_pFries = new GameObject();

	m_pFries->AddComponent(new TextureComponent("Fries.png"));
	auto* pCollisionComponent = new CollisionComponent(32, 32, false);
	pCollisionComponent->SetCollisionGroup(Group3);
	pCollisionComponent->SetCollisionIgnoreGroups(Group3);
	m_pFries->AddComponent(pCollisionComponent);
	m_pFries->AddComponent(new RigidbodyComponent());

	m_pFries->SetTag("Fries");
}

void Fries::Update() const
{
	
}

void Fries::Render() const
{
	
}

void Fries::SetPosition(const glm::vec2& position)
{
	m_pFries->GetTransform()->Translate(position);
}

const glm::vec2& Fries::GetPosition()
{
	return m_pFries->GetTransform()->GetPosition();
}

GameObject* Fries::GetGameObject() const
{
	return m_pFries;
}