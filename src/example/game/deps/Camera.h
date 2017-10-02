#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera 
{
public:

	glm::vec3 m_position;
	glm::vec3 m_fornt;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldup;

	float m_yaw;
	float m_pitch;

	float m_zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f), 
		float m_yaw = YAW, float m_pitch = PITCH) : 
		m_fornt(glm::vec3(0.0f, 0.0f, -1.0f)), m_zoom(ZOOM))
	{
		m_position = position;
		m_worldup = up;
		m_yaw = yaw;
		m_pitch = pitch;

		updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}
	
private:
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		Front = glm::normalize(m_front);

		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Right = glm::normalize(glm::cross(m_front, m_worldup)); 
		
		Up = glm::normalize(glm::cross(m_right, m_front));
	}
};