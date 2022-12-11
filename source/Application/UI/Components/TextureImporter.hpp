#pragma once

#include "UI/Components/IComponent.hpp"
#include "UI/Components/FileExplorer/FileExplorer.hpp"

#include <array>
#include <filesystem>
#include <vector>

#include <sigslot/signal.hpp>

class Texture;

class TextureImporter : public IComponent {
public:
    static constexpr const char* kWindowId = "Texture Importer";

    sigslot::signal<const Texture&> accepted;
    sigslot::signal<> canceled;
    sigslot::signal<> reset;

    TextureImporter();
    virtual ~TextureImporter() = default;

    struct DataModel : public IComponent::DataModel {
        std::filesystem::path workingDirectory = std::filesystem::current_path();
        std::filesystem::path selectedTexturePath;
    };

private:
    virtual void render() override;
    virtual void syncFrom(const IComponent::DataModel* pFrom) override;
    virtual const IComponent::DataModel* dataModel() const override;
    virtual const char* windowId() const override;

    DataModel m_dataModel;

    std::array<char, 256> m_texturePathBuffer = {};
    bool m_flipUVs = false;

    FileExplorer m_fileExplorer;
};
