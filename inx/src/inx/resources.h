#ifndef __INX_RESOURCES_H__
#define __INX_RESOURCES_H__

#include <concepts>
#include <filesystem>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <iostream>

#include <glm/glm.hpp>

#include "types.h"

namespace inx
{
    template<typename T, typename... Args>
    concept is_loadable = requires(Args... args) 
    {  
        { T::load(args...) } -> std::same_as<Scope<T>>;
    };

    struct Resource
    {
    public:
        virtual ~Resource() = default;
    };

    struct ResourceManager
    {
    public:
        template<typename T, typename... Args>
        requires std::is_base_of_v<Resource, T> && is_loadable<T, Args...>
        void load_resource(const std::string& resource_id, Args... args)
        {
            const auto type_index = std::type_index(typeid(T));
            _resources[type_index][resource_id] = T::load(args...);
        }

        template<typename T>
        requires std::is_base_of_v<Resource, T>
        [[nodiscard]] T& get_resource(const std::string& resource_id)
        {
            if (!is_loaded<T>(resource_id))
            { // check if resource exists; throw error if not found
                std::cerr << "Resource [" << resource_id << "] not found.\n";
                throw std::runtime_error(std::string("Could not find resource: ") + resource_id);
            }

            // because we check std::is_base_of<T, V>, we know this is a valid conversion
            Resource* resource_ptr = _resources[std::type_index(typeid(T))][resource_id].get();
            T* resource = dynamic_cast<T*>(resource_ptr);

            return *resource;
        }

        template<typename T>
        requires std::is_base_of_v<Resource, T>
        [[nodiscard]] bool is_loaded(const std::string& resource_id) const noexcept
        {
            bool found = false;
            
            if (const auto& outer_it = _resources.find(std::type_index(typeid(T))); outer_it != _resources.end()) 
            { // search outer map to see if we store this resource type
                const auto& resource_map = outer_it->second;

                if (const auto& inner_it = resource_map.find(resource_id); inner_it != resource_map.end()) 
                { // search inner map to see if we have this specific resource loaded
                    found = true;
                }
            }

            return found;
        }

    private:
        std::unordered_map<std::type_index, std::unordered_map<std::string, Scope<Resource>>> _resources;
    };

    struct Shader : public Resource
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;

        virtual void set_int(const std::string& name, int i) const = 0;
        virtual void set_ints(const std::string& name, int* ints, u32 count) const = 0;
        virtual void set_float(const std::string& name, float f) const = 0;
        virtual void set_vec3(const std::string& name, const glm::vec3& vec) const = 0;
        virtual void set_mat4(const std::string& name, const glm::mat4& mat) const = 0;

        static Scope<Shader> load(const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath);
    };

    enum class ImageFormat
    {
        None = 0, R8, RGB8, RGBA8,
    };

    struct TextureSpec
    {
        u32 width = 1;
        u32 height = 1;
        ImageFormat format = ImageFormat::RGB8;
        bool generate_mipmaps = true;
    };
    
    struct Texture : public Resource
    {
    public:
        virtual ~Texture() = default;
        static Scope<Texture> load(const std::filesystem::path& texture_filepath);
        static Scope<Texture> load(const TextureSpec& spec);

        virtual void bind(unsigned int slot = 0) const = 0;

        virtual const TextureSpec& spec() const = 0;

        virtual void data(const void* data, u32 size) = 0; 
    };
} // namespace inx

#endif // __INX_RESOURCES_H__