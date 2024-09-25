add_executable(koolintelligence)

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
    Main.qml
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