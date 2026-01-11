add_library(Nano INTERFACE)
add_library(Nano::Nano ALIAS Nano)
target_include_directories(Nano INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/Nano>
)

# Install targets
install(TARGETS Nano
  EXPORT NanoTargets
  INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
  DESTINATION include/Nano
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

add_nano_test(NanoTest_NoMutexLIFO tests/test_no_mutex_lifo.cpp)
add_nano_test(NanoTest_Span tests/test_span.cpp)
add_nano_test(NanoTest_Singleton tests/test_singleton.cpp)
add_nano_test(NanoTest_ManagedList tests/test_managed_list.cpp)
add_nano_test(NanoTest_FixedMap tests/test_fixed_map.cpp)
add_nano_test(NanoTest_LinkedList tests/test_linked_list.cpp)
add_nano_test(NanoTest_Angle tests/test_angle.cpp)
add_nano_test(NanoTest_Result tests/test_result.cpp)
add_nano_test(NanoTest_Vector tests/test_vector.cpp)
