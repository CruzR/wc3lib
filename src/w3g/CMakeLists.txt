if (W3G)
	WC3LIB_ENABLE_EXCEPTIONS()

	WC3LIB_GET_H(wc3lib_W3G_H)
	WC3LIB_GET_SRC(wc3lib_W3G_SRC)

	# install
	install(FILES ${wc3lib_CORE_H} DESTINATION include/wc3lib/w3g)
endif ()