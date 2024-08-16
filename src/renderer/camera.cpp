#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace inx
{
    PerspectiveCamera::PerspectiveCamera(const glm::vec3& position)
        : _position(position), _front(glm::vec3(0.f, 0.f, -1.f)), _up(glm::vec3(0.f, 1.f, 0.f))
    {
        update_vectors();
    }

    glm::mat4 PerspectiveCamera::get_view_matrix()
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

    void PerspectiveCamera::change_position(PerspectiveCamera::DIRECTION direction, float dt)
    {
        const float speed = 10.f * dt;
        switch (direction)
        {
            case DIRECTION::FORWARD:  _position += _front * speed; break;
            case DIRECTION::BACKWARD: _position -= _front * speed; break;
            case DIRECTION::LEFT:     _position -= _right * speed; break;
            case DIRECTION::RIGHT:    _position += _right * speed; break;
        }
    }

    void PerspectiveCamera::change_pov(float dx, float dy)
    {
        dx *= .1f;
        dy *= .1f;

        _yaw += dx;
        _pitch += dy;

        update_vectors();
    }

    void PerspectiveCamera::change_fov(float dy)
    {
        _fov -= dy * 10.f;
    }

    void PerspectiveCamera::update_vectors()
    {
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        new_front.y = sin(glm::radians(_pitch));
        new_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(new_front);

        _right = glm::normalize(glm::cross(_front, _up));
    }
} // namespace inx