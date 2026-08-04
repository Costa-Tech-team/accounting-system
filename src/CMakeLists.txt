set(
  SOURCE_FILES
  main.cpp
  core/account.cpp
  core/accounting_entry.cpp
  core/accounts_chart.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#find_package(library_name CONFIG REQUIRED)

#target_link_libraries(${PROJECT_NAME} PRIVATE library::name)