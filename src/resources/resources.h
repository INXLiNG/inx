#ifndef __INX_RESOURCES_H__
#define __INX_RESOURCES_H__

#include <memory>                   // for std::unique_ptr
#include <typeindex>                // for std::type_index
#include <unordered_map>            // for std::unordered_map
#include <optional>                 // for std::optional
#include <concepts>                 // for std::same_as

namespace inx
{
    template<typename T, typename... Args>
    concept is_loadable = requires(Args... args) 
    {  
        { T::load(args...) } -> std::same_as<std::unique_ptr<T>>;
    };

    struct IResource
    {
    public:
        virtual ~IResource() = default;
    };

    struct ResourceManager
    {
    public:
        template<typename T, typename... Args>
        requires std::is_base_of_v<IResource, T> && is_loadable<T, Args...>
        void load_resource(const std::string& resource_id, Args... args)
        {
            const auto type_index = std::type_index(typeid(T));
            _resources[type_index][resource_id] = T::load(args...);
        }

        template<typename T>
        requires std::is_base_of_v<IResource, T>
        [[nodiscard]] T& get_resource(const std::string& resource_id)
        {
            if (!is_loaded<T>(resource_id))
            { // check if resource exists; throw error if not found
                std::cerr << "Resource [" << resource_id << "] not found.\n";
                throw std::runtime_error(std::string("Could not find resource: ") + resource_id);
            }

            // because we check std::is_base_of<T, V>, we know this is a valid conversion
            IResource* resource_ptr = _resources[std::type_index(typeid(T))][resource_id].get();
            T* resource = dynamic_cast<T*>(resource_ptr);

            return *resource;
        }

        template<typename T>
        requires std::is_base_of_v<IResource, T>
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
        std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<IResource>>> _resources;
    };
} // namespace inx

#endif // __INX_RESOURCES_H__