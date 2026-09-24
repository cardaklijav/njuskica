cmake_minimum_required(VERSION 3.10.0)
project(Firewall VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Firewall packet-capturer/src/main.cpp)

