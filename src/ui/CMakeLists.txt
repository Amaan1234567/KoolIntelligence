# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(KoolIntelligence VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)



find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)
include_directories(${CMAKE_SOURCE_DIR}/src/model_backend/)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Test
    Gui
    QuickControls2
    Widgets
)

find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    I18n
    CoreAddons
    QQC2DesktopStyle
    IconThemes
)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

add_executable(koolintelligence)
target_compile_options(koolintelligence PRIVATE -fexceptions)
target_link_libraries(koolintelligence PRIVATE model_backend)

ecm_add_qml_module(koolintelligence
    URI
    org.kde.koolintelligence
)

target_sources(koolintelligence
    PRIVATE
    main.cpp
)

ecm_target_qml_sources(koolintelligence
    SOURCES
    qml/Main.qml
)

target_link_libraries(koolintelligence
    PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
)
    
install(TARGETS koolintelligence ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.koolintelligence.desktop DESTINATION ${KDE_INSTALL_APPDIR})
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
