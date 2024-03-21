project(LSM9DS1_HANDLER C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(APP_SRC_FILES
    # fsw/src/lsm9ds1_driver.c
    fsw/src/cFS_wrapper.c
)

# Create cFS app module
add_cfe_app(lsm9ds1_driver ${APP_SRC_FILES})

target_include_directories(lsm9ds1_driver PUBLIC fsw/inc)


if (ENABLE_UNIT_TESTS)
    add_subdirectory(unit-test)
endif(ENABLE_UNIT_TESTS)

