cmake_minimum_required(VERSION 3.0)

project(plasma-crisp)

find_package(ECM 1.4.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(CMAKE_CXX_STANDARD 17)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt5 CONFIG REQUIRED COMPONENTS
    Quick
    Core
    )

find_package(KF5 REQUIRED COMPONENTS
    Plasma
    I18n
    FileMetaData
    KIO
    )

add_subdirectory(src)
add_subdirectory(icons)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
