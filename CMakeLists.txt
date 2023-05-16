#Sets minimum cmake version 
cmake_minimum_required(VERSION 3.14)

#Sets project name to CS1950UEngine
project(CS1950UEngine)

find_package(OpenGL REQUIRED)

#Tells compiler to use c++ 20
set(CMAKE_CXX_STANDARD 20)
#Makes c++ 20 required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Setting path macros
set(GLFW_SOURCE_DIR "External/glfw-3.3.8")
set(GLEW_SOURCE_DIR "External/glew")
set(GLM_SOURCE_DIR "External/glm-master")
set(STB_SOURCE_DIR "External/stb")
set(FREETYPE_SOURCE_DIR "External/freetype")

#GLFW settings
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

#freetype settings
set(FT_DISABLE_ZLIB TRUE)
set(FT_DISABLE_BZIP2 TRUE)
set(FT_DISABLE_PNG TRUE)
set(FT_DISABLE_HARFBUZZ TRUE)
set(FT_DISABLE_BROTLI TRUE)


add_subdirectory(${GLFW_SOURCE_DIR})
add_subdirectory(${GLM_SOURCE_DIR})
add_subdirectory(${FREETYPE_SOURCE_DIR})

add_library(StaticGLEW STATIC ${GLEW_SOURCE_DIR}/src/glew.c )

include_directories(${GLFW_SOURCE_DIR}/include
                    ${GLFW_SOURCE_DIR}/deps
                    ${GLEW_SOURCE_DIR}/include
                    ${STB_SOURCE_DIR}
                    ${FREETYPE_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}
                    )

add_executable(${PROJECT_NAME}
    main.cpp

    Engine/core.h
    Engine/core.cpp
    Engine/window.h
    Engine/window.cpp

    Engine/application.cpp
    Engine/application.h
    Engine/screen.cpp
    Engine/screen.h

    Graphics/graphics.h
    Graphics/graphics.cpp
    Graphics/global.h
    Graphics/debug.h
    Graphics/camera.h
    Graphics/camera.cpp
    Graphics/shaderloader.h
    Graphics/material.h
    Graphics/material.cpp
    Graphics/shapedata.h
    Graphics/shape.h
    Graphics/shape.cpp
    Graphics/modeltransform.h
    Graphics/modeltransform.cpp
    Graphics/light.h
    Graphics/light.cpp
    Graphics/font.h
    Graphics/font.cpp
    Graphics/uirenderer.h
    Graphics/uirenderer.cpp
    Graphics/GLWrappers/shader.h
    Graphics/GLWrappers/shader.cpp
    Graphics/GLWrappers/texture.h
    Graphics/GLWrappers/texture.cpp
    Graphics/GLWrappers/vbo.h
    Graphics/GLWrappers/vbo.cpp
    Graphics/GLWrappers/vao.h
    Graphics/GLWrappers/vao.cpp
    Graphics/tiny_obj_loader.h

    Engine/random.h
    Engine/random.cpp

    Engine/Game/gameSystems/particleSystem.h
    Engine/Game/gameSystems/particleSystem.cpp

    Engine/Mesh/vertex.h
    Engine/Mesh/vertex.cpp
    Engine/Mesh/triangle.h
    Engine/Mesh/triangle.cpp

    Engine/UIKit/uiElement.h
    Engine/UIKit/uiElement.cpp
    Engine/UIKit/button.h
    Engine/UIKit/button.cpp
    Engine/UIKit/label.h
    Engine/UIKit/label.cpp
    Engine/UIKit/text.h
    Engine/UIKit/text.cpp

    Engine/Game/gameComponent.h
    Engine/Game/gameComponent.cpp
    Engine/Game/gameObject.h
    Engine/Game/gameObject.cpp
    Engine/Game/gameSystem.h
    Engine/Game/gameSystem.cpp
    Engine/Game/gameWorld.h
    Engine/Game/gameWorld.cpp

    Engine/Game/gameSystems/drawSystem.h
    Engine/Game/gameSystems/drawSystem.cpp
    Engine/Game/gameSystems/characterControllerSystem.h
    Engine/Game/gameSystems/characterControllerSystem.cpp
    Engine/Game/gameSystems/cameraSystem.h
    Engine/Game/gameSystems/cameraSystem.cpp
    Engine/Game/gameSystems/collisionSystem.h
    Engine/Game/gameSystems/collisionSystem.cpp
    Engine/Game/gameSystems/physicsSystem.h
    Engine/Game/gameSystems/physicsSystem.cpp
    Engine/Game/gameSystems/tickSystem.h
    Engine/Game/gameSystems/tickSystem.cpp

    Engine/Game/components/drawComponent.h
    Engine/Game/components/drawComponent.cpp
    Engine/Game/components/transformComponent.h
    Engine/Game/components/transformComponent.cpp
    Engine/Game/components/characterMoveComponent.h
    Engine/Game/components/characterMoveComponent.cpp
    Engine/Game/components/characterJumpComponent.h
    Engine/Game/components/characterJumpComponent.cpp
    Engine/Game/components/physicsComponent.h
    Engine/Game/components/physicsComponent.cpp
    Engine/Game/components/pathFindingComponent.h
    Engine/Game/components/pathFindingComponent.cpp
    Engine/Game/components/behaviorComponent.h
    Engine/Game/components/behaviorComponent.cpp
    Engine/Game/components/healthComponent.h
    Engine/Game/components/healthComponent.cpp
    Engine/Game/components/collisionResponseComponent.h
    Engine/Game/components/collisionResponseComponent.cpp
    Engine/Game/components/collisionComponents/collisionComponent.h
    Engine/Game/components/collisionComponents/collisionComponent.cpp
    Engine/Game/components/collisionComponents/cylinderComponent.h
    Engine/Game/components/collisionComponents/cylinderComponent.cpp
    Engine/Game/components/collisionComponents/environmentComponent.h
    Engine/Game/components/collisionComponents/environmentComponent.cpp

    Engine/Game/collision/ray.h
    Engine/Game/collision/collisioninfo.h
    Engine/Game/collision/collisioninfo.cpp
    Engine/Game/collision/aabb.h
    Engine/Game/collision/aabb.cpp
    Engine/Game/collision/bvh.h
    Engine/Game/collision/bvh.cpp
    Engine/Game/collision/bvhnode.h
    Engine/Game/collision/bvhnode.cpp
    Engine/Game/collision/hierarchicalGrid.h
    Engine/Game/collision/hierarchicalGrid.cpp
    Engine/Game/collision/gridnode.h
    Engine/Game/collision/gridnode.cpp

    Engine/Game/ai/navmesh/navmesh.h
    Engine/Game/ai/navmesh/navmesh.cpp
    Engine/Game/ai/navmesh/navmeshnode.h
    Engine/Game/ai/navmesh/navmeshnode.cpp
    Engine/Game/ai/navmesh/navmeshedge.h
    Engine/Game/ai/navmesh/navmeshedge.cpp

    Engine/Game/ai/behaviortree/btnode.h
    Engine/Game/ai/behaviortree/btnode.cpp
    Engine/Game/ai/behaviortree/composite.h
    Engine/Game/ai/behaviortree/composite.cpp
    Engine/Game/ai/behaviortree/action.h
    Engine/Game/ai/behaviortree/action.cpp
    Engine/Game/ai/behaviortree/condition.h
    Engine/Game/ai/behaviortree/condition.cpp
    Engine/Game/ai/behaviortree/selector.h
    Engine/Game/ai/behaviortree/selector.cpp
    Engine/Game/ai/behaviortree/sequence.h
    Engine/Game/ai/behaviortree/sequence.cpp

    Engine/Game/map/map.h
    Engine/Game/map/map.cpp
    Engine/Game/map/mapnode.h
    Engine/Game/map/mapnode.cpp

    Game/app.h
    Game/app.cpp
    Game/menuScreen.h
    Game/menuScreen.cpp
    Game/gameScreen.h
    Game/gameScreen.cpp

    Game/components.h

    Game/prefabs/character.h
    Game/prefabs/environment.h
    Game/prefabs/goal.h
    Game/prefabs/enemy.h
    Game/prefabs/gamehandler.h
    Game/prefabs/projectile.h
    Game/prefabs/rooms.h
    Game/prefabs/spike.h
    Game/prefabs/bullet.h
    Game/prefabs/treasure.h
    Game/prefabs/lights.h

    Game/ai/scopeCondition.h
    Game/ai/scopeCondition.cpp
    Game/ai/trackAction.h
    Game/ai/trackAction.cpp
    Game/ai/shootAction.h
    Game/ai/shootAction.cpp
    Game/ai/barrierCondition.h
    Game/ai/barrierCondition.cpp

    Game/gameComponents/characterCollisionResponse.h
    Game/gameComponents/characterCollisionResponse.cpp
    Game/gameComponents/enemyMovement.h
    Game/gameComponents/enemyMovement.cpp
    Game/gameComponents/gameHandlerComponent.h
    Game/gameComponents/gameHandlerComponent.cpp
    Game/gameComponents/shootComponent.h
    Game/gameComponents/shootComponent.cpp
    Game/gameComponents/movingComponent.h
    Game/gameComponents/movingComponent.cpp
    Game/gameComponents/projectileCollisionResponse.h
    Game/gameComponents/projectileCollisionResponse.cpp
    Game/gameComponents/characterHealth.h
    Game/gameComponents/characterHealth.cpp
    Game/gameComponents/characterShoot.h
    Game/gameComponents/characterShoot.cpp
    Game/gameComponents/bulletCollisionResponse.h
    Game/gameComponents/bulletCollisionResponse.cpp
    Game/gameComponents/enemyCollisionResponse.h
    Game/gameComponents/enemyCollisionResponse.cpp
    Game/gameComponents/enemyHealth.h
    Game/gameComponents/enemyHealth.cpp
    Game/gameComponents/lightComponent.h
    Game/gameComponents/lightComponent.cpp
)

target_link_libraries(${PROJECT_NAME} glfw StaticGLEW glm freetype ${OPENGL_LIBRARIES})

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

if (WIN32)
  add_compile_definitions(GLEW_STATIC)
  target_link_libraries(${PROJECT_NAME}
      opengl32
      glu32
  )
endif()

if (UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME}
      GL
  )
endif()
