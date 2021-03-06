#pragma once
#include "Singleton.h"
#include <vector>

class RigidbodyComponent;
class CollisionComponent;

struct Rectf
{
	Rectf(float posX = 0, float posY = 0, float width = 32.0f, float height = 32.0f) : x{ posX }, y{ posY }, w{ width }, h{ height } {}

	float x;
	float y;
	float w;
	float h;
};

class PhysicsSystem : public Singleton<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	PhysicsSystem(const PhysicsSystem& other) = delete;
	PhysicsSystem(PhysicsSystem&& other) noexcept = delete;
	PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
	PhysicsSystem& operator=(PhysicsSystem&& other) noexcept = delete;

	void AddCollisionComponent(CollisionComponent* collisionComponent);
	void RemoveCollisionComponent(CollisionComponent* collisionComponent);

	void Update();
	void Render() {}

private:
	bool CheckCollision(const Rectf& rect1, const Rectf& rect2);
	const glm::vec2 CalculatePenVector() const;
	
	std::vector<CollisionComponent*> m_CollisionComponents;

	Rectf m_CurrentMinowski;
	glm::vec2 m_CurrentPenVector;
};

