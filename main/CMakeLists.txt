idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    )
set(COMPONENT_REQUIRES "json")
list(APPEND EXTRA_COMPONENT_DIR components)

