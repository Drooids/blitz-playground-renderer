#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "InputHandler.h"
#include "math/Matrix.h"
#include "Vector2D.h"

class Camera
{
public:
	glm::vec3 m_eye;

	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_left;

	glm::vec3 m_target;

	float m_yaw;
	float m_pitch;

	bool m_enableInput;

	// Mouse
	float m_mouseMoveSensitivity;
	float m_mouseZoomSensitivity;

	enum class direction { in, out, right, left, vertical, horizontal, x, y, z };

	Camera(glm::vec3 eye = glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float mouseMoveSensitivity = 0.05f,
		float mouseZoomSensitivity = 0.05f) :

		m_forward(glm::vec3(0.0f, 0.0f, -0.1f)),
		m_enableInput(false)

	{
		m_eye = eye;
		m_up = up;

		m_target = target;

		m_mouseMoveSensitivity = mouseMoveSensitivity;
		m_mouseZoomSensitivity = mouseZoomSensitivity;

		resetViewMatrix(eye, target);
	}

	void lookAt()
	{
	}

	void move(direction dir)
	{
	}

	void rotate(direction dir, float degrees, glm::vec3& eye, glm::vec3& up)
	{
		if (dir == direction::vertical) {
			eye = Math::Matrix::rotation(degrees, m_left) * eye;

			m_left = glm::normalize(glm::cross(m_forward, up));
			m_up = glm::normalize(glm::cross(m_left, m_forward));
		}

		if (dir == direction::horizontal) {
			eye = Math::Matrix::rotation(degrees, up) * eye;
		}

		resetViewMatrix(eye, m_target);
	}

	void zoom(direction dir)
	{
		// TODO: set the camera zoom limit

		if (dir == direction::in) {
			m_eye -= m_eye * m_mouseZoomSensitivity;
		}
		if (dir == direction::out) {
			m_eye += m_eye * m_mouseZoomSensitivity;
		}
	}

	void scrollInput()
	{
	}

	void keyboardInput()
	{
		if (_inHandler->onKeyDown(SDL_SCANCODE_W)) {
			zoom(direction::in);
		}
		if (_inHandler->onKeyDown(SDL_SCANCODE_S)) {
			zoom(direction::out);
		}

		if (_inHandler->onKeyDown(SDL_SCANCODE_D)) {
			move(direction::right);
		}
		if (_inHandler->onKeyDown(SDL_SCANCODE_A)) {
			move(direction::left);
		}
	}

	void dragFPSInput(Vector2D* mouseMoveDiff, bool constrainPitch = true)
	{
		// m_yaw += mouseMoveDiff->getX() * 0.5; // offsetx
		m_pitch += -(mouseMoveDiff->getY() * 0.5); // offsety

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
		rotate(direction::vertical, mouseMoveDiff->getY(), m_eye, m_up);
		rotate(direction::horizontal, mouseMoveDiff->getX(), m_eye, m_up);
	}

	void onInput(bool drag = false, bool scroll = false, bool keyboard = false)
	{
		if (drag)
			if (_inHandler->getMouseButtonState(_inHandler->mouse_buttons::LEFT))
				if(_inHandler->isMouseMovig())
					dragTPSInput(_inHandler->getMouseMoveDiff());

		if (scroll)
			scrollInput();

		if (keyboard)
			keyboardInput();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(m_eye, m_forward, m_up);
	}

private:

	void resetViewMatrix(glm::vec3& eye, glm::vec3& target)
	{
		m_forward = eye - target;
		m_forward = glm::normalize(m_forward);

		m_left = glm::cross(m_up, m_forward);
		m_left = glm::normalize(m_left);

		m_up = glm::cross(m_forward, m_left);
		m_up = glm::normalize(m_up);
	}

	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 forward;
		forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		forward.y = sin(glm::radians(m_pitch));
		forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		m_forward = glm::normalize(forward);

		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you
		// look up or down which results in slower movement.
		m_left = glm::normalize(glm::cross(m_forward, m_up));
		m_up = glm::normalize(glm::cross(m_left, m_forward));
	}

	InputHandler* _inHandler = TheInputHandler::Instance();
};
