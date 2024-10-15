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

)

find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    I18n
    CoreAddons
    QQC2DesktopStyle
    IconThemes
    XmlGui
    KIO
    Parts
    )

#these two options MUST be here only, else qt like a big dumb pussy will not allow us to compile this and throw random errors
#YOU HAVE BEEN WARNED.(I wasted 4 hours on this piece of dumb shit)
add_executable(koolintelligence)
target_compile_options(koolintelligence PRIVATE "-fexceptions")


ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

ecm_add_qml_module(koolintelligence
    URI
    org.kde.koolintelligence
)

ecm_target_qml_sources(koolintelligence
    SOURCES
    qml/Main.qml
    qml/TerminalPanel.qml
)

file(GLOB UI_CPP_FILES *.cpp)

target_sources(koolintelligence
    PRIVATE
    ${UI_CPP_FILES}
)

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
    KF6::XmlGui
    KF6::KIOCore
    KF6::Parts
)
install(TARGETS koolintelligence ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.koolintelligence.desktop DESTINATION ${KDE_INSTALL_APPDIR})
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
add_subdirectory(terminal)
