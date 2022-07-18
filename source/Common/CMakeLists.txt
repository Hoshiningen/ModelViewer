set(INCLUDES
    ${PUBLIC_DIR}/Common/Common/ClassMacros.hpp
    ${PUBLIC_DIR}/Common/Common/IRestorable.hpp
    ${PUBLIC_DIR}/Common/Common/SignalMacros.hpp
)

add_library(Common INTERFACE ${INCLUDES})
add_library(${PROJECT_NAME}::Common ALIAS Common)

# Setup target interface
target_include_directories(Common INTERFACE ${PUBLIC_DIR}/Common)

target_link_libraries(Common INTERFACE
    CONAN_PKG::nlohmann_json
    CONAN_PKG::glm
)
