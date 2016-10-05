set(DOCUMENTATION "OTB module for geodesic opening and closing.")

# OTB_module() defines the module dependencies in ExternalTemplate
# ExternalTemplate depends on OTBCommon and OTBApplicationEngine
# The testing module in ExternalTemplate depends on OTBTestKernel
# and OTBCommandLine

# define the dependencies of the include module and the tests
otb_module(OTBGeodesicFilters
  DEPENDS
    OTBCommon
    OTBApplicationEngine
  TEST_DEPENDS
    OTBCommandLine
  DESCRIPTION
    "${DOCUMENTATION}"
)
