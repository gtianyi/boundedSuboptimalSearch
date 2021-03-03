add_executable(RBTree-test RBTree-test.cpp)
target_link_libraries(RBTree-test CONAN_PKG::gtest)

add_executable(PriorityQueue-test PriorityQueue-test.cpp)
target_link_libraries(PriorityQueue-test CONAN_PKG::gtest)
