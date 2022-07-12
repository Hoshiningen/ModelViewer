from conans import ConanFile, CMake, tools

class ModelViewerRecipe(ConanFile):
    settings = "cppstd", "os", "compiler", "build_type", "arch"
    requires = (
        "glad/0.1.34",
        "glm/0.9.9.8",
        "glfw/3.3.6",
        "nlohmann_json/3.10.5",
        "sigslot/1.2.1",
        "assimp/5.2.2",
        "stb/cci.20200203",
        "imgui/cci.20220621+1.88.docking"
    )
    generators = "cmake_multi"

    def validate(self):
        if not tools.valid_min_cppstd(self, "20"):
            self.output.error("C++20 is required.")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("imgui_impl_glfw.h", src="./res/bindings", dst="../bindings")
        self.copy("imgui_impl_glfw.cpp", src="./res/bindings", dst="../bindings")
        self.copy("imgui_impl_opengl3_loader.h", src="./res/bindings", dst="../bindings")
        self.copy("imgui_impl_opengl3.h", src="./res/bindings", dst="../bindings")
        self.copy("imgui_impl_opengl3.cpp", src="./res/bindings", dst="../bindings")
