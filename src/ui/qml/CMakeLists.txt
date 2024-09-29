ecm_add_qml_module(koolintelligence
    URI
    org.kde.koolintelligence
)

ecm_target_qml_sources(koolintelligence
    SOURCES
    Main.qml
)

# add_library(koolintelligence-components)

# ecm_add_qml_module(koolintelligence-components
#     URI
#     org.kde.koolintelligence.components
# )

# ecm_target_qml_sources(koolintelligence-components
#     SOURCES
#     qml/ScrollView.qml
# )

# ecm_finalize_qml_module(koolintelligence-components)

# install(TARGETS koolintelligence-components ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})