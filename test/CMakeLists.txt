
aux_source_directory(test tests)
include_directories(${INC})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

foreach(main IN LISTS tests)
    get_filename_component(mainname ${main} NAME_WE)
    add_executable(${mainname} ${main})
    target_link_libraries(${mainname} ${CONAN_LIBS})
endforeach()