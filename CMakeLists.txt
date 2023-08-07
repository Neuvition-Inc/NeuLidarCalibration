cmake_minimum_required(VERSION 3.5)

project(NeuLidarCalibration LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/ubuntu/Qt5.14.1/5.14.1/gcc_64")
find_package(Qt5 COMPONENTS Widgets Network SerialPort REQUIRED)
set(QT_LIBRARIES Qt5::Widgets Qt5::Network Qt5::SerialPort)
find_package(Boost 1.73 EXACT REQUIRED)

find_package(VTK 8.2.0 EXACT REQUIRED)
if(VTK_FOUND)
message ("vtk found")
else()
message (FATAL_ERROR "Cannot find vtk")
endif()

find_package(PCL 1.11.0 EXACT REQUIRED)


if(PCL_FOUND)
message ("pcl found")
else()
message (FATAL_ERROR "Cannot find pcl")
endif()
add_definitions     (${PCL_DEFINITIONS})

find_package(OpenCV 4.1.1 REQUIRED )
if(OpenCV_FOUND)
message ("opencv found")
else()
message (FATAL_ERROR "Cannot find opencv")
endif()

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(NEUVSDK_INCLUDE_DIR "../../neusdk/sdk/include")
set(NEUVSDK_LIBRARY_DIR "../../neusdk/sdk/lib")

set(FFMPEG_INCLUDE_DIR "../../neusdk/ffmpegrtspso/ffmpeg/include")
set(FFMPEG_LIBRARY_DIR "../../neusdk/ffmpegrtspso/ffmpeg/lib")
set(FFMPEGRTSP_INCLUDE_DIR "../../neusdk/ffmpegrtspso/sdk/include")
set(FFMPEGRTSP_LIBRARY_DIR "../../neusdk/ffmpegrtspso/sdk/lib")



set(OCELLUS_INCLUDE_DIR "../../neusdk/third/ocellus/include")
set(OCELLUS_LIBRARY_DIR "../../neusdk/third/ocellus/lib")

set(QINIU_INCLUDE_DIR "../third/qiniu/include")
set(QINIU_LIBRARY_DIR "../third/qiniu/lib")

set  (CMAKE_BUILD_TYPE "Debug")
set  (CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -static-libgcc -static-libstdc++")
set  (CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -pthread -static-libgcc -static-libstdc++")

set  (project_SOURCES   main.cpp 
    ./PTZModule/CNeuPTZCore.cpp
    ./PTZModule/CPTZBaseController.cpp
    ./PTZModule/CPTZBasePanel.cpp
    ./PTZModule/CPTZConfig.cpp
    ./PTZModule/CPTZCore.cpp
    ./PTZModule/CPTZUdpCore.cpp
    LaserInstrumentControl.cpp
    Neuvition_Auto.cpp
    SerialPort.cpp
    SlideControl.cpp
    test.cpp
    #./Common/LidarModule/Expand/CNeuvObserver.cpp
    ./Common/LidarModule/CLidarINeuvEvent.cpp
    ./Common/LidarModule/CMatObserver.cpp
    ./Common/LidarModule/CMatSubject.cpp
    ./Common/LidarModule/CNeuvLidarControl.cpp
    ./Common/LidarModule/CNeuvSubject.cpp
    ./Common/LidarModule/globalLidarCallbackFun.cpp
    ./Common/LidarModule/lidar_common_defs.cpp
    ./Common/LidarModule/Subject.cpp
    ./Common/VTKModule/CPCLPainter.cpp
    ./Common/VTKModule/CVTKWidget.cpp
    ./Common/VTKModule/pcl_extern_func.cpp
    ./Common/CGlobalSignalSlot.cpp
    ./Common/CViewController.cpp
     MainWin.cpp
     CalibrationAlgorithm.cpp
     CalibrationProcess.cpp)


set  (project_HEADERS   ./PTZModule/CNeuPTZCore.h
    ./PTZModule/CPTZBaseController.h
    ./PTZModule/CPTZBasePanel.h
    ./PTZModule/CPTZConfig.h
    ./PTZModule/CPTZCore.h
    ./PTZModule/CPTZUdpCore.h
    ./PTZModule/CPTZStateManage.h
    LaserInstrumentControl.h
    Neuvition_Auto.h
    SerialPort.h
    SlideControl.h
    test.h
    #./Common/LidarModule/Expand/test_lidar_data_process.h
    #./Common/LidarModule/Expand/CNeuvObserver.h
    ./Common/LidarModule/CLidarINeuvEvent.h
    ./Common/LidarModule/CMatObserver.h
    ./Common/LidarModule/CMatSubject.h
    ./Common/LidarModule/CNeuvLidarControl.h
    ./Common/LidarModule/CNeuvSubject.h
    ./Common/LidarModule/globalLidarCallbackFun.h
    ./Common/LidarModule/lidar_common_defs.h
    ./Common/LidarModule/Subject.h
    ./Common/VTKModule/CPCLPainter.h
    ./Common/VTKModule/CVTKWidget.h
    ./Common/VTKModule/pcl_extern_func.h
    ./Common/CGlobalSignalSlot.h
    ./Common/CViewController.h
    ./Common/boost_common_defs.h
    ./Common/opencv_common_defs.h
    ./Common/pcl_common_defs.h
    MainWin.h
    CalibrationAlgorithm.h
    CalibrationProcess.h

    resource.h)

set  (project_FORMS      test.ui
    ./Common/VTKModule/CVTKWidget.ui
     MainWin.ui)
set  (project_RESOURCES CMainWin.qrc)

add_definitions(${QT_DEFINITIONS})

#include
include_directories(${OpenCV_INCLUDE_DIRS}
                    ${NEUVSDK_INCLUDE_DIR}
                    ${FFMPEG_INCLUDE_DIR}
                    ${FFMPEGRTSP_INCLUDE_DIR}

		    ${OCELLUS_INCLUDE_DIR}
                  
                    ${VTK_INCLUDE_DIR}
                    ${PCL_INCLUDE_DIRS}
                    ${CURL_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIRS}
                    ${QINIU_INCLUDE_DIR})

#library
link_directories(${OpenCV_LIBRARY_DIRS}
                 ${NEUVSDK_LIBRARY_DIR}
                 ${FFMPEG_LIBRARY_DIR}
                 ${FFMPEGRTSP_LIBRARY_DIR}

		 ${OCELLUS_LIBRARY_DIR}
            
                 ${PCL_LIBRARY_DIRS}
                 ${VTK_LIBRARY_DIR}
                 ${CURL_LIBRARY_DIR}
                 ${Boost_LIBRARY_DIRS}
                 ${QINIU_LIBRARY_DIR})


add_executable(NeuLidarCalibration ${project_SOURCES}
                         ${project_FORMS}
                         ${project_FORMS_HEADERS}
                         ${project_HEADERS_MOC}
                         ${project_RES_MOC}
                         ${Boost_LIBRARY_DIRS}
                         ${project_RESOURCES}
			)
                         #"icon.rc")

# -lboost_system -lboost_thread -lavutil -lavcodec -lavformat -lavdevice -lavfilter -lswscale -lpostproc -lffmpegrtsp

target_link_libraries(NeuLidarCalibration ${QT_LIBRARIES}
                                ${OpenCV_LIBS}
                                ${PCL_LIBRARIES}
                                ${VTK_LIBRARIES}
                                -lneusdk
                                -lopencv_imgcodecs -lavutil -lavcodec -lavformat -lavdevice -lavfilter -lswscale -lpostproc -lffmpegrtsp -lwoodpecker_arm
                                -lqiniu
                                -luuid -lcurl -lboost_thread -lboost_system -lpthread )
