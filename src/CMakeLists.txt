add_executable(bss main.cpp)
target_link_libraries(bss CONAN_PKG::nlohmann_json CONAN_PKG::cxxopts CONAN_PKG::boost)
