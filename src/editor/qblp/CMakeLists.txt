WC3LIB_ENABLE_EXCEPTIONS()

WC3LIB_GET_H(wc3lib_EDITOR_QBLP_H)
WC3LIB_GET_SRC(wc3lib_EDITOR_QBLP_SRC)
set(wc3lib_EDITOR_QBLP_SRC ${wc3lib_EDITOR_QBLP_SRC} PARENT_SCOPE)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

if ("${CMAKE_BUILD_TYPE}" EQUAL "Release")
	add_definitions(-DQT_NO_DEBUG)
endif ()

# BLP Qt plugin
# allows you to read and write BLP image files by using class QImage and image type "BLP"
add_library(qblp SHARED ${wc3lib_EDITOR_QBLP_SRC})
target_link_libraries(qblp wc3libcore wc3libblp ${Boost_LIBRARIES} ${QT_LIBRARIES})
# wc3libcore is necessary because of catching Exception
# TODO drop ${Boost_LIBRARIES}

# install
install(TARGETS qblp DESTINATION ${QT_PLUGINS_DIR}/imageformats)
install(FILES ${wc3lib_EDITOR_QBLP_H} PUBLIC_HEADER DESTINATION include/wc3lib/editor/qblp)