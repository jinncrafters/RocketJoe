set(CURRENT_TARGET rocketjoe_kernel)

set(${CURRENT_TARGET}_headers
        configuration.hpp
        init_service.hpp
)

set(${CURRENT_TARGET}_sources
        configuration.cpp
        init_service.cpp
        main.cpp
)

add_executable(${CURRENT_TARGET} ${${CURRENT_TARGET}_sources} ${${CURRENT_TARGET}_headers})

target_link_libraries(
        ${CURRENT_TARGET} PRIVATE
        ${CONAN_LIBS}
        rocketjoe::jupyter
        rocketjoe::configuration
        rocketjoe::interactive_python
        rocketjoe::log
        dl
)

configure_file("kernel.json.in" "${CMAKE_BINARY_DIR}/kernels/rocketjoe/kernel.json")

if (DEV_MODE)

endif ()

#install(TARGETS
#        ${CURRENT_TARGET}

#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#)
