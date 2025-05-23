find_package(X11 REQUIRED)

include(CheckLibraryExists)
check_library_exists(m sin "" HAVE_LIB_M)
set(LIBM)
if(HAVE_LIB_M)
    set(LIBM m)
endif()

add_library(xgks
    act_ws.c
    aspect_flags.c
    cellarray.c
    choice.c
    colours.c
    deferral_ws.c
    escape.c
    event.c 
    fillarea.c
    gdp.c
    gerrorhand.c
    gks_error.c
    Imakefile
    input.c
    inqfillareas.c
    inqpixel.c
    inqpmarker.c
    inqpolylines.c
    inqtext.c
    inqtransform.c
    inquiries.c
    inqWDT.c
    locator.c
    Makefile.AIX
    message.c
    metafile.c
    open_gks.c
    open_ws.c
    pick.c
    polylines.c
    polymarkers.c
    prmgr.c
    segments.c
    string.c
    stroke.c
    text.c
    transforms.c
    update.c
    valuator.c
    x/xcellarray.c
    x/xcolours.c
    x/xevent.c
    x/xfillarea.c
    x/xinqpixel.c
    x/xopws.c
    x/xpline.c
    x/xpmarker.c
    x/xSet.c
    x/xtext.c
    x/xupdate.c
    ../../include/event.h
    ../../include/fillarea.h
    ../../include/font.h
    ../../include/gks_defines.h
    ../../include/gkserrmsg.h
    ../../include/gks_errors.h
    ../../include/gks_implem.h
    ../../include/gkslist.h
    ../../include/input.h
    ../../include/metafile.h
    ../../include/polylines.h
    ../../include/polymarkers.h
    ../../include/primitive.h
    ../../include/text.h
    ../../include/wdt.h
    ../../include/wslist.h
    ../../include/xgks.h
    ../../include/x/colours.h
    ../../include/x/xevent.h
    )
target_compile_definitions(xgks PRIVATE "FONTDBDIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../../fontdb\"")
target_include_directories(xgks PUBLIC ../../include ${X11_INCLUDE_DIR})
target_link_libraries(xgks PUBLIC ${X11_LIBRARIES} ${LIBM})
target_folder(xgks "Libraries")

add_executable(mkerrmsg mkerrmsg.c)
target_folder(mkerrmsg "Tools")
