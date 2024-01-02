# Try to find the glm library
    #
    # GLM_INCLUDE_DIR
    # //GLM_LIBRARY
    # GLM_FOUND

    FIND_PATH(
      GLM_INCLUDE_DIR glm/glm.hpp
      ${CMAKE_INCLUDE_PATH}
      $ENV{include}
      /usr/include
      /usr/include/glm
      /usr/local/include
    )

    #SET(STORE_CMAKE_FIND_FRAMEWORK ${CMAKE_FIND_FRAMEWORK})
    #SET(CMAKE_FIND_FRAMEWORK NEVER)

    IF (GLM_INCLUDE_DIR)
	    SET(GLM_FOUND TRUE)
    ENDIF (GLM_INCLUDE_DIR)

    IF (GLM_FOUND)
       IF (NOT GLM_FIND_QUIETLY)
	  MESSAGE(STATUS "Found GLM header: ${GLM_INCLUDE_DIR}")
       ENDIF (NOT GLM_FIND_QUIETLY)
    ELSE (GLM_FOUND)
       IF (GLM_FIND_REQUIRED)
          MESSAGE(FATAL_ERROR "Could not find GLM")
       ENDIF (GLM_FIND_REQUIRED)
    ENDIF (GLM_FOUND)
