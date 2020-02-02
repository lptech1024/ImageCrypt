add_executable(crc32_test "")
add_executable(cryptography_test "")
add_executable(fpe_test "")
add_executable(string_collection_test "")

target_sources(crc32_test PRIVATE crc32_test.c)
target_sources(cryptography_test PRIVATE cryptography_test.c)
target_sources(fpe_test PRIVATE fpe_test.c)
target_sources(string_collection_test PRIVATE string_collection_test.c)

target_link_libraries(crc32_test PRIVATE imagecryptlib)
target_link_libraries(cryptography_test PRIVATE imagecryptlib)
target_link_libraries(fpe_test PRIVATE imagecryptlib)
target_link_libraries(string_collection_test PRIVATE imagecryptlib)
