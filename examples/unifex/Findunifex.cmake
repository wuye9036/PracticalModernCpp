find_package(unifex NO_MODULE)

if(unifex_FOUND)
  get_target_property(link_libs unifex::unifex INTERFACE_LINK_LIBRARIES)
  list(REMOVE_ITEM link_libs std::coroutines)
  set_property(TARGET unifex::unifex PROPERTY INTERFACE_LINK_LIBRARIES "${link_libs}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(unifex CONFIG_MODE)