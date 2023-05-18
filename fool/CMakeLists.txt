# CMakeList.txt: проект CMake для fool; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(headers)

set(SOURCE_FILE
	fool.cpp
	sources/Utility.cpp
	sources/ServerService.cpp

	sources/Engine/Application.cpp
	sources/Engine/State.cpp
	sources/Engine/StateStack.cpp
	sources/Engine/SceneNode.cpp
	sources/Engine/SpriteNode.cpp
	sources/Engine/ShapeNode.cpp

	sources/States/GameState.cpp
	sources/States/MenuState.cpp
	sources/States/PauseState.cpp
	sources/States/MultiplayerConnectionState.cpp
	sources/States/ConnectionWaitState.cpp
	sources/States/CreatingRoomState.cpp
	sources/States/StatisticState.cpp
	sources/States/SettingsState.cpp
	sources/States/GameEndState.cpp

	sources/Game/AI.cpp
	sources/Game/GameInfo.cpp
	sources/Game/PlayerInfo.cpp
	sources/Game/Animation.cpp
	sources/Game/Card.cpp
	sources/Game/CardGroup.cpp
	sources/Game/CardField.cpp
	sources/Game/CardDeck.cpp
	sources/Game/CardPile.cpp

	sources/GUI/Offset.cpp
	sources/GUI/Component.cpp
	sources/GUI/Container.cpp
	sources/GUI/Button.cpp
	sources/GUI/Label.cpp
	sources/GUI/InputText.cpp
	sources/GUI/List.cpp
	sources/GUI/ListItem.cpp
	sources/GUI/OnlineGameItem.cpp
	
	icon.rc)

add_executable (fool ${SOURCE_FILE})

target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-audio-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-network-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-graphics-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-window-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-system-d.lib)
target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/freeglut.lib)

# target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-audio.lib)
# # target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-network.lib)
# target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib)
# target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib)
# target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib)
# # target_link_libraries(fool ${CMAKE_SOURCE_DIR}/lib/freeglut.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET fool PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.