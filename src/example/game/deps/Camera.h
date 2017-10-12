#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "InputHandler.h"
#include "Vector2D.h"

class Camera 
{
public:

	glm::vec3 m_position;
	
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;

	glm::vec3 m_worldup;

	float m_yaw;
	float m_pitch;

	float m_zoom;
	float m_moveSpeed;

	bool m_enableInput;

	bool m_mouseFirstMove = true;
	float m_mouseLastX;
	float m_mouseLastY;

	enum class direction { in, out, right, left };

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		float yaw = -90.0f,
		float pitch = 0.0,
		float zoom = 1.0f,
		float moveSpeed = 0.25f) : 
		m_front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		m_enableInput(false)
	{
		m_position = position;

		m_yaw = yaw;
		m_pitch = pitch;

		m_worldup = up;

		m_zoom = zoom;
		m_moveSpeed = moveSpeed;

		updateCameraVectors();
	}

	void lookAt()
	{
	}

	void move(direction d)
	{
		if (d == direction::in)
			m_position += m_moveSpeed * m_front;
		
		if(d == direction::out)
			m_position -= m_moveSpeed * m_front;

		if(d == direction::right)
			m_position += glm::normalize(glm::cross(m_front, m_up)) * m_moveSpeed;

		if(d == direction::left)
			m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_moveSpeed;
	}
	
	void rotate()
	{
	}

	void zoom()
	{
	}

	void scrollInput()
	{
	}

	void keyboardInput()
	{
		// todo: give the user ability to adjust input keys

		if (_inHandler->onKeyDown(SDL_SCANCODE_W)) {
			move(direction::in);
		}
		if (_inHandler->onKeyDown(SDL_SCANCODE_S)) {
			move(direction::out);
		}
		if (_inHandler->onKeyDown(SDL_SCANCODE_D)) {
			move(direction::right);
		}
		if (_inHandler->onKeyDown(SDL_SCANCODE_A)) {
			move(direction::left);
		}
	}

	void dragFPSInput(Vector2D* mousePos, bool constrainPitch = true)
	{

		if (m_mouseFirstMove)
		{
			m_mouseLastX = mousePos->getX();
			m_mouseLastY = mousePos->getY();
			m_mouseFirstMove = false;
		}

		float xoffset = mousePos->getX() - m_mouseLastX;
		float yoffset = m_mouseLastY - mousePos->getY(); // reversed since y-coordinates go from bottom to top

		xoffset *= m_moveSpeed / 50;
		yoffset *= m_moveSpeed / 50;

		m_position = glm::rotateY(m_position, glm::radians(-xoffset));
		m_position = glm::rotateX(m_position, glm::radians(yoffset));

		// m_yaw += xoffset;
		// m_pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	void dragTPSInput(Vector2D* mouseMoveDiff)
	{
		float angleX = 0;
		float angleY = 0;

		float sensitivity = 0.5f;

		m_position = glm::rotateY(m_position, glm::radians(-mouseMoveDiff->getX() * sensitivity));
		m_position = glm::rotateX(m_position, glm::radians(-mouseMoveDiff->getY() * sensitivity));

		// updateCameraVectors();
	}

	void onInput(bool drag = true, bool scroll = true, bool keyboard = false)
	{
		if (drag) {
			if (_inHandler->getMouseButtonState(_inHandler->mouse_buttons::LEFT)) {

				std::cout << _inHandler->getMouseMoveDiff() << endl;

				if(_inHandler->isMouseMovig())
					dragTPSInput(_inHandler->getMouseMoveDiff());
			}
		}

		if (scroll)
			scrollInput();

		if (keyboard)
			keyboardInput();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetFPSVewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	glm::mat4 GetTPSViewMatrix()
	{
		return glm::lookAt(m_position, m_front, m_up);
	}

private:
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		m_front = glm::normalize(front);

		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_right = glm::normalize(glm::cross(m_front, m_worldup)); 
		
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

	InputHandler* _inHandler = TheInputHandler::Instance();
};