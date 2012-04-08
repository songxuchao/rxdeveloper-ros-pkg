find_package(Qt4 COMPONENTS QtCore QtGui QtSql QtSvg REQUIRED)

include(${QT_USE_FILE})

qt4_wrap_cpp(MOC_SOURCES
    tagItems/nodeItem.h
    tagItems/nodeEdit.h
    tagItems/argItem.h
    tagItems/argEdit.h
    tagItems/launchEdit.h
    tagItems/envItem.h
    tagItems/envEdit.h
    tagItems/remapArrowEdit.h
    tagItems/remapItem.h
    tagItems/remapEdit.h
    tagItems/includeFileItem.h
    tagItems/includeFileEdit.h
    tagItems/parameterItem.h
    tagItems/parameterEdit.h
    tagItems/nodeParamEdit.h
    tagItems/rosparamItem.h
    tagItems/rosparamEdit.h
    tagItems/groupEdit.h
    tagItems/machineItem.h
    tagItems/machineEdit.h
    nodeSelection.h
    rxdev.h
    newEntry.h
    wikiDialog.h
    #specFileParser.h
    specFileEdit.h
    settings.h

)
qt4_wrap_ui(UI_HEADERS
    rxdev.ui
    newEntry.ui
    wikiDialog.ui
    specFileEdit.ui
    settings.ui
    nodeSelection.ui
    tagItems/parameterEdit.ui
    tagItems/rosparamEdit.ui
    tagItems/nodeParamEdit.ui
    tagItems/includeFileEdit.ui
    tagItems/machineEdit.ui
    tagItems/argEdit.ui
    tagItems/launchEdit.ui
    tagItems/envEdit.ui
    tagItems/nodeEdit.ui
    tagItems/groupEdit.ui
    tagItems/remapArrowEdit.ui
    tagItems/remapEdit.ui
)

qt4_add_resources(QT_RESOURCES icons.qrc)

set( CPP

    tagItems/nodeItem.cpp
    tagItems/nodeEdit.cpp
    tagItems/groupItem.cpp
    tagItems/groupEdit.cpp
    tagItems/argItem.cpp
    tagItems/argEdit.cpp
    tagItems/launchEdit.cpp
    tagItems/envItem.cpp
    tagItems/envEdit.cpp
    tagItems/includeFileItem.cpp
    tagItems/includeFileEdit.cpp
    tagItems/nodeParamEdit.cpp
    tagItems/parameterItem.cpp
    tagItems/parameterEdit.cpp
    tagItems/rosparamItem.cpp
    tagItems/rosparamEdit.cpp
    tagItems/machineItem.cpp
    tagItems/machineEdit.cpp
    tagItems/remapArrow.cpp
    tagItems/remapArrowEdit.cpp
    tagItems/remapItem.cpp
    tagItems/remapEdit.cpp
    main.cpp
    rxdev.cpp
    nodeSelection.cpp
    c_creator.cpp
    c_connector.cpp
    newEntry.cpp
    wikiDialog.cpp
    specFileEdit.cpp
    specFileParser.cpp
    settings.cpp
    launchFileView.cpp
    launchFileScene.cpp
    launchFileWriter.cpp
    launchFileReader.cpp
#diagramscene.cpp
    #diagramitem.cpp
)


#set( HEADERS
#    remapArrow.h
    #diagramscene.h
    #diagramitem.h
#)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
rosbuild_add_executable(rxdev ${CPP} ${MOC_SOURCES} ${UI_HEADERS} ${QT_RESOURCES})
#rosbuild_add_executable(yaml_parser yaml_parser.cpp)
target_link_libraries(rxdev ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${PYTHON_LIBRARIES} tinyxml yaml-cpp)
#target_link_libraries(yaml_parser ${QT_LIBRARIES})


