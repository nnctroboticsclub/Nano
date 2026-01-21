add_library(NanoHW_StubImpl STATIC
  source/digital_out.cpp
)
add_library(Nano::NanoHW_StubImpl ALIAS NanoHW_StubImpl)
target_link_libraries(NanoHW_StubImpl PUBLIC Nano::NanoHW)

install(TARGETS NanoHW_StubImpl EXPORT NanoTargets)
