cmake_minimum_required(VERSION 3.5)

idf_component_register(SRCS "moduloRele.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver")
