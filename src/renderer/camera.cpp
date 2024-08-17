#include "../renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace inx
{
    PerspectiveCamera::PerspectiveCamera(const glm::vec3& position)
        : _position(position), _front(glm::vec3(0.f, 0.f, -1.f)), _up(glm::vec3(0.f, 1.f, 0.f))
    {
        update_vectors();
    }

    glm::mat4 PerspectiveCamera::view_matrix()
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

    void PerspectiveCamera::position(Direction direction, float dt)
    {
        const float speed = 30.f * dt;
        switch (direction)
        {
            case Direction::Forward:  _position += _front * speed; break;
            case Direction::Backward: _position -= _front * speed; break;
            case Direction::Left:     _position -= _right * speed; break;
            case Direction::Right:    _position += _right * speed; break;
            case Direction::Up:       _position += _up * speed; break;
            case Direction::Down:     _position -= _up * speed; break;
        }
    }

    void PerspectiveCamera::pov(float dx, float dy)
    {
        dx *= .1f;
        dy *= .1f;

        _yaw += dx;
        _pitch += dy;

        update_vectors();
    }

    void PerspectiveCamera::fov(float dy)
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