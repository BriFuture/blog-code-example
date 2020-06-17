cmake_minimum_required(VERSION 3.9)

project(HWMain VERSION "0.0.1")

set(DATA_INPUT_FILE "test_data.txt" CACHE STRING "Set input data File")
# set(DATA_INPUT_FILE "test_data1.txt")
# set(DATA_INPUT_FILE "test_data2.txt")
# configure_file(${DATA_INPUT_FILE} "test_data1.txt" @ONLY)

add_executable(Main
    main.cpp definitions.h
)
set(COMP_DEF 
	#MY_DEBUG 
	MY_INFO
	)
target_compile_definitions(Main PRIVATE 
	${COMP_DEF} BUILD_WITH_CMAKE
	)
if(WIN32)
	if(CMAKE_CL_64)
		target_include_directories(Main PRIVATE
			"D:/software/pthread/include"
		)
		target_link_libraries(Main PRIVATE
			#"D:/software/pthread/dll/x64/pthreadVC2.dll"
			"D:/software/pthread/lib/x64/pthreadVC2.lib"
		)
	else()
		target_include_directories(Main PRIVATE
			"D:/software/pthread/include"
		)
		target_link_libraries(Main PRIVATE
			#"D:/software/pthread/dll/x86/pthreadVC2.dll"
			"D:/software/pthread/lib/x86/pthreadVC2.lib"
		)
	endif()
elseif(UNIX)
	target_link_libraries(Main PRIVATE pthread)
endif()