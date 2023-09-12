#pragma once

#include <type_traits>
#include <string>

#include <unordered_map>
#include <vector>
#include <spdlog/spdlog.h>

using ExtensionList = std::vector<std::string>;

namespace Engine
{
    namespace Assets
    {

        template <class _t>
        class AssetCache
        {
            std::string name;
            std::unordered_map<std::string, _t*> loaded;

            public:
            AssetCache(){
                const std::string type = typeid(_t).name();
                name = type;

            }

            _t *get(const std::string &path)
            {

                auto iter = loaded.find(path);

                if (iter != loaded.end())
                {
                    return iter->second;
                }

                _t *newAsset = _t::Load(path.c_str());

                if (!newAsset)
                {
                    spdlog::warn("{} Failed to load: {}", name.c_str(), path);
                    return nullptr;
                }

                spdlog::info("{} Loaded: {}", name.c_str(), path);
                loaded[path] = newAsset;

                return newAsset;
            }
        };
    }
}