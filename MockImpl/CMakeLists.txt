add_library(NanoHW_MockImpl STATIC
  source/digital_out.cpp
)
add_library(Nano::NanoHW_MockImpl ALIAS NanoHW_MockImpl)
target_link_libraries(NanoHW_MockImpl PUBLIC Nano::NanoHW)

install(TARGETS NanoHW_MockImpl EXPORT NanoTargets)
