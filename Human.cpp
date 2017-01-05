#include "Human.h"

#include "ModelManager.h"

Human::Human() : PhysicsMob(), m_rotation(), m_origin(0.75f, 2.6f, 0.75f)
{
	setType(EntityType::ENTITY_HUMAN);
	setSize(Vec3(1.5f, 2.9f, 1.5f));
	m_modelHead = ModelManager::get("Resources/Models/Human/head.png");
	m_modelBody = ModelManager::get("Resources/Models/Human/body.png");
	m_modelFoot = ModelManager::get("Resources/Models/Human/foot.png");
	m_modelHand = ModelManager::get("Resources/Models/Human/hand.png");
}

Human::~Human()
{
}

void Human::render()
{
	float anim = SDL_GetTicks() * 0.01f;
	float xrot = sinf(anim) * 45.0f;
	float bob = fabs(sinf(anim));
	if (getControllerType() == EntityControllerType::CONTROLLER_PLAYER) return;
	
	Mat4 bodyMatrix(Transform(getPosition() + getOrigin() + Vec3(0.0f, -1.5f + bob * 0.1f, 0.0f), Vec3(0.0f, getRotation().getY(), 0.0f)));
	
	Mat4 headMatrix(Transform(Vec3(0.0f, 1.7f, 0.0f) + Vec3(-1.0f), Vec3(getRotation().getX(), 0.0f, getRotation().getZ())));
	Shader::getCurrentShader()->setUniform("modelMatrix", headMatrix * bodyMatrix);
	m_modelHead->render();
	
	Mat4 bodyOffset(Transform(Vec3(-0.5f)));
	Shader::getCurrentShader()->setUniform("modelMatrix", bodyOffset * bodyMatrix);
	m_modelBody->render();
	
	Mat4 footMatrix(Transform(Vec3(0.0f, -0.7f, 0.0f)));
	Mat4 footOffset(Transform(Vec3(-0.5)));

	Mat4 leftFootMatrix(Transform(Vec3(-0.5f, 0.0f, xrot / 90.0f), Vec3(xrot, 0.0f, 0.0f)));
	Shader::getCurrentShader()->setUniform("modelMatrix", footOffset * leftFootMatrix * footMatrix * bodyMatrix);
	m_modelFoot->render();
	
	Mat4 rightFootMatrix(Transform(Vec3(0.5f, 0.0f, -xrot / 90.0f), Vec3(-xrot, 0.0f, 0.0f)));
	Shader::getCurrentShader()->setUniform("modelMatrix", footOffset * rightFootMatrix * footMatrix * bodyMatrix);
	m_modelFoot->render();

	Mat4 handOffset(Transform(Vec3(-0.25f)));
	Mat4 leftHandMatrix(Transform(Vec3(-0.9f, 0.0f, -xrot / 90.0f)));
	Shader::getCurrentShader()->setUniform("modelMatrix", handOffset * leftHandMatrix * bodyMatrix);
	m_modelHand->render();

	Mat4 rightHandMatrix(Transform(Vec3(0.9f, 0.0f, xrot / 90.0f)));
	Shader::getCurrentShader()->setUniform("modelMatrix", handOffset * rightHandMatrix * bodyMatrix);
	m_modelHand->render();
}

void Human::addData(DataContainer& container)
{
	PhysicsMob::addData(container);
	container.addVec3(m_rotation);
}

void Human::getData(DataContainer& container)
{
	PhysicsMob::getData(container);
	m_rotation = container.getVec3();
}