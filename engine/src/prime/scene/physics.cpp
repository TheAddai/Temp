
#include "pch.h"
#include "physics.h"
#include "components.h"
#include "entity.h"

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

namespace prime {

	inline b2BodyType GetBodyType(BodyType type)
	{
		switch (type)
		{
		case BodyType::Static:    return b2_staticBody; break;
		case BodyType::Dynamic:   return b2_dynamicBody; break;
		case BodyType::Kinematic: return b2_kinematicBody; break;
		}
		return b2_staticBody;
	}

	void PhysicsWorld::SetScene(Ref<Scene>& scene)
	{
		b2World* world = new b2World({ 0.0f, -9.8f });

		auto pEs = scene->m_registry.view<RigidbodyComponent>();
		for (entt::entity entityID : pEs)
		{
			Entity entity = { entityID, scene.get()};
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb = entity.GetComponent<RigidbodyComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = GetBodyType(rb.type);
			bodyDef.position.Set(transform.position.x, transform.position.y);
			bodyDef.angle = transform.rotation;

			b2Body* body = world->CreateBody(&bodyDef);
			body->SetFixedRotation(rb.fixedRotation);
			m_bodies[entity.GetComponent<IDComponent>().guid] = body;

			if (entity.HasComponent<BoxColliderComponent>())
			{
				auto& bc = entity.GetComponent<BoxColliderComponent>();

				b2PolygonShape boxShape;
				f32 width = 0.0f, height = 0.0f;
				if (bc.scaleWithTransform)
				{
					width = bc.size.x * transform.scale.x;
					height = bc.size.y * transform.scale.y;
				}
				else
				{
					width = bc.size.x;
					height = bc.size.y;
				}

				boxShape.SetAsBox(width / 2.0f, height / 2.0f, { bc.offset.x, bc.offset.y }, 0);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = rb.density;
				fixtureDef.friction = rb.friction;
				fixtureDef.restitution = rb.restitution;
				fixtureDef.restitutionThreshold = rb.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
		m_world = world;
		m_scene = scene;
	}

	void PhysicsWorld::Destroy()
	{
		delete (b2World*)m_world;
		m_world = nullptr;
	}

	void PhysicsWorld::Step(f32 deltaTime)
	{
		const i32 velocityIterations = 6;
		const i32 positionIterations = 2;

		b2World* world = (b2World*)m_world;
		world->Step(deltaTime, velocityIterations, positionIterations);

		auto pEs = m_scene->m_registry.view<RigidbodyComponent>();
		for (entt::entity entityID : pEs)
		{
			Entity entity = { entityID, m_scene.get()};
			auto& transform = entity.GetComponent<TransformComponent>();
			b2Body* body = (b2Body*)m_bodies[entity.GetComponent<IDComponent>().guid];

			const auto& position = body->GetPosition();
			transform.position.x = position.x;
			transform.position.y = position.y;
			transform.rotation = body->GetAngle();
		}
	}
}