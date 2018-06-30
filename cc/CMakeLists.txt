
cmake_minimum_required(VERSION 3.5)


project(prototest)

add_definitions(-std=c++11)

#===================== Protobuf =========================
# # 使用pkg-config 寻找路径
# find_package(PkgConfig)
# pkg_check_modules(Protobuf protobuf>=3.0)
# if (Protobuf_FOUND)
#     message("Protobuf Found")
#     message(${PROTOBUF_INCLUDE_DIR})
#     message(${PROTOBUF_LIBRARY})
#     include_directories(${PROTOBUF_INCLUDE_DIR})
#     link_libraries(${PROTOBUF_LIBRARY})
# endif()
# find_package(Threads REQUIRED)

# if (Threads_FOUND)
#     message("Pthread Found")
#     message(${CMAKE_THREAD_LIBS_INIT})
#     link_libraries(${CMAKE_THREAD_LIBS_INIT})
# endif()



# 通过find_package
# 前提 .bash 需要修改
find_package(Protobuf REQUIRED)
if (Protobuf_FOUND)
    message("Protobuf Found")
    message(${PROTOBUF_INCLUDE_DIR})
    message(${PROTOBUF_LIBRARIES})
    include_directories(${PROTOBUF_INCLUDE_DIR})
    link_libraries(${PROTOBUF_LIBRARIES})

    # 添加到编译系统
    # 添加到
endif()


# # 手动添加 2
# include_directories(/usr/local/protobuf/include/)
# # 
# link_libraries(/usr/local/protobuf/lib/libprotobuf.so pthread)

# === end ====

# ===================================

# # PCL 点云库
# find_package(PCL 1.7 REQUIRED)
# if (PCL_FOUND)
#     # 

#     list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")

#     include_directories(${PCL_INCLUDE_DIRS})
#     link_directories(${PCL_LIBRARY_DIRS})
#     add_definitions(${PCL_DEFINITIONS})

#  else()
#     message("no found pcl")

# endif()

# PCL 点云库
find_package(PCL 1.7 REQUIRED COMPONENTS common io visualization)
if (PCL_FOUND)
    # 
    include_directories(${PCL_INCLUDE_DIRS})

    add_definitions(${PCL_DEFINITIONS})
    list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")
    
    # [Linker]>[General]>[Additional Library Directories]
    link_directories( ${PCL_LIBRARY_DIRS} )
    
    # [Linker]>[Input]>[Additional Dependencies]
    #target_link_libraries( proto ${PCL_LIBRARIES} )
    link_libraries(${PCL_LIBRARIES})
 else()
    message(PCL "no found pcl")

endif()





add_executable(proto main.cc PclPointCloudPoint.pb.cc PclPointCient.cc )
