/*
 *              Copyright IBM Corporation 1989
 *
 *                      All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 *  this file includes routines for menu items and popups associated
 *  with main menu:
 *      quit
 *      up
 *      update
 *      retrieve
 *      save
 *      create_pic_file
 *      init_update_popup
 *      switch_update_state
 *      init_verbose_popup
 *      switch_verbose_state
 *      get_verbose_mode
 *      init_grid_popup
 *      switch_grid_state
 *      get_grid_state
 */

#include <xgks.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "color.h"
#include "defs.h"
#include "draw.h"
#include "draw_menu.h"
#include "functbl.h"
#include "grid.h"
#include "key.h"
#include "menu.h"
#include "popup.h"
#include "prompt.h"
#include "read_input.h"
#include "screen_items.h"
#include "trans.h"
#include "write_output.h"
#include "ws.h"

static char *last_file_accessed;

/*
 *  quit
 *
 *  description:        init-exec routine for menu item QUIT
 *
 *  parameters:         menu_item (MENU_ITEM *) - QUIT  menu item
 */

void quit(MENU_ITEM *menu_item)
{
    signal_done();
    prompt(3);
    currmitem = (MENU_ITEM *) NULL;

} /* end quit */

/*
 * up
 *
 *  description:        replaces sec_menu with parent sec_menu
 *                      as long as parent is not primary menu
 *
 *  parameters:         menu_item (MENU_ITEM *) - UP menu item (unused)
 */

void up(MENU_ITEM *menu_item)
{
    MENU *pri_menu;
    MENU *sec_menu;

    pri_menu = get_menu_from_area(PRI_MENU_AREA);
    sec_menu = get_menu_from_area(SEC_MENU_AREA);
    if ((sec_menu->parent != pri_menu) && (sec_menu != (MENU *) NULL))
        switch_sec_menu(sec_menu->parent);
    else
        reprompt(1);
    currmitem = (MENU_ITEM *) NULL;

} /* end up */

/*
 *  update
 *
 *  description:        updates picture area by redrawing objects on
 *                      screen
 *
 *  parameters:         menu_item (MENU_ITEM *) - update menu_item
 */

void update(MENU_ITEM *menu_item)
{
    IDX idx;

    idx = find_screen_tbl_idx(PICT_AREA);
    if (screen_tbl[idx].is_active == FALSE)
    {
        full_prompt(1, 2);
        return;
    }
    hilite(menu_item->key);
    erase_area(PICT_AREA);
    (*(screen_tbl[idx].draw))();
    unhilite(menu_item->key);
    prompt(2);
} /* end update */

/*
 *  create_pic_file
 *
 *  description:        creates PIC description of picture
 *
 *  parameters:         menu_item (MENU_ITEM *) - create pic file
 *                      menu item
 *                      Not used in this new GKS version.
 */

void create_pic_file(MENU_ITEM *menu_item)
{
    /* Gpoint window[4];
        Gpoint box[4];
        char ln[80],command[100];
        IDX idx; */

    /* Function currently not in use; all we do now is acknowledge request: */

    hilite(menu_item->key);
    reprompt(0);
    unhilite(menu_item->key);

#ifdef PIC_FILE_IMPLEMENTED

    /* if picture_area is not active or no objects in picture
           then don't create pic file */

    if (no_objects() == TRUE)
    {
        reprompt(40);
        return;
    }
    idx = find_screen_tbl_idx(PICT_AREA);
    if (screen_tbl[idx].is_active == FALSE)
    {
        reprompt(1);
        return;
    }

    hilite(menu_item->key);
    prompt(41);
    push_curr_trans();
    activate(PICT_AREA);

    /* draw background and window outline */

    set_aspect_flags(GBUNDLED);

    get_viewport(PICT_AREA, window);
    set_window(PICT_AREA, window);
    load_box(box, window[MIN].x, window[MAX].x,
        window[MIN].y, window[MAX].y);
    gsetfillind(BGDCLR_SOLID);
    gfillarea(4, box);
    gsetfillind(WHITE_HOLLOW);
    gfillarea(4, box);

    pop_curr_trans();
    gopenws(pic_wsid, hostname, PIC_WS);
    gactivatews(pic_wsid);

    set_window(PICT_AREA, window);
    activate(PICT_AREA);

    set_aspect_flags(GINDIVIDUAL);

    /* draw picture */

    draw_objects();

    gdeactivatews(pic_wsid);
    gclosews(pic_wsid);

    pop_curr_trans();

    if (get_grid_state() != GRID_OFF)
        draw_grid();

    /* move pic file to user specified file */

    printf("enter name of PIC file: ");
    prompt(35);
    fgets(ln, sizeof(ln), stdin);
    if (ln[0] != 0)
    {
        strcpy(command, "mv uigks.pic ");
        strcat(command, ln);
        system(command);
    }
    unhilite(menu_item->key);
    prompt(2);
#endif

} /* end create_pic_file */

/*
 *  retrieve
 *
 *  description:        retrieves picture description
 *
 *  parameters:         menu_item (MENU_ITEM *) - retrieve menu_item
 */

void retrieve(MENU_ITEM *menu_item)
{
    char ln[80];

    hilite(menu_item->key);

    /* prompt user for filename */

    prompt(35);
    if (last_file_accessed != (char *) NULL)
    {
        printf("(hit <cr> to retrieve picture description in %s)\n",
            last_file_accessed);
    }
    printf("Enter filename: ");

    /* if no file has been specified, then return */

    fgets(ln, sizeof(ln), stdin);
    if ((ln[0] == 0) && (last_file_accessed == (char *) NULL))
    {
        unhilite(menu_item->key);
        return;
    }

    /* set last file accessed to specified file */

    if (ln[0] != 0)
    {
        if (last_file_accessed != (char *) NULL)
            free(last_file_accessed);
        last_file_accessed = (char *) calloc((unsigned) (strlen(ln) + 1), sizeof(char));
        (void) strcpy(last_file_accessed, ln);
    }

    /* retrieve picture description */

    if (read_picture(last_file_accessed))
    {
        erase_area(PICT_AREA);
        (*(screen_tbl[find_screen_tbl_idx(PICT_AREA)].draw))();
        prompt(2);
    }

    unhilite(menu_item->key);
} /* end retrieve */

/*
 *  save
 *
 *  description:        saves picture description
 *
 *  parameters:         menu_item (MENU_ITEM *) - save menu_item
 */

void save(MENU_ITEM *menu_item)
{
    char ln[80];

    hilite(menu_item->key);

    /* prompt user for filename */

    prompt(35);
    if (last_file_accessed != (char *) NULL)
    {
        printf("(hit <cr> to save picture description in %s)\n",
            last_file_accessed);
    }
    printf("Enter filename: ");

    /* if no file specfied, return */

    fgets(ln, sizeof(ln), stdin);
    if ((ln[0] == 0) && (last_file_accessed == (char *) NULL))
    {
        unhilite(menu_item->key);
        return;
    }

    /* update last file accessed */

    if (ln[0] != 0)
    {
        if (last_file_accessed != (char *) NULL)
            free(last_file_accessed);
        last_file_accessed = (char *) calloc((unsigned) (strlen(ln) + 1), sizeof(char));
        (void) strcpy(last_file_accessed, ln);
    }

    /* save picture description */

    if (write_picture(last_file_accessed))
        prompt(2);

    unhilite(menu_item->key);
} /* end save */

/*
 *  the following data structures and routines correspond to
 *  the three popups associated with the main menu, namely
 *  the VERBOSE popup, UPDATE popup and SNAP popup
 */

static Gint update_state = FULL_UPDATE;

static char *update_text[] = {
    "QUICK",
    "UPDATE",
    "FULL",
    "UPDATE",
    "FINAL",
    "UPDATE"
};

/*
 *  init_update_popup
 *
 *  description:        displays popup box and text
 *
 *  parameters:         popup (POPUP *) - update popup
 */

void init_update_popup(POPUP *popup)
{
    char **ln_ptr;
    int i;

    ln_ptr = update_text;
    for (i = 0; i < (2 * update_state); i++)
        ln_ptr++;
    display_popup(popup, 2, ln_ptr);

} /* end init_update_popup */

/*
 *  switch_update_state
 *
 *  description:        advances popup state to next state and
 *                      redraws popup
 *
 *  parameters:         popup (POPUP *) - update popup
 *                      pt (Gpoint) - unused
 */

void switch_update_state(POPUP *popup, Gpoint pt)
{
    switch_popup_state(&update_state, 2, 2, update_text, popup->extent);
} /* end switch_update_state */

/*
 *  get_update_mode
 *
 *  description:        finds current value of update_state
 *
 *  parameters:         none
 *
 *  returns:            (Gint) - update_state
 */

int get_update_mode(void)
{
    Gint mode;

    mode = update_state;

    return (mode);
} /* end get_update_mode */

static Gint verbose_state = VERBOSE_MODE_ON;

static char *verbose_text[] = {
    "VERBOSE",
    "MODE",
    "OFF",
    "VERBOSE",
    "MODE",
    "ON"
};

/*
 *  init_verbose_popup
 *
 *  description:        displays popup box and text
 *
 *  parameters:         popup (POPUP *) - verbose popup
 */

void init_verbose_popup(POPUP *popup)
{
    char **ln_ptr;
    int i;

    ln_ptr = verbose_text;
    for (i = 0; i < (3 * verbose_state); i++)
        ln_ptr++;
    display_popup(popup, 3, ln_ptr);

} /* end init_verbose_popup */

/*
 *  switch_verbose_state
 *
 *  description:        advances popup state to next state and
 *                      redraws popup
 *
 *  parameters:         popup (POPUP *) - verbose popup
 *                      pt (Gpoint) - unused
 */

void switch_verbose_state(POPUP *popup, Gpoint pt)
{
    switch_popup_state(&verbose_state, 2, 3, verbose_text,
        popup->extent);

    switch (verbose_state)
    {
    case VERBOSE_MODE_ON:
        restore_msg_area();
        break;

    case VERBOSE_MODE_OFF:
        rm_msg_area();
        break;
    }

} /* end switch_verbose_state */

/*
 *  get_verbose_mode
 *
 *  description:        finds current value of verbose_state
 *
 *  parameters:         none
 *
 *  returns:            (Gint) - verbose_state
 */

int get_verbose_mode(void)
{
    return (verbose_state);
} /* end get_verbose_mode */

static Gint grid_state = GRID_OFF;

static char *grid_text[] = {
    "GRID",
    "OFF",
    "GRID ON",
    "SNAP OFF",
    "GRID ON",
    "SNAP ON"
};

/*
 *  init_grid_popup
 *
 *  description:        displays popup box and text
 *
 *  parameters:         popup (POPUP *) - grid popup
 */

void init_grid_popup(POPUP *popup)
{
    char **ln_ptr;
    int i;

    ln_ptr = grid_text;
    for (i = 0; i < (2 * grid_state); i++)
        ln_ptr++;
    display_popup(popup, 2, ln_ptr);

} /* end init_grid_popup */

/*
 *  switch_grid_state
 *
 *  description:        advances popup state to next state and
 *                      redraws popup
 *
 *  parameters:         popup (POPUP *) - grid popup
 *                      pt (Gpoint) - unused
 */

void switch_grid_state(POPUP *popup, Gpoint pt)
{
    IDX prev_grid_state;

    prev_grid_state = grid_state;
    switch_popup_state(&grid_state, 3, 2, grid_text, popup->extent);
    if (screen_tbl[find_screen_tbl_idx(PICT_AREA)].is_active)
    {
        if (prev_grid_state == GRID_OFF)
        {
            draw_grid();
        }
        else if (grid_state == GRID_OFF)
        {
            erase_grid();
        }
    }
} /* end switch_grid_state */

/*
 *  get_grid_state
 *
 *  description:        returns current value of grid_state
 *
 *  parameters:         none
 */

int get_grid_state(void)
{
    return (grid_state);
} /* end get_grid_state */
