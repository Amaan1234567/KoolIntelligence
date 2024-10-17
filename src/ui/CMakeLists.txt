include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Concurrent
    Quick
    Test
    Gui
    QuickControls2
    Widgets
    DBus
    Core5Compat
)

find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    I18n
    CoreAddons
    QQC2DesktopStyle
    IconThemes
    Baloo 
    Pty
    )

#these two options MUST be here, else it will not work
add_executable(koolintelligence)
target_compile_options(koolintelligence PRIVATE "-fexceptions")

target_include_directories(koolintelligence PRIVATE ${VTERM_INCLUDE_DIR})

add_library(koolintelligence-components)

ecm_add_qml_module(koolintelligence-components
    URI org.kde.koolintelligence.components
    VERSION 1.0
)

ecm_target_qml_sources(koolintelligence-components
    SOURCES
    qml/TerminalLayout.qml
    qml/ChatHistory.qml
    qml/SettingsPage.qml
)

ecm_finalize_qml_module(koolintelligence-components)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

ecm_add_qml_module(koolintelligence
    URI
    org.kde.koolintelligence
)

ecm_target_qml_sources(koolintelligence
    SOURCES
    qml/Main.qml
)

file(GLOB UI_CPP_FILES *.cpp)

target_sources(koolintelligence
    PRIVATE
    ${UI_CPP_FILES}
)

add_subdirectory(terminal)

target_link_libraries(koolintelligence
    PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::DBus
    Qt6::Concurrent
    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
    KF6::Baloo
    KonsoleQML
    koolintelligence-components
)
    
install(TARGETS koolintelligence-components ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS koolintelligence ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.koolintelligence.desktop DESTINATION ${KDE_INSTALL_APPDIR})
