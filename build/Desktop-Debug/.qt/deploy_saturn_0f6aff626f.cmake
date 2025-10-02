include("C:/Qt/repos/saturn_new_map_engine/build/Desktop-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/saturn-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative")

qt6_deploy_runtime_dependencies(
    EXECUTABLE C:/Qt/repos/saturn_new_map_engine/build/Desktop-Debug/saturn.exe
    GENERATE_QT_CONF
)
