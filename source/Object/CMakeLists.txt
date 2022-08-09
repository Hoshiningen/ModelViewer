set(SOURCES
    Mesh.cpp
)
    
set(INCLUDES
    ${PUBLIC_DIR}/Object/Object/Mesh.hpp
)

add_library(Object ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::Object ALIAS Object)

# Setup target interface
target_include_directories(Object PRIVATE .)
target_include_directories(Object PUBLIC ${PUBLIC_DIR}/Object)

target_link_libraries(Object PRIVATE
    ${PROJECT_NAME}::Common
)
  
target_link_libraries(Object PUBLIC
    ${PROJECT_NAME}::Geometry
    ${PROJECT_NAME}::Material
    CONAN_PKG::glm
)
