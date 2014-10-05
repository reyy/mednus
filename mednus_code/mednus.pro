#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T14:37:29
#
#-------------------------------------------------
QT       += core gui widgets
TARGET = MedNUS
TEMPLATE = app

HEADERS += PMeshViewer.h \
    PMeshModel.h \
    PMeshTableWidget.h \
    PMeshViewerCallback.h \
    MedNUSLessonPanel.h \
    MedMUSMeshViewer.h

SOURCES += main.cpp \
    PMeshViewer.cpp \
    PMeshModel.cpp \
    PMeshTableWidget.cpp \
    PMeshViewerCallback.cpp \
    MedNUSLessonPanel.cpp \
    MedMUSMeshViewer.cpp

RESOURCES = panax.qrc


CONFIG += MAC_CONFIG
MAC_CONFIG {
    QMAKE_CXXFLAGS = -std=c++11 -stdlib=libstdc++ -mmacosx-version-min=10.9 -ftemplate-depth=1024 -DBUILD_visualization=off
    QMAKE_LFLAGS = -std=c++11 -stdlib=libstdc++ -mmacosx-version-min=10.9 -ftemplate-depth=1024 -DBUILD_visualization=off
}
#-fobjc-gc
INCLUDEPATH += /usr/local/include/vtk-6.1/
LIBS += -L/usr/local/lib
LIBS += -lstdc++
LIBS += -lvtksys-6.1.1
LIBS += -lvtkCommonCore-6.1.1
LIBS += -lvtkCommonMath-6.1.1
LIBS += -lvtkCommonMisc-6.1.1
LIBS += -lvtkCommonSystem-6.1.1
LIBS += -lvtkCommonTransforms-6.1.1
LIBS += -lvtkCommonDataModel-6.1.1
LIBS += -lvtkCommonColor-6.1.1
LIBS += -lvtkCommonExecutionModel-6.1.1
LIBS += -lvtkFiltersCore-6.1.1
LIBS += -lvtkCommonComputationalGeometry-6.1.1
LIBS += -lvtkFiltersGeneral-6.1.1
LIBS += -lvtkImagingCore-6.1.1
LIBS += -lvtkImagingFourier-6.1.1
LIBS += -lvtkalglib-6.1.1
LIBS += -lvtkFiltersStatistics-6.1.1
LIBS += -lvtkFiltersExtraction-6.1.1
LIBS += -lvtkInfovisCore-6.1.1
LIBS += -lvtkFiltersGeometry-6.1.1
LIBS += -lvtkFiltersSources-6.1.1
LIBS += -lvtkRenderingCore-6.1.1
LIBS += -lvtkzlib-6.1.1
LIBS += -lvtkfreetype-6.1.1
LIBS += -lvtkftgl-6.1.1
LIBS += -lvtkRenderingFreeType-6.1.1
LIBS += -lvtkDICOMParser-6.1.1
LIBS += -lvtkIOCore-6.1.1
LIBS += -lvtkmetaio-6.1.1
LIBS += -lvtkjpeg-6.1.1
LIBS += -lvtkpng-6.1.1
LIBS += -lvtktiff-6.1.1
LIBS += -lvtkIOImage-6.1.1
LIBS += -lvtkImagingHybrid-6.1.1
LIBS += -lvtkRenderingOpenGL-6.1.1
LIBS += -lvtkRenderingContext2D-6.1.1
LIBS += -lvtkChartsCore-6.1.1
LIBS += -lvtkjsoncpp-6.1.1
LIBS += -lvtkIOGeometry-6.1.1
LIBS += -lvtkexpat-6.1.1
LIBS += -lvtkIOXMLParser-6.1.1
LIBS += -lvtkIOXML-6.1.1
LIBS += -lvtkDomainsChemistry-6.1.1
LIBS += -lvtkIOLegacy-6.1.1
LIBS += -lvtkParallelCore-6.1.1
LIBS += -lvtkFiltersAMR-6.1.1
LIBS += -lvtkFiltersFlowPaths-6.1.1
LIBS += -lvtkFiltersGeneric-6.1.1
LIBS += -lvtkImagingSources-6.1.1
LIBS += -lvtkFiltersHybrid-6.1.1
LIBS += -lvtkFiltersHyperTree-6.1.1
LIBS += -lvtkImagingGeneral-6.1.1
LIBS += -lvtkFiltersImaging-6.1.1
LIBS += -lvtkFiltersModeling-6.1.1
LIBS += -lvtkFiltersParallel-6.1.1
LIBS += -lvtkFiltersParallelImaging-6.1.1
LIBS += -lvtkFiltersProgrammable-6.1.1
LIBS += -lvtkFiltersSMP-6.1.1
LIBS += -lvtkFiltersSelection-6.1.1
LIBS += -lvtkFiltersTexture-6.1.1
LIBS += -lvtkverdict-6.1.1
LIBS += -lvtkFiltersVerdict-6.1.1
LIBS += -lvtkInteractionStyle-6.1.1
LIBS += -lvtkGUISupportQt-6.1.1
LIBS += -lvtkGUISupportQtOpenGL-6.1.1
LIBS += -lvtksqlite-6.1.1
LIBS += -lvtkIOSQL-6.1.1
LIBS += -lvtkGUISupportQtSQL-6.1.1
LIBS += -lvtkInfovisLayout-6.1.1
LIBS += -lvtkRenderingLabel-6.1.1
LIBS += -lvtkImagingColor-6.1.1
LIBS += -lvtkRenderingAnnotation-6.1.1
LIBS += -lvtkRenderingVolume-6.1.1
LIBS += -lvtkInteractionWidgets-6.1.1
LIBS += -lvtkViewsCore-6.1.1
LIBS += -lvtkViewsInfovis-6.1.1
LIBS += -lvtkViewsQt-6.1.1
LIBS += -lvtkGUISupportQtWebkit-6.1.1
LIBS += -lvtkproj4-6.1.1
LIBS += -lvtkGeovisCore-6.1.1
LIBS += -lvtkhdf5-6.1.1
LIBS += -lvtkhdf5_hl-6.1.1
LIBS += -lvtkIOAMR-6.1.1
LIBS += -lvtkIOEnSight-6.1.1
LIBS += -lvtkNetCDF-6.1.1
LIBS += -lvtkNetCDF_cxx-6.1.1
LIBS += -lvtkexoIIc-6.1.1
LIBS += -lvtkIOExodus-6.1.1
LIBS += -lvtkgl2ps-6.1.1
LIBS += -lvtkRenderingGL2PS-6.1.1
LIBS += -lvtkIOExport-6.1.1
LIBS += -lvtkIOImport-6.1.1
LIBS += -lvtklibxml2-6.1.1
LIBS += -lvtkIOInfovis-6.1.1
LIBS += -lvtkIOLSDyna-6.1.1
LIBS += -lvtkIOMINC-6.1.1
LIBS += -lvtkoggtheora-6.1.1
LIBS += -lvtkIOMovie-6.1.1
LIBS += -lvtkIONetCDF-6.1.1
LIBS += -lvtkIOPLY-6.1.1
LIBS += -lvtkIOParallel-6.1.1
LIBS += -lvtkIOVideo-6.1.1
LIBS += -lvtkImagingMath-6.1.1
LIBS += -lvtkImagingMorphological-6.1.1
LIBS += -lvtkImagingStatistics-6.1.1
LIBS += -lvtkImagingStencil-6.1.1
LIBS += -lvtkInteractionImage-6.1.1
LIBS += -lvtkRenderingFreeTypeOpenGL-6.1.1
LIBS += -lvtkRenderingImage-6.1.1
LIBS += -lvtkRenderingLIC-6.1.1
LIBS += -lvtkRenderingLOD-6.1.1
LIBS += -lvtkRenderingQt-6.1.1
LIBS += -lvtkRenderingVolumeAMR-6.1.1
LIBS += -lvtkRenderingVolumeOpenGL-6.1.1
LIBS += -lvtkViewsContext2D-6.1.1
LIBS += -lvtkViewsGeovis-6.1.1
