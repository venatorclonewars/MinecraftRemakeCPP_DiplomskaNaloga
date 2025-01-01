#include <OGL3D/Minecraft/Player.h>
#include <OGL3D/InputSystem/OInputSystem.h>
#include <OGL3D/Minecraft/World.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/OVec4.h>
#include <OGL3D/Window/OWindow.h>
#include <OGL3D/Time/OTime.h>
#include <algorithm>
#include <OGL3D/OPrerequisites.h>


Player::Player(World* world)
{
	m_world = world;

	OInputSystem::get()->addListener(this);
	OInputSystem::get()->showCursor(false);
	_worldCam.setTranslation(OVec3((WORLD_SIZE_IN_CHUNKS * CHUNK_WIDTH) / 2.0f, 15, (WORLD_SIZE_IN_CHUNKS * CHUNK_WIDTH) / 2.0f));
	transformPos = _worldCam.getTranslation();

	projection = {};
	auto displaySize = m_world->display->getInnerSize();
	projection.setPerspectiveFovH(1.2f, ((f32)displaySize.width / (f32)displaySize.height), 0.1f, 10000.0f);
}

Player::~Player()
{
}

void Player::onUpdate()
{
	
	calculateVelocity();

	if (m_jumpRequest)
		jump();

	if (m_isGrounded)
		stepOffset();

	setWorldPerspective();
}

void Player::placeCursorBlocks()
{
	OVec3 rayStart = _worldCam.getTranslation();  // Camera or object position
	OVec3 rayDirection = _worldCam.getZDirection();  // Forward direction
	rayDirection.normalize();

	float step = 0.1f;
	OVec3 lastPos = {};

	while (step < 100.0f) {

		OVec3 pos = rayStart + (rayDirection * step);

		if (m_world->checkForVoxel(pos)) 
		{

			m_highlightBlock = OVec3((int)pos.x, (int)pos.y, (int)pos.z);
			m_placeBlock = lastPos;

			return;

		}

		lastPos = OVec3((int)pos.x, (int)pos.y, (int)pos.z);

		step += 0.001f;

	}
}

void Player::jump()
{

	m_verticalMomentum = m_jumpForce;
	m_isGrounded = false;
	m_jumpRequest = false;

}

void Player::stepOffset()
{
	OVec3 rayDirection = _worldCam.getZDirection();  // Forward direction
	rayDirection.y = 0.0f;
	rayDirection.normalize();

	OVec3 transformDir = transformPos - m_transformPos;  // Movement direction
	transformDir.y = 0.0f;  // Ignore vertical movement
	transformDir.normalize();  // Normalize to get a unit vector


	// Calculate the dot product to see if we are moving forward
	f32 dotProduct = rayDirection.dot(transformDir);

	OVec3 rayStart = _worldCam.getTranslation() - OVec3(0, 1, 0);  
	OVec3 fowLength = ((rayDirection + m_playerWidth) * 0.2f);
	OVec3 pos = rayStart + fowLength;

	if (dotProduct <= 0.0f || 
		m_world->checkForVoxel(pos + OVec3(0, 1, 0)) ||
		m_world->checkForVoxel(pos + OVec3(0, 2, 0)) ||
		m_world->checkForVoxel(transformPos + OVec3(0, 1, 0)) ||
		!m_isGrounded) 
		
		return;

	
	if (m_world->checkForVoxel(pos)) 
	{
		transformPos = transformPos + OVec3(fowLength.x * 0.5f, 1.01f, fowLength.z * 0.5f);
	}
}


void Player::calculateVelocity()
{
	// Affect vertical momentum with gravity.
	if (m_verticalMomentum > m_gravity)
		m_verticalMomentum += OTime::get()->deltaTime * m_gravity;

	m_velocity = ((_worldCam.getZDirection() * m_forward * m_walkSpeed) + ((_worldCam.getXDirection() * m_rightward * m_walkSpeed))) * OTime::get()->deltaTime;
	m_velocity.y = 0.0f;

	// Apply vertical momentum (falling/jumping).
	m_velocity = m_velocity + OVec3(0, 1, 0) * m_verticalMomentum * OTime::get()->deltaTime * 2.5f;


	if ((m_velocity.z > 0 && checkFront()) || (m_velocity.z < 0 && checkBack()))
		m_velocity.z = 0;
	if ((m_velocity.x > 0 && checkRight()) || (m_velocity.x < 0 && checkLeft()))
		m_velocity.x = 0;


	if (m_velocity.y < 0)
		m_velocity.y = checkDownSpeed(m_velocity.y);

	else if (m_velocity.y > 0)
		m_velocity.y = checkUpSpeed(m_velocity.y);

	m_transformPos = transformPos;
	transformPos = transformPos + m_velocity;

}

void Player::setWorldPerspective()
{

	const float minPitch = -M_PI / 2.0f;  // -90 degrees in radians
	const float maxPitch = M_PI / 2.0f;   // 90 degrees in radians
	m_rotX = std::clamp(m_rotX, minPitch, maxPitch);
	
	temp = {};
	worldCam = {};

	temp.setIdentity();
	temp.setRotationX(m_rotX);
	worldCam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rotY);
	worldCam *= temp;

	worldCam.setTranslation(transformPos);

	_worldCam = worldCam;
	
	worldCam.inverse();
}

f32 Player::checkDownSpeed(f32 downSpeed)
{
	if (m_world->checkForVoxel(OVec3(transformPos.x - m_playerWidth + m_playerOffset, transformPos.y - m_playerHeight, transformPos.z - m_playerWidth + m_playerOffset)) ||
		m_world->checkForVoxel(OVec3(transformPos.x + m_playerWidth - m_playerOffset, transformPos.y - m_playerHeight, transformPos.z - m_playerWidth + m_playerOffset)) ||
		m_world->checkForVoxel(OVec3(transformPos.x + m_playerWidth - m_playerOffset, transformPos.y - m_playerHeight, transformPos.z + m_playerWidth - m_playerOffset)) ||
		m_world->checkForVoxel(OVec3(transformPos.x - m_playerWidth + m_playerOffset, transformPos.y - m_playerHeight, transformPos.z + m_playerWidth - m_playerOffset)))
	{
		m_verticalMomentum = 0.0f;
		m_isGrounded = true;
		return 0;

	}
	else 
	{
		m_isGrounded = false;
		return downSpeed;
	}
}

f32 Player::checkUpSpeed(f32 upSpeed)
{
	if (m_world->checkForVoxel(OVec3(transformPos.x - m_playerWidth + m_playerOffset, transformPos.y + upSpeed, transformPos.z - m_playerWidth + m_playerOffset)) ||
		m_world->checkForVoxel(OVec3(transformPos.x + m_playerWidth - m_playerOffset, transformPos.y + upSpeed, transformPos.z - m_playerWidth + m_playerOffset)) ||
		m_world->checkForVoxel(OVec3(transformPos.x + m_playerWidth - m_playerOffset, transformPos.y + upSpeed, transformPos.z + m_playerWidth - m_playerOffset)) ||
		m_world->checkForVoxel(OVec3(transformPos.x - m_playerWidth + m_playerOffset, transformPos.y + upSpeed, transformPos.z + m_playerWidth - m_playerOffset)))
	{
		m_verticalMomentum = 0.0f;
		return 0;
	}
	else 
		return upSpeed;	
}

bool Player::checkFront() {
	return m_world->checkForVoxel(OVec3(transformPos.x, transformPos.y, transformPos.z + m_playerWidth)) ||
		m_world->checkForVoxel(OVec3(transformPos.x, transformPos.y - 1.0f, transformPos.z + m_playerWidth));
}

bool Player::checkBack() {
	return m_world->checkForVoxel(OVec3(transformPos.x, transformPos.y, transformPos.z - m_playerWidth)) ||
		m_world->checkForVoxel(OVec3(transformPos.x, transformPos.y - 1.0f, transformPos.z - m_playerWidth));
}

bool Player::checkLeft() {
	return m_world->checkForVoxel(OVec3(transformPos.x - m_playerWidth, transformPos.y, transformPos.z)) ||
		m_world->checkForVoxel(OVec3(transformPos.x - m_playerWidth, transformPos.y - 1.0f, transformPos.z));
}

bool Player::checkRight() {
	return m_world->checkForVoxel(OVec3(transformPos.x + m_playerWidth, transformPos.y, transformPos.z)) ||
		m_world->checkForVoxel(OVec3(transformPos.x + m_playerWidth, transformPos.y - 1.0f, transformPos.z));
}


void Player::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}

	if (key == 'S')
	{
		m_forward = -1.0f;
	}

	if (key == 'A')
	{
		m_rightward = -1.0f;
	}

	if (key == 'D')
	{
		m_rightward = 1.0f;
	}

	if (key == ' ' && m_isGrounded)
	{
		m_jumpRequest = true;
	}
}

void Player::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void Player::onMouseMove(const OPoint& mousePos)
{
	auto displaySize = m_world->display->getInnerSize();

	m_rotX += (mousePos.m_y - (displaySize.height / 2.0f)) * OTime::get()->deltaTime * 0.8f;
	m_rotY += (mousePos.m_x - (displaySize.width / 2.0f)) * OTime::get()->deltaTime * 0.8f;


	OInputSystem::get()->setCursorPosition(OPoint(displaySize.width / 2.0f, displaySize.height / 2.0f));
}

void Player::onLeftMouseUp(const OPoint& deltaMousePos)
{

}

void Player::onLeftMouseDown(const OPoint& deltaMousePos)
{
	placeCursorBlocks();
	m_world->getChunkFromVector3(m_highlightBlock)->editChunk(m_highlightBlock, 0);
}

void Player::onRightMouseUp(const OPoint& deltaMousePos)
{

}

void Player::onRightMouseDown(const OPoint& deltaMousePos)
{
	placeCursorBlocks();

	if (
		m_placeBlock != OVec3((int)transformPos.x, (int)transformPos.y, (int)transformPos.z) &&

		m_placeBlock != OVec3((int)(transformPos.x - m_playerWidth), (int)transformPos.y, (int)transformPos.z) &&
		m_placeBlock != OVec3((int)(transformPos.x + m_playerWidth), (int)transformPos.y, (int)transformPos.z) &&

		m_placeBlock != OVec3((int)transformPos.x, (int)transformPos.y, (int)(transformPos.z + m_playerWidth)) &&
		m_placeBlock != OVec3((int)transformPos.x, (int)transformPos.y, (int)(transformPos.z - m_playerWidth)) &&

		m_placeBlock != OVec3((int)(transformPos.x - m_playerWidth), (int)transformPos.y, (int)(transformPos.z - m_playerWidth)) &&
		m_placeBlock != OVec3((int)(transformPos.x + m_playerWidth), (int)transformPos.y, (int)(transformPos.z + m_playerWidth)) &&

		m_placeBlock != OVec3((int)(transformPos.x + m_playerWidth), (int)transformPos.y, (int)(transformPos.z - m_playerWidth)) &&
		m_placeBlock != OVec3((int)(transformPos.x - m_playerWidth), (int)transformPos.y, (int)(transformPos.z + m_playerWidth)) &&

		// y - 1
		m_placeBlock != OVec3((int)transformPos.x, (int)(transformPos.y - 1), (int)transformPos.z) &&

		m_placeBlock != OVec3((int)(transformPos.x - m_playerWidth), (int)(transformPos.y - 1), (int)transformPos.z) &&
		m_placeBlock != OVec3((int)(transformPos.x + m_playerWidth), (int)(transformPos.y - 1), (int)transformPos.z) &&

		m_placeBlock != OVec3((int)transformPos.x, (int)(transformPos.y - 1), (int)(transformPos.z + m_playerWidth)) &&
		m_placeBlock != OVec3((int)transformPos.x, (int)(transformPos.y - 1), (int)(transformPos.z - m_playerWidth)) &&

		m_placeBlock != OVec3((int)(transformPos.x - m_playerWidth), (int)(transformPos.y - 1), (int)(transformPos.z - m_playerWidth)) &&
		m_placeBlock != OVec3((int)(transformPos.x + m_playerWidth), (int)(transformPos.y - 1), (int)(transformPos.z + m_playerWidth)) &&
		m_placeBlock != OVec3((int)(transformPos.x + m_playerWidth), (int)(transformPos.y - 1), (int)(transformPos.z - m_playerWidth)) &&
		m_placeBlock != OVec3((int)(transformPos.x - m_playerWidth), (int)(transformPos.y - 1), (int)(transformPos.z + m_playerWidth))
	)
			m_world->getChunkFromVector3(m_placeBlock)->editChunk(m_placeBlock, 3);
}

void Player::onFocus()
{
}

void Player::onKillFocus()
{
}