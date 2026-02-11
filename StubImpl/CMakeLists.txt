add_library(NanoHW_StubImpl INTERFACE)
add_library(Nano::NanoHW_StubImpl ALIAS NanoHW_StubImpl)
target_include_directories(NanoHW_StubImpl INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/Nano>
)
target_link_libraries(NanoHW_StubImpl INTERFACE Nano::NanoHW)

install(TARGETS NanoHW_StubImpl EXPORT NanoTargets)
