from conans import ConanFile, CMake

class ModelViewerRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "glad/0.1.34",
        "glm/0.9.9.8",
        "glfw/3.3.6",
        "nlohmann_json/3.10.5",
        "sigslot/1.2.1",
        "assimp/5.2.2",
        "stb/cci.20200203"
    )
    generators = "cmake_multi"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
