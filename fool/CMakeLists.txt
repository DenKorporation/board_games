# CMakeList.txt: проект CMake для fool; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(headers)

set(SOURCE_FILE
	fool.cpp
	sources/Card.cpp
	sources/AI.cpp
	sources/Application.cpp
	sources/Utility.cpp
	sources/State.cpp
	sources/StateStack.cpp
	sources/GameStatus.cpp
	sources/GameState.cpp
	sources/MenuState.cpp
	sources/PauseState.cpp
	sources/StatisticState.cpp
	sources/SceneNode.cpp
	sources/SpriteNode.cpp
	sources/CardGroup.cpp
	sources/CardField.cpp
	sources/Component.cpp
	sources/Container.cpp
	sources/Button.cpp
	sources/Label.cpp
	//sources/ShapeNode.cpp
	icon.rc)

add_executable (fool ${SOURCE_FILE})

target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-audio-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-network-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-graphics-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-window-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-system-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/freeglut.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET fool PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.