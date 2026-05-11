cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
project(volfield VERSION 1.0.0)

# Change me to on to enable the editor
SET(WITH_EDITOR ON)

set(
  SOURCE_FILES
  "main.cpp"

  "Engine/ECS/src/Component.cpp"
  "Engine/ECS/src/CollisionComponent.cpp"
  "Engine/ECS/src/ImageComponent.cpp"
  "Engine/ECS/src/SoundComponent.cpp"
  "Engine/ECS/src/AnimationComponent.cpp"
  "Engine/ECS/src/PhysicsComponent.cpp"
  "Engine/ECS/src/InputComponent.cpp"
  "Engine/ECS/src/Commands.cpp"
  "Engine/src/Scene.cpp"
  "Engine/src/Button.cpp"

  "Volfield/Ship.cpp"
  "Volfield/VolfieldScene.cpp"
  "Volfield/MenuScene.cpp"
)

# Add editor files to the list if WITH_EDITOR is ON
if (WITH_EDITOR)
  # list(
  #   APPEND SOURCE_FILES
  #   "Editor/src/Button.cpp"
  #   "Editor/src/Blocks.cpp"
  #   "Editor/src/Actor.cpp"
  #   "Editor/src/ActorTooltip.cpp"
  #   "Editor/src/Level.cpp"
  # )
endif()

add_executable(volfield ${SOURCE_FILES})

target_compile_definitions(
  volfield PUBLIC
)

if (WITH_EDITOR)
  target_compile_definitions(volfield PUBLIC
  # WITH_EDITOR
  CHECK_ERRORS
  DRAW_DEBUG_HELPERS
  ENABLE_CHEATS
  )
endif()

target_include_directories(
  volfield PUBLIC
  ${PROJECT_SOURCE_DIR}
)

set(SDLTTF_VENDORED ON)
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
add_subdirectory(${VENDOR_DIR}/SDL)
add_subdirectory(${VENDOR_DIR}/SDL_image)
add_subdirectory(${VENDOR_DIR}/SDL_ttf)

target_link_libraries(volfield PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
)

set(AssetDirectory "${PROJECT_SOURCE_DIR}/Assets")
add_custom_command(
  TARGET volfield POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SDL3::SDL3>"
    "$<TARGET_FILE:SDL3_image::SDL3_image>"
    "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
    "$<TARGET_FILE_DIR:volfield>"

  COMMAND
  ${CMAKE_COMMAND} -E copy_directory_if_different
    "${AssetDirectory}"
    "$<TARGET_FILE_DIR:volfield>/Assets"
  VERBATIM
)

