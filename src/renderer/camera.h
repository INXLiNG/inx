#ifndef __INX_CAMERA_H__
#define __INX_CAMERA_H__

#include <glm/glm.hpp>

namespace inx
{
    struct PerspectiveCamera
    {
    public:
        enum DIRECTION
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN,
        };

        PerspectiveCamera(const glm::vec3& position);

        glm::mat4 get_view_matrix();

        void change_position(PerspectiveCamera::DIRECTION direction, float dt);
        void change_pov(float dx, float dy);
        void change_fov(float dy);

        float get_fov() const { return _fov; }
        const glm::vec3& get_position() const { return _position; }

    private:
        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _right;
        glm::vec3 _up;

        float _yaw   = -90.f;
        float _pitch = 0.f;
        float _fov   = 45.f;

        void update_vectors();
    };
} // namespace inx

#endif // __INX_CAMERA_H__