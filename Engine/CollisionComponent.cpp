#include "pch.h"
#include "CollisionComponent.h"

#include "TransformComponent.h"
#include "Renderer.h"

CollisionComponent::CollisionComponent(float width, float height, bool trigger)
	: m_pTransformComponent(nullptr)
	, m_pPhysicsSystem(PhysicsSystem::GetInstance())
	, m_CollisionBox(0, 0, width, height)
	, m_IsActive(true)
	, m_Trigger(trigger)
{
}

void CollisionComponent::Initialize()
{
	if (m_pGameObject != nullptr)
		m_pTransformComponent = GetTransform();
	else
		CORE_ERROR("CollisionComponent::Initialize() > Cannot Initialize CollisionComponent, it is not attached to a GameObject");

	m_pPhysicsSystem->AddCollisionComponent(this);

	m_CollisionBox.x = m_pTransformComponent->GetPosition().x;
	m_CollisionBox.y = m_pTransformComponent->GetPosition().y;
}

CollisionComponent::~CollisionComponent()
{
	m_pTransformComponent = nullptr;
}

void CollisionComponent::Update()
{
	m_CollisionBox.x = m_pTransformComponent->GetPosition().x;
	m_CollisionBox.y = m_pTransformComponent->GetPosition().y;
}

void CollisionComponent::Render()
{
	//SDL_SetRenderDrawColor(Renderer::GetInstance()->GetSDLRenderer(), 255, 0, 0, 255);
	//SDL_RenderDrawRect(Renderer::GetInstance()->GetSDLRenderer(), &SDL_Rect({ (int)m_CollisionBox.x, (int)m_CollisionBox.y, (int)m_CollisionBox.w, (int)m_CollisionBox.h }));
}
