cmake_minimum_required(VERSION 2.8.3)
project(Fervor)
option(FERVOR_BUILD_SAMPLE "Should the sample be built?" OFF)
option(FERVOR_BUILD_TESTS "Should the tests be built" OFF)
option(FERVOR_IGNORE_SSL_ERRORS "Ignore SSL errors" ON)

IF( USE_QT4 )
  FIND_PACKAGE( Qt4 REQUIRED  QtCore QtGui QtNetwork QtWebKit )
  INCLUDE( ${QT_USE_FILE} )
  SET(QT_USE_QTNETWORK true)
  SET(QT_USE_QTWEBKIT true)
  ADD_DEFINITIONS( ${QT_DEFINITIONS} )
  ADD_DEFINITIONS( -DUSE_QT4_FERVOR)
ELSE()
 FIND_PACKAGE( Qt5Core REQUIRED )
 FIND_PACKAGE( Qt5Gui REQUIRED )
 FIND_PACKAGE( Qt5Widgets REQUIRED )
 FIND_PACKAGE( Qt5Network REQUIRED )
 FIND_PACKAGE( Qt5WebKit REQUIRED )
 FIND_PACKAGE( Qt5WebKitWidgets REQUIRED )
  
 INCLUDE_DIRECTORIES( ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5WebKit_INCLUDE_DIRS}  ${Qt5WebKitWidgets_INCLUDE_DIRS} )
 LIST(APPEND CMAKE_CXX_FLAGS ${Qt5Core_EXECUTABLE_COMPILE_FLAGS} )
 SET(QT_USE_QTNETWORK true)
 SET(QT_USE_QTWEBKIT true)
 
 SET(FERVOR_INCLUDE_DIRS ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5WebKit_INCLUDE_DIRS}  ${Qt5WebKitWidgets_INCLUDE_DIRS} PARENT_SCOPE )
 SET(FERVOR_DEPENDENCIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5WebKit_LIBRARIES}  ${Qt5WebKitWidgets_LIBRARIES} PARENT_SCOPE )
ENDIF()

IF( FERVOR_IGNORE_SSL_ERRORS )
  ADD_DEFINITIONS( -DFERVOR_IGNORE_SSL_ERRORS)
ENDIF()

if(DEFINED FERVOR_APP_VERSION)
  add_definitions(-DFV_APP_VERSION="${FERVOR_APP_VERSION}")
else()
  add_definitions(-DFV_APP_VERSION="1.0")
  message(WARNING "No version information for Fervor given (please set FERVOR_APP_VERSION)! Assuming 1.0.")
endif()
if(DEFINED FERVOR_APP_NAME)
  add_definitions(-DFV_APP_NAME="${FERVOR_APP_NAME}")
else()
  add_definitions(-DFV_APP_NAME="App Name")
  message(WARNING "No application name for Fervor given (please set FERVOR_APP_NAME)! Assuming 'App Name'.")
endif()

set(SOURCES
  fvavailableupdate.cpp
  fvignoredversions.cpp
  fvplatform.cpp
  fvupdateconfirmdialog.cpp
  fvupdater.cpp
  fvupdatewindow.cpp
  fvversioncomparator.cpp
)

set(MOC_HEADERS
  fvavailableupdate.h
  fvignoredversions.h
  fvplatform.h
  fvupdateconfirmdialog.h
  fvupdater.h
  fvupdatewindow.h
)

set(UIS
  fvupdateconfirmdialog.ui
  fvupdatewindow.ui
)

IF( USE_QT4 )
  QT4_WRAP_UI(UI_HEADERS ${UIS})
  QT4_WRAP_CPP(MOC_SOURCES ${MOC_HEADERS})
ELSE()
  QT5_WRAP_UI(UI_HEADERS ${UIS})
  QT5_WRAP_CPP(MOC_SOURCES ${MOC_HEADERS})
ENDIF()


include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
add_library(Fervor ${SOURCES} ${MOC_HEADERS} ${MOC_SOURCES} ${UIS})

if(FERVOR_BUILD_SAMPLE)
  add_subdirectory(sample)
endif()
if(FERVOR_BUILD_TESTS)
  ENABLE_TESTING()
  add_subdirectory(tests)
endif()