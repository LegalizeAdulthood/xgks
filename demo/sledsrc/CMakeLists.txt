add_executable(sled
    add_objects.c
    add_objects.h
    add_obj_func.h
    add_obj_subs.c
    adjust_grid.h
    attributes.h
    changes.h
    clr_sel_draw.c
    clr_sel_draw.h
    clr_selector.h
    clr_sel_exec.c
    clr_sel_func.h
    clr_sel_key.h
    color.c
    color.h
    conversions.c
    conversions.h
    defs.h
    draw.c
    draw.h
    draw_menu.c
    draw_menu.h
    draw_palette.c
    dummies.c
    edit_picture.c
    edit_picture.h
    error.c
    error.h
    func.h
    functbl.c
    functbl.h
    grid.c
    init_tbls.c
    input.c
    io.c
    io.h
    key.h
    main.c
    main_menu.c
    main_menu.h
    menu.c
    menu.h
    obj_detect.c
    obj_detect.h
    object_defs.h
    object_list.c
    object_list.h
    objects_copy.c
    objects_draw.c
    objects_draw.h
    objects_free.c
    objects_func.h
    objects.h
    object_tbl.c
    object_tbl.h
    obj_rotate.c
    palette.c
    palette.h
    photodummy.c
    photo_func.h
    picture.c
    popup.c
    popup.h
    prompt.c
    prompt.h
    read_input.c
    read_menu.c
    screen_items.c
    screen_items.h
    trans.c
    trans.h
    trans_subs.c
    trans_subs.h
    undo.c
    write_output.c
    ws.h
    )
target_link_libraries(sled PUBLIC xgks)
target_folder(sled "Demos")
