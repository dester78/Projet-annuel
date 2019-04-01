cmake_minimum_required(VERSION 3.10)
project(CScan)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
include_directories(include)
#include_directories(include/curl-7.64.0-win32-mingw)
link_directories(C:\\msys64\\mingw64\\x86_64-w64-mingw32\\lib )


file(
        GLOB_RECURSE
        source_files
        src/main.c
        src/curlHandler.c
        src/fileHandler.c
        src/jsonHandler.c
        src/gtkHandler.c
        src/gtkCallbacks.c
        src/jsonReader.c
        src/jsonWriter.c
        src/convertcharHandler.c

)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
PKG_CHECK_MODULES(CURL REQUIRED libcurl)

SET(${VERBOSE} 1)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${INTL_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS}  ${CURL_LIBRARY_DIRS} ${INTL_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER}  ${CURL_CFLAGS_DIRS})


add_executable(
        CScan
        ${source_files}
)

target_link_libraries(
        CScan
)

TARGET_LINK_LIBRARIES(CScan ${GTK3_LIBRARIES} ${CURL_LIBRARIES})

target_compile_options(CScan PRIVATE -Wall -Wextra -pedantic  )
#
#
#
#FIND_PROGRAM(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)
#FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
#FIND_PROGRAM(XGETTEXT_EXECUTABLE xgettext)
#
#IF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )
#        SET(GETTEXT_FOUND TRUE)
#ELSE (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE)
#        SET(GETTEXT_FOUND FALSE)
#        IF (GetText_REQUIRED)
#                MESSAGE(FATAL_ERROR "GetText not found")
#        ENDIF (GetText_REQUIRED)
#ENDIF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )
#IF(XGETTEXT_EXECUTABLE)
#        SET(XGETTEXT_FOUND TRUE)
#ELSE(XGETTEXT_EXECUTABLE)
#        MESSAGE(STATUS "xgettext not found.")
#        SET(XGETTTEXT_FOUND FALSE)
#ENDIF(XGETTEXT_EXECUTABLE)
#IF(NOT DEFINED XGETTEXT_OPTIONS)
#        SET(XGETTEXT_OPTIONS --language=Glade --keyword=translatable -s)
#ENDIF(NOT DEFINED XGETTEXT_OPTIONS)
#IF(XGETTEXT_FOUND)
#        MACRO(GETTEXT_CREATE_POT _potFile _pot_options )
#
#                SET(_xgettext_options_list)
#                SET(_src_list)
#                SET(_src_list_abs)
#                SET(_stage "SRC")
#                FOREACH(_pot_option ${_pot_options} ${ARGN})
#                    MESSAGE("TOTO")
#                        IF(_pot_option STREQUAL "OPTION")
#                                SET(_stage "OPTION")
#                        ELSEIF(_pot_option STREQUAL "SRC")
#                                SET(_stage "SRC")
#                        ELSE(_pot_option STREQUAL "OPTION")
#                                IF(_stage STREQUAL "OPTION")
#                                        SET(_xgettext_options_list ${_xgettext_options_list} ${_pot_option})
#                                ELSE(_stage STREQUAL "OPTION")
#                                        FILE(RELATIVE_PATH _relFile ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${_pot_option})
#                                        GET_FILENAME_COMPONENT(_absFile ${_pot_option} ABSOLUTE)
#                                        SET(_src_list ${_src_list} ${_relFile})
#                                        SET(_src_list_abs ${_src_list_abs} ${_absFile})
#                                ENDIF(_stage STREQUAL "OPTION")
#                        ENDIF(_pot_option STREQUAL "OPTION")
#                ENDFOREACH(_pot_option ${_pot_options} ${ARGN})
#                IF (_xgettext_options_list)
#                        SET(_xgettext_options ${_xgettext_options_list})
#                ELSE(_xgettext_options_list)
#                        SET(_xgettext_options ${XGETTEXT_OPTIONS})
#                ENDIF(_xgettext_options_list)
#                    MESSAGE (OUTPUT ${_potFile}
#                        COMMAND ${XGETTEXT_EXECUTABLE} ${_xgettext_options} -o ${_potFile} ${_src_list}
#                        DEPENDS ${_src_list_abs}
#                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#                #MESSAGE("${XGETTEXT_EXECUTABLE} ${_xgettext_options_list} -o ${_potFile} ${_src_list}")
#                ADD_CUSTOM_COMMAND(OUTPUT ${_potFile}
#                        COMMAND ${XGETTEXT_EXECUTABLE} ${_xgettext_options} -o ${_potFile} ${_src_list}
#                        DEPENDS ${_src_list_abs}
#                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                        )
#                ADD_CUSTOM_TARGET(pot_file
#                        COMMAND ${XGETTEXT_EXECUTABLE} ${_xgettext_options_list} -o ${_potFile} ${_src_list}
#                        DEPENDS ${_src_list_abs}
#                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                        COMMENT "Extract translatable messages to ${_potFile}"
#                        )
#        ENDMACRO(GETTEXT_CREATE_POT _potFile _pot_options)
#
#        MACRO(GETTEXT_CREATE_TRANSLATIONS _potFile _firstLang)
#                SET(_gmoFiles)
#                GET_FILENAME_COMPONENT(_potBasename ${_potFile} NAME_WE)
#                GET_FILENAME_COMPONENT(_absPotFile ${_potFile} ABSOLUTE)
#                SET(_addToAll)
#                SET(_is_comment FALSE)
#                FOREACH (_currentLang ${_firstLang} ${ARGN})
#
#                        IF(_currentLang STREQUAL "ALL")
#                                SET(_addToAll "ALL")
#                        ELSEIF(_currentLang STREQUAL "COMMENT")
#                                SET(_is_comment TRUE)
#                        ELSEIF(_is_comment)
#                                SET(_is_comment FALSE)
#                                SET(_comment ${_currentLang})
#                        ELSE()
#                                SET(_lang ${_currentLang})
#                                GET_FILENAME_COMPONENT(_absFile ${_currentLang}.po ABSOLUTE)
#                                GET_FILENAME_COMPONENT(_abs_PATH ${_absFile} PATH)
#                                SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)
#                                #MESSAGE("_absFile=${_absFile} _abs_PATH=${_abs_PATH} _lang=${_lang} curr_bin=${CMAKE_CURRENT_BINARY_DIR}")
#                                ADD_CUSTOM_COMMAND(
#                                        OUTPUT ${_gmoFile}
#                                        COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --quiet --update --backup=none -s ${_absFile} ${_absPotFile}
#                                        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_absFile}
#                                        DEPENDS ${_absPotFile} ${_absFile}
#                                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                                )
#                                INSTALL(FILES ${_gmoFile} DESTINATION share/locale/${_lang}/LC_MESSAGES RENAME ${_potBasename}.mo)
#                                SET(_gmoFiles ${_gmoFiles} ${_gmoFile})
#                        ENDIF()
#                ENDFOREACH (_currentLang )
#                IF(DEFINED _comment)
#                        ADD_CUSTOM_TARGET(translations ${_addToAll} DEPENDS ${_gmoFiles} COMMENT ${_comment})
#                ELSE(DEFINED _comment)
#                        ADD_CUSTOM_TARGET(translations ${_addToAll} DEPENDS ${_gmoFiles})
#                ENDIF(DEFINED _comment)
#        ENDMACRO(GETTEXT_CREATE_TRANSLATIONS )
#ENDIF(XGETTEXT_FOUND)
#
#
#GETTEXT_CREATE_POT("po/fr.pot" "glade/*.glade")




#
#
#
#FIND_PACKAGE (Intl REQUIRED)
#FIND_PACKAGE(Gettext REQUIRED)
#FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
#FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
#
#
#
#
#
#IF(NOT GETTEXT_MSGFMT_EXECUTABLE)
#    MESSAGE("------
#    NOTE: msgfmt not found. Translations will *not* be installed
#------")
#ELSE(NOT GETTEXT_MSGFMT_EXECUTABLE)
#
#    SET(catalogname rkward)
#
#    FILE(GLOB PO_FILES po/*.po)
#    SET(GMO_FILES)
#
#    FOREACH(_poFile ${PO_FILES})
#        GET_FILENAME_COMPONENT(_poFileName ${_poFile} NAME)
#        STRING(REGEX REPLACE "^${catalogname}_?" "" _langCode ${_poFileName} )
#        STRING(REGEX REPLACE "\\.po$" "" _langCode ${_langCode} )
#
#        IF( _langCode )
#            GET_FILENAME_COMPONENT(_lang ${_poFile} NAME_WE)
#            SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)
#
#            ADD_CUSTOM_COMMAND(OUTPUT ${_gmoFile}
#                    COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_poFile}
#                    DEPENDS ${_poFile})
#            INSTALL(FILES ${_gmoFile} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${catalogname}.mo)
#            LIST(APPEND GMO_FILES ${_gmoFile})
#        ENDIF( _langCode )
#
#    ENDFOREACH(_poFile ${PO_FILES})
#
#    ADD_CUSTOM_TARGET(translations ALL DEPENDS ${GMO_FILES})
#
#ENDIF(NOT GETTEXT_MSGFMT_EXECUTABLE)


#if (DEFINED GettextTranslate_ALL)
#set(_addToALL "ALL")
#endif()
#
#add_custom_target(update-po)
#add_custom_target(update-gmo ${_addToALL})
#
##look for all the programs
##xgettext, msginit, msgfilter, msgconv, msgmerge, msgfmt
#
#function(REQUIRE_BINARY binname varname)
#        if (${${varname}} STREQUAL "${varname}-NOTFOUND")
#                message(FATAL_ERROR "Could not find " ${binname})
#        endif()
#endfunction()
#
#find_program(GettextTranslate_XGETTEXT_EXECUTABLE xgettext
#HINTS ${GettextTranslate_XGETTEXT} ${GettextTranslate_BINARIES}
#)
#REQUIRE_BINARY(xgettext GettextTranslate_XGETTEXT_EXECUTABLE)
#
#find_program(GettextTranslate_MSGINIT_EXECUTABLE msginit
#HINTS ${GettextTranslate_MSGINIT} ${GettextTranslate_BINARIES}
#)
#REQUIRE_BINARY(msginit GettextTranslate_MSGINIT_EXECUTABLE)
#
#find_program(GettextTranslate_MSGFILTER_EXECUTABLE msgfilter
#HINTS ${GettextTranslate_MSGFILTER} ${GettextTranslate_BINARIES}
#)
#REQUIRE_BINARY(msgfilter GettextTranslate_MSGFILTER_EXECUTABLE)
#
#find_program(GettextTranslate_MSGCONV_EXECUTABLE msgconv
#HINTS ${GettextTranslate_MSGCONV} ${GettextTranslate_BINARIES}
#)
#REQUIRE_BINARY(msgconv GettextTranslate_MSGCONV_EXECUTABLE)
#
#find_program(GettextTranslate_MSGMERGE_EXECUTABLE msgmerge
#HINTS ${GettextTranslate_MSGMERGE} ${GettextTranslate_BINARIES}
#)
#REQUIRE_BINARY(msgmerge GettextTranslate_MSGMERGE_EXECUTABLE)
#
#find_program(GettextTranslate_MSGFMT_EXECUTABLE msgfmt
#HINTS ${GettextTranslate_MSGFMT} ${GettextTranslate_BINARIES}
#)
#REQUIRE_BINARY(msgfmt GettextTranslate_MSGFMT_EXECUTABLE)
#
#mark_as_advanced(
#GettextTranslate_MSGCONV_EXECUTABLE
#GettextTranslate_MSGFILTER_EXECUTABLE
#GettextTranslate_MSGFMT_EXECUTABLE
#GettextTranslate_MSGINIT_EXECUTABLE
#GettextTranslate_MSGMERGE_EXECUTABLE
#GettextTranslate_XGETTEXT_EXECUTABLE
#)
#
#macro( GettextTranslate)
#
#    if(GettextTranslate_GMO_BINARY)
#            set (GMO_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
#    else()
#            set (GMO_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#    endif()
#
#    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/POTFILES.in)
#            message(FATAL_ERROR "There is no POTFILES.in in
#            ${CMAKE_CURRENT_SOURCE_DIR}")
#    endif()
#
#    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Makevars)
#            message(FATAL_ERROR "There is no Makevars in ${CMAKE_CURRENT_SOURCE_DIR}")
#    endif()
#
#    file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/Makevars makevars
#    REGEX "^[^=]+=(.*)$"
#    )
#
#    foreach(makevar ${makevars})
#        string(REGEX REPLACE "^([^= ]+) =[ ]?(.*)$" "\\1" MAKEVAR_KEY ${makevar})
#        string(REGEX REPLACE "^([^= ]+) =[ ]?(.*)$" "\\2"
#        MAKEVAR_${MAKEVAR_KEY} ${makevar})
#    endforeach()
#
#    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/POTFILES.in
#    ${CMAKE_CURRENT_BINARY_DIR}/POTFILES
#    COPYONLY
#    )
#
#    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LINGUAS
#    ${CMAKE_CURRENT_BINARY_DIR}/LINGUAS
#    COPYONLY
#    )
#
#    #set the directory to not clean
#    #set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    #  PROPERTY CLEAN_NO_CUSTOM true)
#
#    file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/POTFILES.in potfiles
#    REGEX "^[^#].*"
#    )
#    MESSAGE("TEST")
#    foreach(potfile ${potfiles})
#        list(APPEND source_translatable
#        ${CMAKE_CURRENT_SOURCE_DIR}/${MAKEVAR_top_builddir}/${potfile})
#
#    endforeach()
#
#    set(TEMPLATE_FILE ${MAKEVAR_DOMAIN}.pot)
#    set(TEMPLATE_FILE_ABS ${CMAKE_CURRENT_SOURCE_DIR}/${TEMPLATE_FILE})
#    string(REGEX MATCHALL "[^ ]+" XGETTEXT_OPTS ${MAKEVAR_XGETTEXT_OPTIONS})
#    #add_custom_target(${MAKEVAR_DOMAIN}.pot-update DEPENDS
#    #  ${TEMPLATE_FILE_ABS}
#    #)
#
#    add_custom_target(${MAKEVAR_DOMAIN}.pot-update
#    COMMAND ${GettextTranslate_XGETTEXT_EXECUTABLE} ${XGETTEXT_OPTS}
#    -o ${TEMPLATE_FILE_ABS}
#    --default-domain=${MAKEVAR_DOMAIN}
#    --add-comments=TRANSLATORS:
#    --copyright-holder=${MAKEVAR_COPYRIGHT_HOLDER}
#    --msgid-bugs-address="${MAKEVAR_MSGID_BUGS_ADDRESS}"
#    --directory=${MAKEVAR_top_builddir}
#    --files-from=${CMAKE_CURRENT_BINARY_DIR}/POTFILES
#    --package-version=${VERSION}
#    --package-name=${CMAKE_PROJECT_NAME}
#    DEPENDS ${source_translatable}
#    ${CMAKE_CURRENT_SOURCE_DIR}/POTFILES.in
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    )
#
#    #add_custom_command(OUTPUT ${TEMPLATE_FILE_ABS}
#    #  COMMAND ${GettextTranslate_XGETTEXT_EXECUTABLE} ${XGETTEXT_OPTS}
#    #    -o ${TEMPLATE_FILE_ABS}
#    #    --default-domain=${MAKEVAR_DOMAIN}
#    #    --add-comments=TRANSLATORS:
#    #    --copyright-holder=${MAKEVAR_COPYRIGHT_HOLDER}
#    #    --msgid-bugs-address="${MAKEVAR_MSGID_BUGS_ADDRESS}"
#    #    --directory=${MAKEVAR_top_builddir}
#    #    --files-from=${CMAKE_CURRENT_BINARY_DIR}/POTFILES
#    #    --package-version=${VERSION}
#    #    --package-name=${CMAKE_PROJECT_NAME}
#    #  DEPENDS ${source_translatable}
#    #  ${CMAKE_CURRENT_SOURCE_DIR}/POTFILES.in
#    #  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    #)
#
#    #add_dependencies(update-po ${MAKEVAR_DOMAIN}.pot-update)
#
#    file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/LINGUAS LINGUAS
#    REGEX "^[^#].*")
#    string(REGEX MATCHALL "[^ ]+" languages ${LINGUAS})
#
#    foreach(lang ${languages})
#        set(PO_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/${lang}.po")
#        set(GMO_FILE_NAME "${GMO_BUILD_DIR}/${lang}.gmo")
#        set(PO_TARGET "generate-${MAKEVAR_DOMAIN}-${lang}-po")
#        set(GMO_TARGET "generate-${MAKEVAR_DOMAIN}-${lang}-gmo")
#        list(APPEND po_files ${PO_TARGET})
#        list(APPEND gmo_files ${GMO_TARGET})
#
#        if(${lang} MATCHES "en@(.*)quot")
#
#            add_custom_command(OUTPUT ${lang}.insert-header
#            COMMAND
#            sed -e "'/^#/d'" -e 's/HEADER/${lang}.header/g'
#            ${CMAKE_CURRENT_SOURCE_DIR}/insert-header.sin > ${lang}.insert-header
#            )
#
#            #generate the en@quot files
#            add_custom_target(${PO_TARGET}
#            COMMAND
#            ${GettextTranslate_MSGINIT_EXECUTABLE} -i ${TEMPLATE_FILE_ABS}
#            --no-translator -l ${lang}
#            -o - 2>/dev/null
#            | sed -f ${CMAKE_CURRENT_BINARY_DIR}/${lang}.insert-header
#            | ${GettextTranslate_MSGCONV_EXECUTABLE} -t UTF-8
#            | ${GettextTranslate_MSGFILTER_EXECUTABLE} sed -f
#            ${CMAKE_CURRENT_SOURCE_DIR}/`echo ${lang}
#            | sed -e 's/.*@//'`.sed 2>/dev/null >
#            ${PO_FILE_NAME}
#            DEPENDS ${lang}.insert-header ${TEMPLATE_FILE_ABS}
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#            )
#
#        else()
#
#            add_custom_target(${PO_TARGET}
#            COMMAND ${GettextTranslate_MSGMERGE_EXECUTABLE} --lang=${lang}
#            ${PO_FILE_NAME} ${TEMPLATE_FILE_ABS}
#            -o ${PO_FILE_NAME}.new
#            COMMAND mv ${PO_FILE_NAME}.new ${PO_FILE_NAME}
#            DEPENDS ${TEMPLATE_FILE_ABS}
#            )
#
#        endif()
#
#        add_custom_target(${GMO_TARGET}
#        COMMAND ${GettextTranslate_MSGFMT_EXECUTABLE} -c --statistics --verbose
#        -o ${GMO_FILE_NAME} ${PO_FILE_NAME}
#        DEPENDS ${PO_TARGET}
#        )
#
#        add_dependencies(${PO_TARGET} ${MAKEVAR_DOMAIN}.pot-update)
#
#        install(FILES ${GMO_FILE_NAME} DESTINATION
#        ${LOCALEDIR}/${lang}/LC_MESSAGES
#        RENAME ${MAKEVAR_DOMAIN}.mo
#        )
#
#    endforeach()
#
#    add_dependencies(update-po ${po_files})
#    add_dependencies(update-gmo ${gmo_files})
#
#    #string(REGEX MATCH "^[^=]+=(.*)$" parsed_variables ${makevars})
#
#endmacro()
#
#


get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
endforeach()











