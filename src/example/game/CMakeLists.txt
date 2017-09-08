include_directories(deps)

SET(DEPS
	deps/Game.cpp
	deps/Game.h
	deps/LoaderParams.h
	deps/TextureManager.cpp
	deps/TextureManager.h
	deps/Vector2D.cpp
	deps/Vector2D.h
	deps/InputHandler.cpp
	deps/InputHandler.h
	deps/GameObject.cpp
	deps/GameObject.h
	deps/SDLGameObject.cpp
	deps/SDLGameObject.h
	deps/AnimatedGraphic.cpp
	deps/AnimatedGraphic.h
	deps/Player.cpp
	deps/Player.h
	deps/Enemy.cpp
	deps/Enemy.h
	deps/MenuButton.cpp
	deps/MenuButton.h
	deps/GameState.h
	deps/GameStateMachine.cpp
	deps/GameStateMachine.h
	deps/BaseCreator.h
	deps/GameObjectFactory.cpp
	deps/GameObjectFactory.h
	deps/MainMenuState.cpp
	deps/MainMenuState.h
	deps/StateParser.cpp
	deps/StateParser.h
	deps/MenuState.h
	deps/PlayState.cpp
	deps/PlayState.h
	deps/PauseState.h
	deps/PauseState.cpp
	deps/GameOverState.h
	deps/GameOverState.cpp
	deps/graphics/Shader.h
)

add_executable (main
	main.cpp
	${DEPS}
)

target_link_libraries(main
	${BLITZ_LIBS}
)

message(${BLITZ_LIBS})

IF(WIN32)
	target_link_libraries(main "${SDL2_MAIN_LIBRARY}")
	target_link_libraries(main "${SDL2_TEST_LIBRARY}")
ENDIF()

add_custom_command(TARGET main PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	  ${CMAKE_CURRENT_SOURCE_DIR}/assets
	  $<TARGET_FILE_DIR:main>/assets)

add_custom_command(TARGET main PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	  ${CMAKE_CURRENT_SOURCE_DIR}/deps/graphics/shaders
	  $<TARGET_FILE_DIR:main>/graphics/shaders)

IF(WIN32)
	# SDL
	add_custom_command(TARGET main PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${SDL2_DLLS}
		$<TARGET_FILE_DIR:main>)

	# SDL_image
	add_custom_command(TARGET main PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${SDL2_IMAGE_DLLS}
		$<TARGET_FILE_DIR:main>)

	# GLEW
	add_custom_command(TARGET main PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${GLEW_DLLS}
		$<TARGET_FILE_DIR:main>)

	# GLFW
	add_custom_command(TARGET main PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${GLFW_DLLS}
		$<TARGET_FILE_DIR:main>)

	# tinyxml2
	add_custom_command(TARGET main PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${TINYXML2_DLLS}
		$<TARGET_FILE_DIR:main>)
ENDIF()
