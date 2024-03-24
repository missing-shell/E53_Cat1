#Add sources from ui directory
file(GLOB_RECURSE SRC_AT ${CMAKE_SOURCE_DIR} "at/*.c")
idf_component_register(SRCS 
                            "main.c"
                            "ec800M_CN.C"
                           ${SRC_AT}
                    INCLUDE_DIRS 
                            "."
                        "include"                
)
