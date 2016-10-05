set(OTBAppMorphology_LINK_LIBS
  ${OTBImageBase_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  )

OTB_CREATE_APPLICATION(NAME GrayScaleGeodesicMorphologicalOperation
                       SOURCES otbGrayScaleGeodesicMorphologicalOperation.cxx
                       LINK_LIBRARIES ${${otb-module}_LIBRARIES}
)





