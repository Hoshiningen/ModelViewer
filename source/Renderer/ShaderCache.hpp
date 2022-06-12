#pragma once

#include <algorithm>
#include <concepts>
#include <memory>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

class ShaderProgram;

class ShaderCache {
public:
    template<std::default_initializable T>
    bool registerProgram(std::unique_ptr<ShaderProgram> pShader) {

        const auto [iter, inserted] = m_programs.insert(std::move(pShader));
        return m_typeMap.emplace(typeid(T), iter->get()).second;
    }

    template<std::default_initializable T>
    bool registerProgram(ShaderProgram* pShader) {

        const auto iter = std::ranges::find_if(m_programs, [pShader](const std::unique_ptr<ShaderProgram>& pOwnedShader) {
            return pOwnedShader.get() == pShader;
        });

        if (iter == m_programs.cend())
            return false;

        return m_typeMap.emplace(typeid(T), pShader).second;
    }

    template<std::default_initializable T>
    bool registerProgram(const ShaderProgram& shader) {
        return registerProgram(&shader);
    }

    template<typename T>
    ShaderProgram* get() const {

        if (auto iter = m_typeMap.find(typeid(T)); iter != m_typeMap.cend())
            return iter->second;

        return nullptr;
    }

    template<typename T>
    ShaderProgram* get(T&& arg) const {

        if (auto iter = m_typeMap.find(typeid(std::forward<T>(arg))); iter != m_typeMap.cend())
            return iter->second;

        return nullptr;
    }

private:
    std::unordered_map<std::type_index, ShaderProgram*> m_typeMap;
    std::unordered_set<std::unique_ptr<ShaderProgram>> m_programs;
};
