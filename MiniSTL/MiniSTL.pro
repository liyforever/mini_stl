TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    memory.h \
    mini_stl_construct.h \
    mini_stl_iterator.h \
    mini_stl_type_traits.h \
    mini_stl_allocator.h \
    mini_stl_uninitialized.h \
    mini_stl_vector.h \
    mini_stl_algobase.h \
    mini_stl_config.h \
    mini_std_pair.h

OTHER_FILES += \
    info.txt

