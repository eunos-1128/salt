find_package(Sphinx REQUIRED)

configure_file(conf.py.in ${CMAKE_CURRENT_SOURCE_DIR}/conf.py @ONLY)

add_custom_target(manual_page
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/salt.1)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/salt.1
	COMMAND ${SPHINX_EXECUTABLE} -b man ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/salt.rst
	COMMENT "Generating manual pages with Sphinx")

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/salt.html
	COMMAND ${SPHINX_EXECUTABLE} -b html ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/salt.rst
	DEPENDS salt.rst
	COMMENT "Generating manual pages with Sphinx")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/salt.1
	DESTINATION ${CMAKE_INSTALL_DATADIR}/man/man1)
