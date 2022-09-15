set(SOURCES
    Math.cpp
)

set(INCLUDES
    ${PUBLIC_DIR}/Common/Common/ClassMacros.hpp
    ${PUBLIC_DIR}/Common/Common/Constants.hpp
    ${PUBLIC_DIR}/Common/Common/IRestorable.hpp
    ${PUBLIC_DIR}/Common/Common/Math.hpp
)

add_library(Common ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Common ALIAS Common)

# Setup target interface
target_include_directories(Common PRIVATE .)
target_include_directories(Common PUBLIC ${PUBLIC_DIR}/Common)

target_link_libraries(Common PRIVATE
    ${PROJECT_NAME}::Geometry
)

target_link_libraries(Common PUBLIC
    CONAN_PKG::nlohmann_json
    CONAN_PKG::glm
)
