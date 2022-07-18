set(SOURCES
    Texture.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Texture/Texture/Texture.hpp
)

add_library(Texture ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Texture ALIAS Texture)

# Setup target interface
target_include_directories(Texture PRIVATE .)
target_include_directories(Texture PUBLIC ${PUBLIC_DIR}/Texture)

target_link_libraries(Texture PRIVATE
    ${PROJECT_NAME}::Common
    CONAN_PKG::stb
)

target_link_libraries(Texture PUBLIC
    CONAN_PKG::glad
    CONAN_PKG::glm
)