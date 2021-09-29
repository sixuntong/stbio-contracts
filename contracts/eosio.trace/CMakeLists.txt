add_contract(eosio.trace eosio.trace
   ${CMAKE_CURRENT_SOURCE_DIR}/src/eosio.trace.cpp
)

target_include_directories(eosio.trace
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../eosio.token/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
   
set_target_properties(eosio.trace
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
