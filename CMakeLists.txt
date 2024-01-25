cmake_minimum_required(VERSION 3.26)
project(gangsta)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(THIRD_PARTY_DIR third-party)
set(GLFW_DIR ${THIRD_PARTY_DIR}/GLFW)
set(GLM_DIR ${THIRD_PARTY_DIR}/GLM)
set(GLAD_DIR ${THIRD_PARTY_DIR}/GLAD)

add_executable(${PROJECT_NAME} 
	src/main.cpp

	${GLAD_DIR}/src/glad.c
)

add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLM_DIR})

find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
