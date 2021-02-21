QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PKGCONFIG += opencv

INCLUDEPATH += C:\opencv\build\install\include
LIBS += C:\opencv\build\lib\libopencv_core430.dll.a
LIBS += C:\opencv\build\lib\libopencv_highgui430.dll.a
LIBS += C:\opencv\build\lib\libopencv_imgproc430.dll.a
LIBS += C:\opencv\build\lib\libopencv_imgcodecs430.dll.a
LIBS += C:\opencv\build\lib\libopencv_video430.dll.a
LIBS += C:\opencv\build\lib\libopencv_photo430.dll.a
LIBS += C:\opencv\build\lib\libopencv_objdetect430.dll.a
LIBS += C:\opencv\build\lib\libopencv_features2d430.dll.a
LIBS += C:\opencv\build\lib\libade.a
LIBS += C:\opencv\build\lib\libopencv_ts430.a
LIBS += C:\opencv\build\lib\libopencv_videoio430.dll.a
LIBS += C:\opencv\build\lib\libopencv_stitching430.dll.a
LIBS += C:\opencv\build\lib\libopencv_ml430.dll.a
LIBS += C:\opencv\build\lib\libopencv_gapi430.dll.a
LIBS += C:\opencv\build\lib\libopencv_flann430.dll.a
LIBS += C:\opencv\build\lib\libopencv_calib3d430.dll.a

SOURCES += \
    main.cpp \
    dailog.cpp

HEADERS += \
    dailog.h

FORMS += \
    dailog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
