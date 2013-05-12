# Install script for directory: /home/lovewilliam/Develop/h3cc

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}/usr/lib/libh3c8021x.so.1.0"
      "$ENV{DESTDIR}/usr/lib/libh3c8021x.so.2"
      "$ENV{DESTDIR}/usr/lib/libh3c8021x.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/lib/libh3c8021x.so.1.0;/usr/lib/libh3c8021x.so.2;/usr/lib/libh3c8021x.so")
FILE(INSTALL DESTINATION "/usr/lib" TYPE SHARED_LIBRARY FILES
    "/home/lovewilliam/Develop/h3cc/build/libh3c8021x.so.1.0"
    "/home/lovewilliam/Develop/h3cc/build/libh3c8021x.so.2"
    "/home/lovewilliam/Develop/h3cc/build/libh3c8021x.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/usr/lib/libh3c8021x.so.1.0"
      "$ENV{DESTDIR}/usr/lib/libh3c8021x.so.2"
      "$ENV{DESTDIR}/usr/lib/libh3c8021x.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/include/h3c8021x.h")
FILE(INSTALL DESTINATION "/usr/include" TYPE FILE FILES "/home/lovewilliam/Develop/h3cc/h3c8021x.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/bin/h3cc")
FILE(INSTALL DESTINATION "/usr/bin" TYPE PROGRAM FILES "/home/lovewilliam/Develop/h3cc/build/h3cc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/bin/h3cgui")
FILE(INSTALL DESTINATION "/usr/bin" TYPE PROGRAM FILES "/home/lovewilliam/Develop/h3cc/build/h3cgui")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/share/h3cc/icons/h3cc128x128.png;/usr/share/h3cc/icons/h3cc.png")
FILE(INSTALL DESTINATION "/usr/share/h3cc/icons" TYPE FILE FILES
    "/home/lovewilliam/Develop/h3cc/icons/h3cc128x128.png"
    "/home/lovewilliam/Develop/h3cc/icons/h3cc.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/lovewilliam/Develop/h3cc/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/lovewilliam/Develop/h3cc/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
