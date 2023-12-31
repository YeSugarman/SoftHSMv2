project(Encrypt_Decrypt)

set(INCLUDE_DIRS 
                 ${PROJECT_SOURCE_DIR}/../../lib/common
                 ${PROJECT_SOURCE_DIR}/../../lib/crypto
                 ${PROJECT_SOURCE_DIR}/../../lib/data_mgr
                 ${PROJECT_SOURCE_DIR}/../../lib/object_store
                 ${PROJECT_SOURCE_DIR}/../../lib/pkcs11
                 ${PROJECT_SOURCE_DIR}/../../lib/session_mgr
                 ${PROJECT_SOURCE_DIR}/../../lib/slot_mgr
                 ${PROJECT_SOURCE_DIR}/../../lib/handle_mgr
                 ${PROJECT_SOURCE_DIR}/../../lib
                 ${PROJECT_SOURCE_DIR}/../../CLI
                 ${CRYPTO_INCLUDES}
                 ${SQLITE3_INCLUDES}
                 )

set(SOURCES main.cpp
Generate_Key.cpp
Encrypt_Decrypt.cpp
Initialization_Finalization.cpp
GlobalInstance.cpp
            )

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../../lib/win32 ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/win32)
endif()

include_directories(${INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SOURCES} "main.cpp" "Generate_Key.cpp" "Encrypt_Decrypt.cpp" "Initialization_Finalization.cpp" "GlobalInstance.h" "Generate_Key.h" "Initialization_Finalization.h" "Encrypt_Decrypt.h" "GlobalInstance.cpp")
target_link_libraries(${PROJECT_NAME} softhsm2-static ${CRYPTO_LIBS} ${SQLITE3_LIBS} ${CMAKE_DL_LIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})
# Include the relative path to your headers
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/lib)


install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(FILES ${PROJECT_NAME}.1
        DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
        )




