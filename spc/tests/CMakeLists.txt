project(testing)


add_executable(spc_test_attitude attitude.cpp)
target_link_libraries(spc_test_attitude spc_elements spc_external spc_methods spc_io)

add_executable(spc_test_boost_eval test_boost_eval.cpp)
#target_link_libraries(test_boost_eval spc_elements)

#add_executable(spc_test_boost_eval2 test_boost_eval2.cpp)
#target_link_libraries(spc_test_boost_eval2 spc_calibration spc_external)

add_executable(spc_test_boost_eval3 test_boost_eval3.cpp)
target_link_libraries(spc_test_boost_eval3   spc_elements spc_external spc_methods)

add_executable(spc_test_cereal_serialize cereal_serialize.cpp)
target_link_libraries(spc_test_cereal_serialize  spc_elements spc_external spc_methods)



add_executable(spc_uuid uuid.cpp)
target_link_libraries(spc_uuid  spc_elements spc_external spc_methods spc_io)

add_executable(spc_sample sample.cpp)
target_link_libraries(spc_sample  spc_elements spc_external spc_methods)

add_executable(spc_test_flann_bug flann_bug.cpp)
target_link_libraries(spc_test_flann_bug ${PCL_LIBRARIES})

add_executable(spc_test_polynomials polynomials.cpp)
target_link_libraries(spc_test_polynomials ${PCL_LIBRARIES})

