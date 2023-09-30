file(GLOB APP_SOURCES *.cpp)

foreach(testsourcefile ${APP_SOURCES})
  get_filename_component(testname ${testsourcefile} NAME_WE)
  add_executable(${testname} ${testsourcefile})
  set_compile_options(${testname})
endforeach(testsourcefile ${APP_SOURCES})