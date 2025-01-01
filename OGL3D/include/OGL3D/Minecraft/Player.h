#pragma once
#include <OGL3D/InputSystem/OInputListener.h>
#include <OGL3D/Minecraft/World.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/OVec4.h>
#include <OGL3D/OPrerequisites.h>


class Player : public OInputListener
{
public:
	Player(World* world);
	~Player();

	void onUpdate();
	void placeCursorBlocks();
	void calculateVelocity();

	// Inherited via OInputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const OPoint& mousePos) override;
	virtual void onLeftMouseUp(const OPoint& deltaMousePos) override;
	virtual void onLeftMouseDown(const OPoint& deltaMousePos) override;

	virtual void onRightMouseUp(const OPoint& deltaMousePos) override;
	virtual void onRightMouseDown(const OPoint& deltaMousePos) override;

	void onFocus();
	void onKillFocus();

	void setWorldPerspective();

private:
	void jump();
	void stepOffset();
	f32 checkDownSpeed(f32 downSpeed);
	f32 checkUpSpeed(f32 upSpeed);
	bool checkFront();
	bool checkBack();
	bool checkLeft();
	bool checkRight();
	

public:
	OMat4 temp;
	OMat4 projection;
	OMat4 worldCam;
	OMat4 _worldCam;

	OVec3 transformPos;

private:
	World* m_world;

	OVec3 m_placeBlock;
	OVec3 m_highlightBlock;

	// Gravity and collider stuff
	bool m_isGrounded = false;
	bool m_isSprinting;
	f32 m_playerWidth = 0.15f;
	f32 m_playerHeight = 1.75f;
	f32 m_boundsTolerance = 0.1f;

	f32 m_walkSpeed = 8.0f;
	f32 m_sprintSpeed = 6.0f;
	f32 m_jumpForce = 5.0f;
	f32 m_gravity = -9.8f;


	f32 m_rotX = 0.0f;
	f32 m_rotY = 0.0f;
	f32 m_forward = 0.0f;
	f32 m_rightward = 0.0f;

	OVec3 m_velocity;
	f32 m_verticalMomentum = 0.0f;
	bool m_jumpRequest;

	f32 m_playerOffset = 0.2f;
	OVec3 m_transformPos;
};

