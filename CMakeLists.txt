cmake_minimum_required(VERSION 3.21)
project(SmartPrioritizedCalendar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

add_executable(SmartPrioritizedCalendar main.cpp LoginWindow.cpp LoginWindow.hpp SignUpDialog.cpp SignUpDialog.hpp MainWindow.cpp MainWindow.hpp Event.cpp Event.hpp NetworkManager.cpp NetworkManager.hpp AuthManager.hpp)

target_link_libraries(SmartPrioritizedCalendar PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Boost::system OpenSSL::SSL OpenSSL::Crypto)
