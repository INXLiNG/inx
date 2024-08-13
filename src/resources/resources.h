#ifndef __INX_RESOURCES_H__
#define __INX_RESOURCES_H__

#include <memory>           // for std::shared_ptr
#include <typeindex>        // for std::type_index
#include <unordered_map>    // for std::unordered_map
#include <optional>         // for std::optional

namespace inx
{
    /// @brief Checks whether `T` accepts `U` as a parameter in construction.
    template<typename T, typename U>
    concept takes_resource_params = requires(const U& params) { T(params); };

    /// @brief Base class for all resource types (i.e. loaded from disk at 
    ///        runtime) to enable runtime polymorphism.
    struct IResource
    {
    public:
        // One virtual method is required for type to be polymorphic.
        virtual ~IResource() = default;
    };

    /// @brief Manages the loading and lifetime management of 'resource' types
    ///        (i.e. that which is loaded at runtime). All resources are stored
    ///        in a nested map structure of `map<type, map<string_id, resource>>`
    ///        to allow better tracking of derived resource types.
    struct ResourceManager
    {
    public:
        /// @brief Load a resource from disk.
        /// @tparam T The resource type; must derived from `IResource`.
        /// @tparam U Parameters to pass into construction of `T`; `T(U)` must be valid.
        /// @param resource_identifier String identifier for a resource.
        /// @param params Parameters to be passed for resource construction.
        template<typename T, typename U>
        requires std::is_base_of_v<IResource, T> && takes_resource_params<T, U>
        void load_resource(const std::string resource_identifier, const U& params)
        {
            const auto type_idx = std::type_index(typeid(T));
            _resources[type_idx][resource_identifier] = std::make_unique<T>(params);
        }

        /// @brief Retrieve a reference to a loaded resource
        /// @tparam T The resource type; must derived from `IResource`.
        /// @param resource_identifier String identifier for a resource.
        /// @return A reference to the resource, if it could be found.
        template<typename T>
        requires std::is_base_of_v<IResource, T>
        [[nodiscard]] T& get(const std::string& resource_identifier)
        {
            if (!is_loaded<T>(resource_identifier))
            { // check if resource exists; throw error if not found
                throw std::runtime_error(std::string("Could not find resource: ") + resource_identifier);
            }

            IResource* resource_ptr = _resources[std::type_index(typeid(T))][resource_identifier].get();
            T* resource = dynamic_cast<T*>(resource_ptr);

            return *resource;
        }

        /// @brief Check if a resource is currently loaded.
        /// @param resource_identifier String identifier for a resource.
        /// @return true if the resource is loaded; false otherwise.
        template<typename T>
        requires std::is_base_of_v<IResource, T>
        [[nodiscard]] bool is_loaded(const std::string& resource_identifier) const noexcept
        {
            bool result = false;
            
            if (const auto& outer_it = _resources.find(std::type_index(typeid(T))); outer_it != _resources.end()) 
            { // search outer map to see if we store this resource type
                const auto& map = outer_it->second;

                if (const auto& inner_it = map.find(resource_identifier); inner_it != map.end()) 
                { // search inner map to see if we have this specific resource loaded
                    result = true;
                }
            }

            return result;
        }

    private:
        /// @brief Maps resource types to a map of particular resources keyed on their resource path
        std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<IResource>>> _resources;
    };
} // namespace inx

#endif // __INX_RESOURCES_H__