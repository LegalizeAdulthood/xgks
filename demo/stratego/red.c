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
 */

#include <xgks.h>
#include <xgksesc.h>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pictures.h"
#include "stratego.h"

void startup(char *envp[]);
void initialize(void);
void setup(void);
void transmit_set_up(void);
int makemove(void);
void sendmove(void);
void drawmove(void);
int receivemove(void);
void lstartup(void);
void drawboard(int option);
void drawpiece(int x, int y, int col, int type);
void drawnum(Gfloat x, Gfloat y, int i, int col);
void setupcurpts(WCPT pts[5]);
void drawsquare(int x, int y, int col);
void clearboard(void);
void putdownpc(void);
void drawcastle(int x, int y, int col);
int quitgame(void);
int checkmove(int sx, int sy, int dx, int dy);
int findwinner(int a, int b);
void addpow(int rank);

static int win = FALSE;
static int mycolor = RED;
static int numleft[] = { 0, 1, 1, 2, 3, 4, 4, 4, 5, 8, 1, 6, 1 };
static char remote_dis[80];
static WCPT numloc = { 0.28, 0.65 };
static Gloc last = { 1, 5.0, 5.0 };
static Glocrec loc;
static Gcobundl co_flashon = { 1.0, 0.0, 0.0 },
                co_flashoff = { 1.0, 1.0, 1.0 };
static int pRtB[2]; /* Red to Blue pipe descriptors */
static int pBtR[2]; /* Blue to Red pipe descriptors */

int main(int argc, char *argv[], char *envp[])
{
    int transmit = TRUE; /* transmit or receive flag */
    int quit = FALSE;

    /* get the remote display name */
    if (argc > 1)
        strcpy(remote_dis, argv[1]);
    else
        strcpy(remote_dis, getenv("DISPLAY"));

/* start up blue */
#ifdef DEBUG
    printf("red: about to call startup...\n");
#endif
    startup(envp);
#ifdef DEBUG
    printf("red: past startup, mycolor = %d\n", mycolor);
#endif

    initialize();
#ifdef DEBUG
    printf("red: past initialize, mycolor = %d\n", mycolor);
#endif

    setup();
#ifdef DEBUG
    printf("red: past setup, mycolor = %d\n", mycolor);
#endif

    transmit_set_up();
#ifdef DEBUG
    printf("red: past transmit_set_up, mycolor = %d\n", mycolor);
#endif

    /* play */
    while (!win && !quit)
    {
        if (transmit)
        {
            gmessage(1, "red move");
            quit = makemove();
            sendmove();
            drawmove();

#ifndef LTEST
            transmit = FALSE;
#endif
        }
        else
        {
            gmessage(1, "blue move");
            quit = receivemove();
            drawmove();
            transmit = TRUE;
        }
    }

    /* game over and shut down */
    gdeactivatews(1);
    gclosews(1);
    gclosegks();
}

/* local-only start up -- remove after debugging  */
void lstartup(void)
{
    int i, j;

    drawboard(HIGH);
    for (i = 0; i < 10; ++i)
        for (j = 0; j < 4; ++j)
            drawpiece(i, j, board[i][j].color, board[i][j].rank);
#ifdef LTEST
    for (i = 0; i < 10; ++i)
        for (j = 6; j < 10; ++j)
            drawcastle(i, j, board[i][j].color);
#endif
}

/****   startup  ****/
/* startup performs the function of starting the blue program and creating a
   pipe between the two programs.  Red creates a pipe, forks, and then does
   an execve which replaces the child with the blue program.
*/
#ifndef LTEST
void startup(char *envp[])
{
    char progname[80],
        aparms[4][80];
    char *vparms[5];

    /* create path name for blue program */
    strcpy(progname, "blue");

    /* open the two pipes */
    pipe(pRtB); /* Red to Blue */
    pipe(pBtR); /* Blue to Red */

    /* prepare parameters to send to blue */
    strcpy(aparms[0], "blue");             /* program name */
    sprintf(aparms[1], "%d", pRtB[READ]);  /* number of read-descriptor */
    sprintf(aparms[2], "%d", pBtR[WRITE]); /* number of write-descriptor */
    strcpy(aparms[3], remote_dis);         /* remote display name */
    /* fill in parameter list */
    vparms[0] = (char *) aparms[0];
    vparms[1] = (char *) aparms[1];
    vparms[2] = (char *) aparms[2];
    vparms[3] = (char *) aparms[3];
    vparms[4] = (char *) NULL; /* mark end of parameter list */

    /* fork and replace child with blue */
    if (fork() == 0)
    {
        execve(progname, vparms, envp);
        /* if we return, we failed */
        fprintf(stderr, "can\'t start blue\n");
        exit(1);
    }
}
#endif

/****  initialize  ****/
/* initialize does the initialization for gks,windows,color table, board,
   fill style, and line type.
*/

void initialize(void)
{
    Glimit templim;
    Gcobundl tempcol;
    Glimit earea;
    Gtxfp temptxfp;
    XWMHints xwmhints;
    Display *xdpy;
    Window xwin;
    GC xgc;

#include "stratego.MASK"
#include "stratego.bitmap"

    /* initialize gks */
    gopengks((Gfile *) stderr, 0);
    gopenws(1, NULL, NULL);

    gescinqxattr(1, &xdpy, &xwin, &xgc);

    xwmhints.flags = IconPixmapHint | IconMaskHint;
    xwmhints.icon_mask = XCreatePixmapFromBitmapData(xdpy, xwin, stratego_mask_bits, stratego_mask_width, stratego_mask_height, 1, 0, 1);
    xwmhints.icon_pixmap = XCreatePixmapFromBitmapData(xdpy, xwin, stratego_bits, stratego_width, stratego_height, 0, 1, 1);
    XSetWMHints(xdpy, xwin, &xwmhints);

    XSetIconName(xdpy, xwin, "XGKS stratego");
    XStoreName(xdpy, xwin, "XGKS stratego");

    gactivatews(1);

    /* set windows */
    templim.xmin = 0.0;
    templim.xmax = 12.5;
    templim.ymin = 0.0;
    templim.ymax = 10.0;
    gsetwindow(1, &templim);
    templim.xmin = 0.0;
    templim.xmax = 1.0;
    templim.ymin = 0.0;
    templim.ymax = 1.0;
    gsetviewport(1, &templim);
    gselntran(1);

    /* prepare color table */
    tempcol.red = 0.0;
    tempcol.blue = 0.0;
    tempcol.green = 0.66;
    gsetcolorrep(1, GREEN, &tempcol);
    tempcol.red = 0.66;
    tempcol.blue = 0.0;
    tempcol.green = 0.0;
    gsetcolorrep(1, RED, &tempcol);
    tempcol.red = 0.0;
    tempcol.blue = 0.66;
    tempcol.green = 0.0;
    gsetcolorrep(1, BLUE, &tempcol);
    tempcol.red = 0.0;
    tempcol.blue = 0.0;
    tempcol.green = 0.0;
    gsetcolorrep(1, BLACK, &tempcol);
    tempcol.red = 0.0;
    tempcol.blue = 1.0;
    tempcol.green = 0.0;
    gsetcolorrep(1, L_BLUE, &tempcol);
    tempcol.red = 1.0;
    tempcol.blue = 1.0;
    tempcol.green = 1.0;
    gsetcolorrep(1, WHITE, &tempcol);
    gsetcolorrep(1, FLASH1, &co_flashoff);
    gsetcolorrep(1, FLASH2, &co_flashoff);

    /* initialize locator attributes */
    loc.pet3.data = NULL;

    /* set styles */
    gsetfillintstyle(GSOLID);
    gsetlinetype(1);
    temptxfp.font = 2;
    temptxfp.prec = GSTRING;
    gsettextfontprec(&temptxfp);

    /* setup locator */
    earea.xmin = 0.0;
    earea.xmax = 1023.0;
    earea.ymin = 0.0;
    earea.ymax = 1023.0;
    ginitloc(1, 1, &last, 3, &earea, &loc);
    gsetlocmode(1, 1, GREQUEST, GECHO);

    gsetviewportinputpri(1, 0, GHIGHER);
}

/**** set up *****/
/* start of routine that allows player to initialize the board with  */
/* the pieces he wants placed in the positions he wants              */

void setup(void)
{
    int i;
    WCPT at, pts[5];
    char string[100];
    Gtxalign txalgn;

    drawboard(LOW);
    for (i = 0; i < 10; ++i)
    {
        drawpiece(i, 8, mycolor, (i + 1));
        drawnum((i + 0.5), 9.5, numleft[i + 1], WHITE);
    }
    for (i = 0; i < 2; ++i)
    {
        drawpiece(i, 6, mycolor, (i + 11));
        drawnum((i + 0.5), 7.5, numleft[i + 11], WHITE);
    }
    setupcurpts(pts); /* draw 'current piece' box */
    gsetlinecolourind(BLUE);
    gsetlinewidth(0.3);
    gpolyline(5, pts);
    strcpy(string, "current piece");
    gsetcharheight(0.15);
    txalgn.hor = GTH_CENTRE;
    txalgn.ver = GTV_HALF;
    gsettextalign(&txalgn);
    gsettextcolourind(BLUE);
    at.x = 8.5;
    at.y = 5.7;
    gtext(&at, string);
    drawsquare(4, 6, BLUE);
    strcpy(string, "FINISHED");

    gsettextcolourind(WHITE);
    at.x = 4.5;
    at.y = 5.7;
    gtext(&at, string);
#ifndef NTEST
    clearboard(); /* clear the board */
#endif
    putdownpc(); /* call the routine that lets player put down pieces */
}

/* clearboard -- sets all the board to NO_PIECE */
void clearboard(void)
{
    int x, y;

    for (x = 0; x < 10; x++)
        for (y = 0; y < 10; y++)
        {
            board[x][y].rank = NO_PIECE;
            board[x][y].color = 0;
        }
    /* set lake areas of the board to a -1 rank */
    board[2][4].rank = board[2][5].rank = board[3][4].rank = board[3][5].rank = -1;
    board[6][4].rank = board[6][5].rank = board[7][4].rank = board[7][5].rank = -1;
}

/**** pick square *****/
/* this routine returns the integer value of the coordinates of the sqaure */
/* picked by the player  */

Gistat picksquare(int *x, int *y)
{
    Gistat status;
    Gqloc locrec;

    greqloc(1, 1, &locrec);
    last = locrec.loc;
    status = locrec.status;
    *x = (int) last.position.x;
    *y = (int) last.position.y;
    if (*x == 10)
        *x = 9;
    if (*y == 10)
        *y = 9; /* check for exception case at end of screen and round */
    return (status);
}

/*** draw castle  ****/
/* this routine draws a blank castle in any color, given x,y, and color */

void drawcastle(int x, int y, int col)
{
    int i;
    WCPT pts[CASTLEPNTS];

    gsetfillcolourind(col);
    for (i = 0; i < CASTLEPNTS; ++i)
    {
        pts[i].x = castle[i].x + x;
        pts[i].y = castle[i].y + y;
    }
    gfillarea(CASTLEPNTS, pts);
}

/*** draw piece ****/
/* this routine draws the actual piece faces on the blank castles, it uses */
/* drawcastle to create the castles */
/*
    int x, y;   lower left corner of piece
    int col;    color of piece
    int type;   rank of piece
*/
void drawpiece(int x, int y, int col, int type)
{
    WCPT transpc[15]; /* translated piece coordinates */
    int i;
    WCPT at;
    char ch[2];

    for (i = 0; i < 15; ++i)
    {
        transpc[i].x = pieces[type - 1][i].x + x;
        transpc[i].y = pieces[type - 1][i].y + y;
    }
    drawcastle(x, y, col);
    gsetlinecolourind(WHITE);
    gsetlinewidth(0.3);
    gpolyline(15, transpc);
    at.x = numloc.x + x;
    at.y = numloc.y + y;
    gsetcharheight(0.15);
    gsettextcolourind(WHITE);
    if (type < 10)
        ch[0] = '0' + type;
    if (type == SPY)
        ch[0] = 'S';
    if (type == BOMB)
        ch[0] = 'B';
    if (type == FLAG)
        ch[0] = 'F';
    ch[1] = '\0';
    gtext(&at, ch);
}

/* put down piece  */
/* this routine is the one which loops until the player is satisfied with */
/* the position of his pieces  */

void putdownpc(void)
{
    int x, y, picked, valid, dropped, finished, i, temp;

    picked = 0;
#ifdef NTEST
    finished = TRUE;
#else
    finished = FALSE;
#endif
    while (finished != TRUE)
    {
        valid = FALSE;
        while (valid != TRUE)
        {
            do
            {
#ifdef DEBUG
                printf("in do...\n");
#endif
                picksquare(&x, &y);

#ifdef DEBUG
                printf(" x and y are: %d %d\n", x, y);
#endif
                if ((y == 6) && (x == 4)) /* the player has picked "finished" */
                {
#ifdef DEBUG
                    printf("picked finish...\n");
#endif
                    temp = 0;
                    for (i = 1; i < 13; ++i)
                        temp += numleft[i];
                    if (temp == 0)
                    {
                        finished = TRUE;
                        valid = TRUE;
                        picked = 1; /* anything non-zero will do */
                    }
                }
                else if ((y < 4) && ((picked = board[x][y].rank) != 0))
                {
#ifdef DEBUG
                    printf("picked a piece on the board...\n");
#endif
                    valid = TRUE;
                    drawsquare(x, y, GREEN);
                    board[x][y].rank = NO_PIECE;
                }
                else if ((y == 8) && (numleft[x + 1] != 0))
                {
#ifdef DEBUG
                    printf("picked a piece from the top row...\n");
#endif
                    picked = x + 1;
                    drawnum((x + 0.5), 9.5, numleft[picked], BLACK);
                    numleft[picked] -= 1;
                    drawnum((x + 0.5), 9.5, numleft[picked], WHITE);
                    valid = TRUE;
                }
                else if ((y == 6) && (x < 2))
                {
                    if (numleft[x + 11] != 0)
                    {
#ifdef DEBUG
                        printf("picked a piece from the bottom row...\n");
#endif
                        picked = x + 11;
                        drawnum((x + 0.5), 7.5, numleft[picked], BLACK);
                        numleft[picked] -= 1;
                        drawnum((x + 0.5), 7.5, numleft[picked], WHITE);
                        valid = TRUE;
                    }
                }
            } while (picked == 0);
            if (finished != TRUE)
            {
#ifdef DEBUG
                printf("drawing piece in current box, picked = %d\n", picked);
#endif
                drawpiece(8, 6, mycolor, picked); /* draw in "current piece" box */
                dropped = FALSE;
            }
            else
                dropped = TRUE;
            while (dropped != TRUE)
            {
#ifdef DEBUG
                printf("in drop loop...\n");
#endif
                picksquare(&x, &y);
#ifdef DEBUG
                printf("x, y = %d, %d\n", x, y);
#endif
                if (y < 4)
                {
#ifdef DEBUG
                    printf("dropping on board...\n");
#endif
                    if (board[x][y].rank == NO_PIECE)
                    {
#ifdef DEBUG
                        printf("empty space...\n");
#endif
                        board[x][y].rank = picked;
                        board[x][y].color = mycolor;
                        drawsquare(8, 6, BLACK);
                        dropped = TRUE;
                        drawpiece(x, y, mycolor, picked);
                        picked = 0;
#ifdef DEBUG
                        printf("back from drawpiece, should go back to do...\n");
#endif
                    }
                    else /* square already occupied by another piece */
                    {
#ifdef DEBUG
                        printf("occupied space...\n");
#endif
                        temp = board[x][y].rank;
                        board[x][y].rank = picked;
                        board[x][y].color = mycolor;
                        drawpiece(x, y, mycolor, picked);
                        drawpiece(8, 6, mycolor, temp);
                        picked = temp;
                    }
                }
            }
        }
    }
#ifdef DEBUG
    printf("calling drawboard...\n");
#endif
    drawboard(HIGH);
}

/** draw number **/
/* this routine draws a number at a specified x,y coordinate */

void drawnum(Gfloat x, Gfloat y, int i, int col)
{
    WCPT at;
    char ch[2];
    Gtxalign txalgn;

    at.x = x;
    at.y = y;
    gsettextcolourind(col);
    gsetcharheight(0.5);
    txalgn.hor = GTH_CENTRE;
    txalgn.ver = GTV_HALF;
    gsettextalign(&txalgn);
    ch[0] = '0' + i;
    ch[1] = '\0';
    gtext(&at, ch);
}

/* set up "current piece" box */
void setupcurpts(WCPT pts[5])
{
    pts[0].x = 7.9;
    pts[0].y = 5.9;
    pts[1].x = 7.9;
    pts[1].y = 7.1;
    pts[2].x = 9.1;
    pts[2].y = 7.1;
    pts[3].x = 9.1;
    pts[3].y = 5.9;
    pts[4].x = 7.9;
    pts[4].y = 5.9;
}

/* draw square : draws a sqaure of any color */
void drawsquare(int x, int y, int col)
{
    WCPT pts[5];

    pts[0].x = x + 0.05;
    pts[0].y = y + 0.05;
    pts[1].x = x + 0.05;
    pts[1].y = y + 0.95;
    pts[2].x = x + 0.95;
    pts[2].y = y + 0.95;
    pts[3].x = x + 0.95;
    pts[3].y = y + 0.05;
    pts[4].x = x + 0.05;
    pts[4].y = y + 0.05;
    gsetfillcolourind(col);
    gfillarea(5, pts);
}

/**** drawboard  ****/
/* drawboard draw part or all of the board depending on it's parameter option
   if option = LOW then just the lowest 4 rows are drawn. if option = HIGH
   then the upper 6 rows are added to the 4 already there.
*/
/* top_rows is an array used in fill area for the top 6 rows */
WCPT top_rows[] = { 0.0, 4.0,
    0.0, 10.0,
    10.0, 10.0,
    10.0, 4.0,
    0.0, 4.0 };
/* lake contains the data to fill the lakes */
WCPT lake[] = { 2.0, 5.0, 2.2, 4.3, 3.0, 4.0, 3.4, 4.05, 4.0, 4.5,
    4.0, 5.0, 3.9, 5.4, 3.3, 5.8, 3.0, 6.0, 2.5, 5.8,
    2.2, 5.3, 2.0, 5.0 };

void drawboard(int option)
{
    WCPT point[2];
    Gfloat ymax, a;
    int i;

    if (option == LOW)
    { /* draw 6 rows on top black */
        gsetfillcolourind(BLACK);
        gfillarea(5, top_rows);
        ymax = 4.0;
    }
    else
    { /* draw 6 rows on top green */
        gsetfillcolourind(GREEN);
        gfillarea(5, top_rows);
        ymax = 10.0;
    }
    gsetlinewidth(2.0);
    gsetlinecolourind(BLACK);
    /* draw vertical lines */
    point[0].y = 0.0;
    point[1].y = ymax;
    for (a = 0.0; a < 10.5; a += 1.0)
    {
        point[0].x = point[1].x = a;
        gpolyline(2, point);
    }

    /* draw horizontal lines */
    point[0].x = 0.0;
    point[1].x = 10.0;
    for (a = 0.0; a < ymax + 0.5; a += 1.0)
    {
        point[0].y = point[1].y = a;
        gpolyline(2, point);
    }

    /* draw lakes if drawing the whole screen */
    if (option == HIGH)
    {
        /* draw first lake */
        gsetfillcolourind(L_BLUE);
        gfillarea(12, lake);
        /* shift lake over to right 4 units and redraw as 2nd lake */
        for (i = 0; i < 12; ++i)
            lake[i].x += 4.0;
        gfillarea(12, lake);
        /* move lake back */
        for (i = 0; i < 12; ++i)
            lake[i].x -= 4.0;
    }
}

/****  makemove  ****/
/* makemove allows the player to choose the move he wishes to make.  both
   his first pick (origination square) and his second pick (destination square)
   are checked for validity.  if they are valid then the value of orig and dest
   are set to the corresponding squares.  if they are not valid the user is
   reprompted.  the user may quit the game by hitting break instead of his
   first pick.  if on the second pick he picks another square which contains
   his piece then this becomes his first pick and he is reprompted for a second
   pick.
*/

int makemove(void)
{
    int valid = 0;       /* flag to check if the move is valid */
    int i, j, k, l;      /* values to hold square position. i and j are for the
                    origination square. k and l are for the destination square
                 */
    int status;          /* pick status */
    int stop, firstpick; /* flags */

    while (!valid)
    {
#ifdef LTEST
        printf("pick the piece you wish to move\n");
#endif

        status = picksquare(&i, &j);
        if (status == GNONE)
        {
            stop = quitgame();
            if (stop == 1)
            {
                /* set origination to -1 (indicates quit) */
                orig.x = orig.y = dest.x = dest.y = QUIT;
                return (1);
            }
            else if (stop == 2)
            {
                /* don't quit, just skip turn */
                /* set origination to 99 (indicates skip turn) */
                orig.x = orig.y = dest.x = dest.y = SKIPMOVE;
                return (0);
            }
        }
        else
        {
            /* check for validity of first square */
            firstpick = TRUE;
            if (board[i][j].color == mycolor && board[i][j].rank != BOMB
                && board[i][j].rank != FLAG && board[i][j].rank != NO_PIECE)
            {
                while (firstpick)
                {
                    status = picksquare(&k, &l);
                    if (status != GNONE)
                    {
                        if (board[k][l].color == mycolor && board[k][l].rank != NO_PIECE && board[k][l].rank != BOMB && board[k][l].rank != FLAG)
                        {
                            /* if picked another square with his piece, make this
                         the first square and repick the second square.     */
                            i = k;
                            j = l;
                        }
                        else
                        {
                            valid = checkmove(i, j, k, l);
                            /* if good get out of loop to pick second piece */
                            if (valid)
                                firstpick = FALSE;
                        }
                    }
                }
            }
        }
    }
    orig.x = i;
    orig.y = j;
    dest.x = k;
    dest.y = l;
    return (0);
}

/****  checkmove   ****/
/* checkmove takes a source square and a destination square and determines if
   this consitutes a legal move. if it does it returns a 1 , if not it returns
   a 0.  there are 4 parametres: sx and sy are the position of the origination
   square and dx,dy is the position of the destination square.
*/

int checkmove(int sx, int sy, int dx, int dy)
{
    int i;
    int scoutmove = 1; /* flag for moving scout */
    int spacesx, spacesy;

    spacesx = sx - dx;
    spacesy = sy - dy;
    spacesx = spacesx < 0 ? -spacesx : spacesx;
    spacesy = spacesy < 0 ? -spacesy : spacesy;

    if (sx == dx || sy == dy) /* check to make sure not a diagonal move */
    {
        if (board[dx][dy].rank != -1) /* check lake move */
        {
            if (board[sx][sy].rank != SCOUT) /* do scout move diff from reg move */
            {
                /* check if not more than 2 spaces */
                if (spacesx < 2 && spacesy < 2)
                    return (1); /* it wasn't more than 2 spaces */
                else
                    return (0); /* it was more than 2 spaces */
            }
            else /* check if the scout move was valid */
            {
                if (spacesx > 1) /* horizontal move more than one space */
                {
                    if (sx < dx)
                    {
                        for (i = sx + 1; i <= sx + spacesx && scoutmove; ++i)
                        {
                            if (board[i][sy].rank != NO_PIECE)
                            {
                                /* there was a piece in the way of the move */
                                scoutmove = 0;
                            }
                        }
                    }
                    else
                    {
                        for (i = sx - 1; i >= sx - spacesx && scoutmove; --i)
                        {
                            if (board[i][sy].rank != NO_PIECE)
                            {
                                /* there was a piece in the way of the move */
                                scoutmove = 0;
                            }
                        }
                    }
                }
                else if (spacesy > 1) /* vertical move more than one space */
                {
                    if (sy < dy)
                    {
                        for (i = sy + 1; i <= sy + spacesy && scoutmove; ++i)
                        {
                            if (board[sx][i].rank != NO_PIECE)
                            {
                                /* there was a piece in the way */
                                scoutmove = 0;
                            }
                        }
                    }
                    else
                    {
                        for (i = sy - 1; i >= sy - spacesy && scoutmove; --i)
                        {
                            if (board[sx][i].rank != NO_PIECE)
                            {
                                /* there was a piece in the way */
                                scoutmove = 0;
                            }
                        }
                    }
                }
            }
        }
        if (!scoutmove)
            return (0);
        else
            return (1);
    }
    return (0); /* was diagonal or was lake move */
}

/****  quitgame  ****/
/* quitgame prompts the user with the option to quit, skip turn, or return
   without skipping turn.
*/

int quitgame(void)
{
    char c;

    do
    {
        printf(" 1) QUIT\n");
        printf(" 2) SKIP TURN\n");
        printf(" 3) RETURN WITHOUT SKIPPING TURN\n");
        printf("\n CHOOSE 1-3  ");
        c = getchar();
        rewind(stdin);
    } while (c < '1' || c > '3');
    printf("\n");
    return (c - '0');
}

/****  drawmove  ****/
/* drawmove takes the origination square and destination square and make the
   move both pictorally and internally.  there are two cases.
      1) no fight
      2) fight
   the first case is rather simple. however in the second case it must be
   determined who is the winner and who is attacking. each possability is
   addressed.
*/

void drawmove(void)
{
    int winner; /* the winner of a single fight */
    int i, j;

    if (orig.x == SKIPMOVE || orig.x == QUIT)
        return;                                 /* skipped or quit */
    if (board[dest.x][dest.y].rank == NO_PIECE) /*** NO FIGHT ***/
    {
        drawsquare(orig.x, orig.y, GREEN); /*erase square */
        if (board[orig.x][orig.y].color == mycolor)
            drawpiece(dest.x, dest.y, mycolor, board[orig.x][orig.y].rank);
        else /* only draw shape of opponents piece */
            drawcastle(dest.x, dest.y, board[orig.x][orig.y].color);
        /* make internal representation changes */
        board[dest.x][dest.y].rank = board[orig.x][orig.y].rank;
        board[dest.x][dest.y].color = board[orig.x][orig.y].color;
        board[orig.x][orig.y].rank = NO_PIECE;
    }
    else /** FIGHT **/
    {
        drawsquare(orig.x, orig.y, FLASH1); /* draw highlighted squares */
        drawsquare(dest.x, dest.y, FLASH2);
        drawpiece(orig.x, orig.y, board[orig.x][orig.y].color, board[orig.x][orig.y].rank);
        drawpiece(dest.x, dest.y, board[dest.x][dest.y].color, board[dest.x][dest.y].rank);
        for (i = 0; i < 300; ++i)
            ; /* time delay */
        /* call findwinner. if return 0, piece on orig square wins (attacker)
                          if return 1, piece on dest square wins (defender)
                          if return 2, both pieces are equal, and both lose
      */
        winner = findwinner(board[orig.x][orig.y].rank, board[dest.x][dest.y].rank);
        if (winner == 0)
        { /* attacker wins */
            for (i = 0; i < 7; ++i)
            {
                gsetcolorrep(1, FLASH1, &co_flashon);
                for (j = 0; j < 16000; ++j)
                    ;
                gsetcolorrep(1, FLASH1, &co_flashoff);
            }
            drawsquare(orig.x, orig.y, GREEN);
            drawsquare(dest.x, dest.y, GREEN);
            if (board[orig.x][orig.y].color == mycolor)
            {
                drawpiece(dest.x, dest.y, mycolor, board[orig.x][orig.y].rank);
                addpow(board[dest.x][dest.y].rank);
                if (board[dest.x][dest.y].rank == FLAG)
                {
                    printf("you win\n");
                    win = 1;
                }
            }
            else /* only draw shape of opponents piece */
            {
                drawcastle(dest.x, dest.y, board[orig.x][orig.y].color);
                if (board[dest.x][dest.y].rank == FLAG)
                {
                    printf("you lose\n");
                    win = 1;
                }
            }
            board[dest.x][dest.y].rank = board[orig.x][orig.y].rank;
            board[dest.x][dest.y].color = board[orig.x][orig.y].color;
            board[orig.x][orig.y].rank = NO_PIECE;
        }
        else if (winner == 1) /* defender wins */
        {
            for (i = 0; i < 7; ++i)
            {
                gsetcolorrep(1, FLASH2, &co_flashon);
                for (j = 0; j < 16000; ++j)
                    ;
                gsetcolorrep(1, FLASH2, &co_flashoff);
            }
            drawsquare(orig.x, orig.y, GREEN);
            drawsquare(dest.x, dest.y, GREEN);
            if (board[dest.x][dest.y].color == mycolor)
            {
                drawpiece(dest.x, dest.y, mycolor, board[dest.x][dest.y].rank);
                addpow(board[orig.x][orig.y].rank);
            }
            else
                drawcastle(dest.x, dest.y, board[dest.x][dest.y].color);
            board[orig.x][orig.y].rank = NO_PIECE;
        }
        else /* both pieces lose */
        {
            for (i = 0; i < 7; ++i)
            {
                gsetcolorrep(1, FLASH1, &co_flashon);
                gsetcolorrep(1, FLASH2, &co_flashon);
                for (j = 0; j < 16000; ++j)
                    ;
                gsetcolorrep(1, FLASH1, &co_flashoff);
                gsetcolorrep(1, FLASH2, &co_flashoff);
            }
            printf(" you both lose your pieces!\n");
            if (board[dest.x][dest.y].color == mycolor)
                addpow(board[orig.x][orig.y].rank);
            else
                addpow(board[dest.x][dest.y].rank);
            drawsquare(orig.x, orig.y, GREEN);
            drawsquare(dest.x, dest.y, GREEN);
            board[orig.x][orig.y].rank = board[dest.x][dest.y].rank = NO_PIECE;
        }
    }
}

/****  addpow  ****/
/* addpow admits an opponent's piece to the P.O.W. camp and prints what pieces
   are in the camp.
*/
static int pow[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void addpow(int rank)
{
    WCPT at;
    char name[2];

    if (rank != FLAG)
    {
        (pow[rank])++;
        gsetcharheight(0.25);
        if (mycolor == RED)
            gsettextcolorind(BLUE);
        else
            gsettextcolorind(RED);
        at.x = 11.50;
        at.y = 4.0 + (0.3 * (float) rank);
        gtext(&at, " ");
        name[0] = (char) ('0' + rank);
        name[1] = '\0';
        gtext(&at, name);
    }
}

/* drawpow -- draws initial pow camp */
void drawpow()
{
    int i;
    char name[2];
    WCPT at;

    at.x = 11.25;
    at.y = 8.0;
    gsetcharheight(0.25);
    gtext(&at, "PsOW:");

    for (i = 1; i < 12; i++)
    {
        at.x = 11.20;
        at.y = 4.0 + (0.3 * (float) i);
        switch (i)
        {
        case 10:
            name[0] = 'S';
            break;

        case 11:
            name[0] = 'B';
            break;

        default:
            name[0] = (char) ('0' + i);
            break;
        }
        name[1] = '\0';
        gtext(&at, name);
    }
}

/**** findwinner ****/
/* findwinner finds the winner of a single fight. the parameters are a and b.
   a is the rank of the attacker. b is the rank of the defender. if the a wins
   a 0 is returned. if b wins a 1 is returned. the method is to make a general
   statement that a higher piece wins and then check if a special case took
   place and change the result of the fight accordingly.
*/

int findwinner(int a, int b)
{
    int victor;

    /* first use general rule */
    if (b < a)
        victor = 1;
    else
        victor = 0;

    /* except we might have a special case and we might need to change victor */
    if (b == BOMB && a != MINER)
        victor = 1;
    if (a == SPY && b == MARSHAL)
        victor = 0;
    if (a == b)
        victor = 2;
    return (victor);
}

static struct
{
    int fx, fy, tx, ty;
} data_packet;

/**** sendmove  ****/
/* sendmove sends the move just made to the opponent */

void sendmove(void)
{
    data_packet.fx = orig.x;
    data_packet.fy = orig.y;
    data_packet.tx = dest.x;
    data_packet.ty = dest.y;

#ifndef LTEST
    write(pRtB[WRITE], &data_packet, sizeof(data_packet));
#endif
}

/**** receivemove ****/
/* receivemove receives the opponents move */

int receivemove(void)
{
    read(pBtR[READ], &data_packet, sizeof(data_packet));

    if (data_packet.fx == QUIT)
    {
        return (1);
    }
    else
    {
        /* to convert from opponents coordinates into player coordinates */
        /* subtract opponent positions from 9.                           */
        orig.x = 9 - data_packet.fx;
        orig.y = 9 - data_packet.fy;
        dest.x = 9 - data_packet.tx;
        dest.y = 9 - data_packet.ty;
        return (0);
    }
}

void transmit_set_up(void)
{
    int rank;
    int i, j, oppcolor;

    if (mycolor == RED)
        oppcolor = BLUE;
    else
        oppcolor = RED;

    /* write */
    for (j = 0; j < 4; ++j)
    {
        for (i = 0; i < 10; ++i)
        {
            rank = board[i][j].rank;
#ifdef DEBUG
            printf("red: about to write piece at %d, %d\n", i, j);
#endif
            write(pRtB[WRITE], &rank, sizeof(rank));
        }
    }

    /* read */
    for (j = 9; j >= 6; --j)
    {
        for (i = 9; i >= 0; --i)
        {
#ifdef DEBUG
            printf("red: about to read piece at %d, %d\n", i, j);
#endif
            read(pBtR[READ], &rank, sizeof(rank));
            board[i][j].rank = rank;
            board[i][j].color = oppcolor;
            drawcastle(i, j, oppcolor);
        }
    }
#ifdef DEBUG
    printf("red: done with transmit_set_up\n");
#endif
}
