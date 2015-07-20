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
    mini_stl_list.h \
    mini_stl_deque.h \
    mini_stl_stack.h \
    mini_stl_queue.h \
    mini_stl_pair.h \
    mini_stl_function.h \
    mini_stl_tree.h \
    mini_stl_set.h \
    mini_stl_map.h \
    mini_stl_multiset.h \
    mini_stl_hashtable.h \
    mini_stl_multimap.h \
    mini_stl_string.h \
    mini_stl_chartraits.h \
    mini_stl_hash_fun.h \
    mini_stl_unordered_set.h \
    mini_stl_unordered_multiset.h \
    mini_stl_unordered_map.h \
    mini_stl_unordered_multimap.h

OTHER_FILES += \
    info.txt

