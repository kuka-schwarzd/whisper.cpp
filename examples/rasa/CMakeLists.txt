if (WHISPER_SDL2)
    # rasa
    set(TARGET rasa)
    add_executable(${TARGET} rasa.cpp)

    include(DefaultTargetOptions)

    target_link_libraries(${TARGET} PRIVATE common common-sdl whisper ${CMAKE_THREAD_LIBS_INIT})
endif ()
