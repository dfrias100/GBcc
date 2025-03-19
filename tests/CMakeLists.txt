add_executable(RegisterInstantiationTest RegisterInstantiationTest.cpp)
add_executable(RegisterCopyTest RegisterCopyTest.cpp)
add_executable(RegisterMoveTest RegisterMoveTest.cpp)
add_executable(RegisterSetTest RegisterSetTest.cpp)
add_executable(RegisterFlagsTest RegisterFlagsTest.cpp)

target_include_directories(
    RegisterInstantiationTest PRIVATE
    "../include"
)

target_include_directories(
    RegisterCopyTest PRIVATE
    "../include"
)
target_include_directories(
    RegisterMoveTest PRIVATE
    "../include"
)

target_include_directories(
    RegisterSetTest PRIVATE
    "../include"
)

target_include_directories(
    RegisterFlagsTest PRIVATE
    "../include"
)

target_link_libraries(RegisterInstantiationTest SharpRegister)
target_link_libraries(RegisterCopyTest SharpRegister)
target_link_libraries(RegisterMoveTest SharpRegister)
target_link_libraries(RegisterSetTest SharpRegister)
target_link_libraries(RegisterFlagsTest SharpRegister)

add_test(
    NAME RegisterInstantiationTest
    COMMAND RegisterInstantiationTest
)

add_test(
    NAME RegisterCopyTest
    COMMAND RegisterCopyTest
)

add_test(
    NAME RegisterMoveTest
    COMMAND RegisterMoveTest
)

add_test(
    NAME RegisterSetTest
    COMMAND RegisterSetTest
)

add_test(
    NAME RegisterFlagsTest
    COMMAND RegisterFlagsTest
)
