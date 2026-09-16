set(
  SOURCE_FILES
  main.cpp
  core/account_node.cpp
  core/accounting_entry.cpp
  core/accounts_chart.cpp
  core/journal.cpp
  db/database_handler.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)

#find_package(library_name CONFIG REQUIRED)

#target_link_libraries(${PROJECT_NAME} PRIVATE library::name)