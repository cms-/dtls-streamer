set(THREADS_USE_PTHREADS_WIN32 true)
find_package(Threads)

set(libs
    mbedtls
    output_dtls
)

set(targets
    dtls_client
    dtls_server
    ssl_server2
)

if(USE_PKCS11_HELPER_LIBRARY)
    set(libs ${libs} pkcs11-helper)
endif(USE_PKCS11_HELPER_LIBRARY)

if(ENABLE_ZLIB_SUPPORT)
    set(libs ${libs} ${ZLIB_LIBRARIES})
endif(ENABLE_ZLIB_SUPPORT)

add_executable(dtls_client dtls_client.c)
target_link_libraries(dtls_client ${libs})

add_executable(dtls_server dtls_server.c)
target_link_libraries(dtls_server ${libs})

add_executable(ssl_server2 ssl_server2.c)
target_link_libraries(ssl_server2 ${libs})

install(TARGETS ${targets}
        DESTINATION "bin"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
