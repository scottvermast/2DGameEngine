#include "pch.h"
#include "PhysicsSystem.h"
#include "utils.h"
#include "CollisionComponent.h"
#include "CollisionGroups.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

/// Amazing easy way of implementing basic collisions with axis-alligned bounding boxes
/// https://blog.hamaluik.ca/posts/simple-aabb-collision-using-minkowski-difference/

PhysicsSystem::PhysicsSystem()
	: m_CurrentMinowski()
	, m_CurrentPenVector()
{

}

PhysicsSystem::~PhysicsSystem()
{
	m_CollisionComponents.clear();
}

void PhysicsSystem::AddCollisionComponent(CollisionComponent* collisionComponent)
{
	m_CollisionComponents.push_back(collisionComponent);
}

void PhysicsSystem::RemoveCollisionComponent(CollisionComponent* collisionComponent)
{
	auto it = std::find(m_CollisionComponents.begin(), m_CollisionComponents.end(), collisionComponent);

	m_CollisionComponents.erase(it);
}

void PhysicsSystem::Update()
{
	if (m_CollisionComponents.size() > 1)
	{
		for (size_t i{ 0 }; i != m_CollisionComponents.size() - 1; ++i)
		{
			for (size_t j{ i + 1 }; j < m_CollisionComponents.size(); ++j)
			{
				if (CheckCollision(m_CollisionComponents[i]->GetRect(), m_CollisionComponents[j]->GetRect()))
				{
					GameObject* gameObjectI = m_CollisionComponents[i]->GetGameObject();
					GameObject* gameObjectJ = m_CollisionComponents[j]->GetGameObject();

					if (gameObjectI->CheckDestroy() || gameObjectJ->CheckDestroy())
						continue;

					CollisionGroup flagI = m_CollisionComponents[i]->GetCollisionGroup();
					CollisionGroup flagJ = m_CollisionComponents[j]->GetCollisionGroup();

					CollisionGroup ignoreFlagI = m_CollisionComponents[i]->GetCollisionIgnoreGroups();
					CollisionGroup ignoreFlagJ = m_CollisionComponents[j]->GetCollisionIgnoreGroups();

					bool triggerI = m_CollisionComponents[i]->IsTrigger();
					bool triggerJ = m_CollisionComponents[j]->IsTrigger();

					if (triggerI && triggerJ)
					{
						gameObjectI->m_CollisionCallBack(gameObjectJ, true);
						gameObjectJ->m_CollisionCallBack(gameObjectI, true);
						continue;
					}
					else if (triggerI && !triggerJ)
					{
						gameObjectI->m_CollisionCallBack(gameObjectJ, false);
						gameObjectJ->m_CollisionCallBack(gameObjectI, true);
						continue;
					}
					else if (!triggerI && triggerJ)
					{
						gameObjectI->m_CollisionCallBack(gameObjectJ, true);
						gameObjectJ->m_CollisionCallBack(gameObjectI, false);
						continue;
					}

					if (static_cast<CollisionGroup>(ignoreFlagI & flagJ) == flagJ)
						continue;

					if (static_cast<CollisionGroup>(ignoreFlagJ & flagI) == flagJ)
						continue;

					RigidbodyComponent* rigidbodyI = gameObjectI->GetComponent<RigidbodyComponent>();
					RigidbodyComponent* rigidbodyJ = gameObjectJ->GetComponent<RigidbodyComponent>();

					if (rigidbodyI)
					{
						m_CurrentPenVector = CalculatePenVector();
						if (m_CurrentPenVector.y > 0)
							continue;
						if (m_CurrentPenVector.y < 0)
						{
							if (rigidbodyI->GetVelocity().y < 0)
								continue;
							rigidbodyI->SetVelocityY(0);
						}
						m_CollisionComponents[i]->GetTransform()->Move(m_CurrentPenVector.x, m_CurrentPenVector.y);
						gameObjectI->m_CollisionCallBack(gameObjectJ, false);
						gameObjectJ->m_CollisionCallBack(gameObjectI, false);
					}
					else if (rigidbodyJ)
					{
						m_CurrentPenVector = CalculatePenVector();
						if (m_CurrentPenVector.y < 0)
							continue;
						if (m_CurrentPenVector.y > 0)
						{
							if (rigidbodyJ->GetVelocity().y < 0)
								continue;
							rigidbodyJ->SetVelocityY(0);
						}
						m_CollisionComponents[j]->GetTransform()->Move(-m_CurrentPenVector.x, -m_CurrentPenVector.y);
						gameObjectJ->m_CollisionCallBack(gameObjectI, false);
						gameObjectI->m_CollisionCallBack(gameObjectJ, false);
					}
				}
			}
		}
	}
}

bool PhysicsSystem::CheckCollision(const Rectf& rect1, const Rectf& rect2)
{
	m_CurrentMinowski.x = rect2.x - (rect1.x + rect1.w);
	m_CurrentMinowski.y = rect2.y - (rect1.y + rect1.h);
	m_CurrentMinowski.w = rect2.w + rect1.w;
	m_CurrentMinowski.h = rect2.h + rect1.h;

	if (utils::IsPointInRect({ 0.0f, 0.0f }, { m_CurrentMinowski.x, m_CurrentMinowski.y , m_CurrentMinowski.w , m_CurrentMinowski.h }))
		return true;
	return false;
}

const glm::vec2 PhysicsSystem::CalculatePenVector() const
{
	glm::vec2 max{ m_CurrentMinowski.x + m_CurrentMinowski.w, m_CurrentMinowski.y + m_CurrentMinowski.h };
	glm::vec2 min{ m_CurrentMinowski.x, m_CurrentMinowski.y };

	glm::vec2 origin{ 0, 0, };

	float minDist = abs(origin.x - min.x);
	glm::vec2 boundsPoint{ min.x, origin.y };

	if (abs(max.x - origin.x) < minDist)
	{
		minDist = abs(max.x - origin.x);
		boundsPoint = { max.x, origin.y };
	}
	if (abs(max.y - origin.y) < minDist)
	{
		minDist = abs(max.y - origin.y);
		boundsPoint = { origin.x, max.y };
	}
	if (abs(min.y - origin.y) < minDist)
	{
		minDist = abs(min.y - origin.y);
		boundsPoint = { origin.x, min.y };
	}

	return boundsPoint;
}

