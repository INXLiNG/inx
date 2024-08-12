#ifndef __INX_RESOURCES_H__
#define __INX_RESOURCES_H__

#include <memory>           // for std::shared_ptr
#include <typeindex>        // for std::type_index
#include <unordered_map>    // for std::unordered_map

namespace inx
{
    template<typename T, typename U>
    concept takes_resource_params = requires(const U& params) { T(params); };

    /// @brief Superclass for all 'resource' types i.e. types whose constitutive data is loaded from disk to be
    ///        constructed. Used to be able to store all resources collectively in the `ResourceManager`.
    struct Resource
    {
    public:
        // One virtual method is required for type to be polymorphic
        virtual ~Resource() = default;
    };

    /// @brief Manages the loading and lifetime operations for 'resource' types i.e. types whose constitutive data is
    ///        loaded from disk to be constructed. Provides the basic means to load and retrieve resources without
    ///        concern for managing lifetimes.
    struct ResourceManager
    {
    public:
        /// @brief Loads a resource from disk and stores it to the resource manager
        /// @tparam T The resource type to be loaded; must derive from `Resource`
        /// @tparam U Parameter resource for constructing `T`; must be used in construction of `T`, i.e. `T(U)`
        /// @param resource_path The string-identifier to refer to this resource; often but not necessarily a filepath
        /// @param params The set of parameters required for loading a particular resource
        template<typename T, typename U>
        requires std::is_base_of_v<Resource, T> && takes_resource_params<T, U>
        void load_resource(const std::string resource_path, const U& params)
        {
            const auto type_idx = std::type_index(typeid(T));
            _resources[type_idx][resource_path] = std::make_shared<T>(params);
        }

        /// @brief Retrieve a reference to a loaded resource
        /// @tparam T The resource type to be retrieved; must derive from `Resource` 
        /// @param resource_path The string-identifier to refer to this resource; often but not necessarily a filepath
        /// @return A pointer to the resource; nullptr if not currently loaded
        template<typename T>
        requires std::is_base_of_v<Resource, T>
        std::shared_ptr<T> get(const std::string& resource_path) const
        {
            if (const auto outer_it = _resources.find(std::type_index(typeid(T))); outer_it != _resources.end()) 
            { // search outer map to see if we store this resource type
                const auto map = outer_it->second;

                if (const auto inner_it = map.find(resource_path); inner_it != map.end()) 
                { // search inner map to see if we have this specific resource loaded
                    std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(inner_it->second);
                    return result;
                }
            }

            // unable to find resource, return null
            return nullptr;
        }

    private:
        /// @brief Maps resource types to a map of particular resources keyed on their resource path
        std::unordered_map<std::type_index, std::unordered_map<std::string, std::shared_ptr<Resource>>> _resources;
    };
} // namespace inx

#endif // __INX_RESOURCES_H__