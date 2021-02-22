/*
 * Do not edit this file, it is generated automatically from
 * the gks_errors.h file via mkerrmsg.c.
 */

#ifndef GKSERRMSG_H
#define GKSERRMSG_H
#ifdef GKS_ERROR_C
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
 *
 * University of Illinois at Urbana-Champaign
 * Department of Computer Science
 * 1304 W. Springfield Ave.
 * Urbana, IL   61801
 *
 * (C) Copyright 1987, 1988 by The University of Illinois Board of Trustees.
 * All rights reserved.
 *
 * Tool: X 11 Graphical Kernel System
 * Author: Gregory Scott Rogers
 * Author: Sung Hsien Ching Kelvin
 * Author: Yu Pan
 * 
 */

/* clang-format off */

/* 
 * States
 */
char *GKSErrorMessages0to200[] = {
/* 0 */                 (char *) NULL,
/* ENOTGKCL 1 */        "GKS not in proper state: GKS shall be in the state GKCL",
/* ENOTGKKOP 2 */       "GKS not in proper state: GKS shall be in the state GKOP",
/* ENOTWASC 3 */        "GKS not in proper state: GKS shall be in the state WSAC",
/* ENOTSGOP 4 */        "GKS not in proper state: GKS shall be in the state SGOP",
/* ENOTACOP 5 */        "GKS not in proper state: GKS shall be either in the state WSAC or in the state SGOP",
/* ENOTOPAC 6 */        "GKS not in proper state: GKS shall be either in the state WSOP or in the state WSAC",
/* ENOTWSOP 7 */        "GKS not in proper state: GKS shall be in one of the states WSOP, WSAC or SGOP",
/* ENOTGWWS 8 */        "GKS not in proper state: GKS shall be in one of the states GKOP, WSOP, WSAC or SGOP",
/*
 * Workstations
 */
/* 9 */                 (char *) NULL,
/* 10 */                (char *) NULL,
/* 11 */                (char *) NULL,
/* 12 */                (char *) NULL,
/* 13 */                (char *) NULL,
/* 14 */                (char *) NULL,
/* 15 */                (char *) NULL,
/* 16 */                (char *) NULL,
/* 17 */                (char *) NULL,
/* 18 */                (char *) NULL,
/* 19 */                (char *) NULL,
/* EWSIDINV 20 */       "Specified workstation identifier is invalid",
/* ECNIDINV 21 */       "Specified connection identifier is invalid",
/* EWSTPINV 22 */       "Specified workstation type is invalid",
/* ENOWSTYP 23 */       "Specified workstation type does not exist",
/* EWSISOPN 24 */       "Specified workstation is open",
/* EWSNOTOP 25 */       "Specified workstation is not open",
/* EWSCNTOP 26 */       "Specified workstation cannot be opened",
/* EWISSNOP 27 */       "Workstation Independent Segment Storage is not open",
/* EWISSOPN 28 */       "Workstation Independent Segment Storage is already open",
/* EWSISACT 29 */       "Specified workstation is active",
/* EWSNTSCT 30 */       "Specified workstation is not active",
/* EWSCATMO 31 */       "Specified workstation is of category MO",
/* EWSNOTMO 32 */       "Specified workstation is not of category MO",
/* EWSCATMI 33 */       "Specified workstation is of category MI",
/* EWSNOTMI 34 */       "Specified workstation is not of category MI",
/* EWSCATIN 35 */       "Specified workstation is of category INPUT",
/* EWSIWISS 36 */       "Specified workstation is Workstation Independent Segment Storage",
/* EWSNOTOI 37 */       "Specified workstation is not of category OUTIN",
/* EWSNOTIO 38 */       "Specified workstation is neither of category INPUT nor of category OUTIN",
/* EWSNOTOO 39 */       "Specified workstation is neither of category OUTPUT nor of category OUTIN",
/* EWSNOPXL 40 */       "Specified workstation has no pixel store readback capability",
/* EWSNOGDP 41 */       "Specified workstation type is not able to generate the specified generalized drawing primitive",
/* EWSMXOPN 42 */       "Maximum number of simultaneously open workstations would be exceeded",
/* EWSMXACT 43 */       "Maximum number of simultaneously active workstations would be exceeded",
/*
 * Transformations
 */
/* 44 */                (char *) NULL,
/* 45 */                (char *) NULL,
/* 46 */                (char *) NULL,
/* 47 */                (char *) NULL,
/* 48 */                (char *) NULL,
/* 49 */                (char *) NULL,
/* EBADXFRM 50 */       "Transformation number is invalid",
/* EBADRCTD 51 */       "Rectangle definition is invalid",
/* EBDVIEWP 52 */       "Viewport is not within the Normalized Device Coordinate unit square",
/* EBDWINDW 53 */       "Workstation window is not within the Normalized Device Coordinate unit square",
/* EVIEWDSP 54 */       "Workstation viewport is not within the display space",
/*
 * Output Attributes
 */
/* 55 */                (char *) NULL,
/* 56 */                (char *) NULL,
/* 57 */                (char *) NULL,
/* 58 */                (char *) NULL,
/* 59 */                (char *) NULL,
/* EBADLINX 60 */       "Polyline index is invalid",
/* ENOLINEX 61 */       "A representation for the specified polyline index has not been defined on this workstation",
/* ENOPLINX 62 */       "A representation for the specified polyline index has not been predefined on this workstation",
/* ELINEEQZ 63 */       "Linetype is equal to zero",
/* ENOLINTP 64 */       "Specified linetype is not supported on this workstation",
/* ELNWDLTZ 65 */       "Linewidth scale factor is less than zero",
/* EBADMRKX 66 */       "Polymarker index is invalid",
/* ENOMARKX 67 */       "A representation for the specified polymarker index has not been defined on this workstation",
/* ENOPMRKX 68 */       "A representation for the specified polymarker index has not been predefined on this workstation",
/* EMAKREQZ 69 */       "Marker type is equal to zero",
/* ENOMRKTP 70 */       "Specified marker type is not supported on this workstation",
/* EMKSZLTZ 71 */       "Marker size scale factor is less than zero",
/* EBADTXTX 72 */       "Text index is invalid",
/* ENOTEXTX 73 */       "A representation for the specified text index has not been defined on this workstation",
/* ENOPTXTX 74 */       "A representation for the specified text index has not been predefined on this workstation",
/* ETXTFEQZ 75 */       "Text font is equal to zero",
/* ENOTXTFP 76 */       "Requested text font is not supported for the specified precision on this workstation",
/* ECEXFLEZ 77 */       "Character expansion factor is less than or equal to zero",
/* ECHHTLEZ 78 */       "Character height is less than or equal to zero",
/* ECHRUPVZ 79 */       "Length of character up vector is zero",
/* EBADFILX 80 */       "Fill area index is invalid",
/* ENOFILLX 81 */       "A representation for the specified fill area index has not been defined on this workstation",
/* ENOPFILX 82 */       "A representation for the specified fill area index has not been predefined on this workstation",
/* ENOFSTYL 83 */       "Specified fill area interior style is not supported on this workstation",
/* ESTYLEQZ 84 */       "Style (pattern or hatch) index is equal to zero",
/* EBADPATN 85 */       "Specified pattern index is invalid",
/* ENOHATCH 86 */       "Specified hatch style is not supported on this workstation",
/* EPATSZLZ 87 */       "Pattern size value is not positive",
/* ENOPATNX 88 */       "A representation for the specified pattern index has not been defined on this workstation",
/* ENOPPTNX 89 */       "A representation for the specified pattern index has not been predefined on this workstation",
/* ENOPSTYL 90 */       "Interior style PATTERN is not supported on this workstation",
/* ECADIMEN 91 */       "Dimensions of colour array are invalid",
/* ECINDXLZ 92 */       "Colour index is less than zero",
/* EBADCOLX 93 */       "Colour index is invalid",
/* ENOCOLRX 94 */       "A representation for the specified colour index has not been defined on this workstation",
/* ENOPCLRX 95 */       "A representation for the specified colour index has not been predefined on this workstation",
/* ECOLRNGE 96 */       "Colour is outside range [0,1]",
/* EBADPICK 97 */       "Pick identifier is invalid",
/*
 * Output Primitives
 */
/* 98 */                (char *) NULL,
/* 99 */                (char *) NULL,
/* ENPOINTS 100 */      "Number of points is invalid",
/* ECHRCODE 101 */      "Invalid code in string",
/* EBDGDPID 102 */      "Generalized drawing primitive identifier is invalid",
/* EGDPDATA 103 */      "Content of generalized drawing primitive data record is invalid",
/* ECANTGDP 104 */      "At least one active workstation is not able to generate the specified generalized drawing primitive",
/* ECNTGDPC 105 */      "At least one active workstation is not able to generate the specified generalized drawing primitive under the current transformations and clipping rectangle",
/*
 * Segments
 */
/* 106 */               (char *) NULL,
/* 107 */               (char *) NULL,
/* 108 */               (char *) NULL,
/* 109 */               (char *) NULL,
/* 110 */               (char *) NULL,
/* 111 */               (char *) NULL,
/* 112 */               (char *) NULL,
/* 113 */               (char *) NULL,
/* 114 */               (char *) NULL,
/* 115 */               (char *) NULL,
/* 116 */               (char *) NULL,
/* 117 */               (char *) NULL,
/* 118 */               (char *) NULL,
/* 119 */               (char *) NULL,
/* EBADNAME 120 */      "Specified segment name is invalid",
/* ENAMUSED 121 */      "Specified segment name is already in use",
/* EWHATSEG 122 */      "Specified segment does not exist",
/* EWORKSEG 123 */      "Specified segment does not exist on specified workstation",
/* EWISSSEG 124 */      "Specified segment does not exist on Workstation Independent Segment Storage",
/* ESEGOPEN 125 */      "Specified segment is open",
/* ESEGPRIR 126 */      "Segment priority is outside the range [0,1]",
/*
 * Input
 */
/* 127 */               (char *) NULL,
/* 128 */               (char *) NULL,
/* 129 */               (char *) NULL,
/* 130 */               (char *) NULL,
/* 131 */               (char *) NULL,
/* 132 */               (char *) NULL,
/* 133 */               (char *) NULL,
/* 134 */               (char *) NULL,
/* 135 */               (char *) NULL,
/* 136 */               (char *) NULL,
/* 137 */               (char *) NULL,
/* 138 */               (char *) NULL,
/* 139 */               (char *) NULL,
/* ENOINDEV 140 */      "Specified input device is not present on workstation",
/* EREQUEST 141 */      "Input device is not in REQUEST mode",
/* ENSAMPLE 142 */      "Input device is not in SAMPLE mode",
/* ENOEVSMP 143 */      "EVENT and SAMPLE input mode are not available at this level of GKS",
/* ENOPETWS 144 */      "Specified prompt and echo type is not supported on this workstation",
/* EEBOUNDS 145 */      "Echo area is outside display space",
/* EBADDATA 146 */      "Contents of input data record are invalid",
/* EINQOVFL 147 */      "Input queue has overflowed",
/* ENOQOVFL 148 */      "Input queue has not overflowed since GKS was opened or the last invocation of INQUIRE INPUT QUEUE OVERFLOW",
/* EASWSCLO 149 */      "Input queue has overflowed, but associated workstation has been closed",
/* ENOCURIV 150 */      "No input value of the correct class is in the current event report",
/* EINVTOUT 151 */      "Timeout is invalid",
/* EBDINITV 152 */      "Initial value is invalid",
/* ESTROKSZ 153 */      "Number of points in the initial strokes is greater than the buffer size",
/* ESTRINSZ 154 */      "Length of the initial string is greater than the buffer size",
/*
 * Metafiles
 */
/* 155 */               (char *) NULL,
/* 156 */               (char *) NULL,
/* 157 */               (char *) NULL,
/* 158 */               (char *) NULL,
/* 159 */               (char *) NULL,
/* ERESERVE 160 */      "Item type is not allowed for user items",
/* EBDLNGTH 161 */      "Item length is invalid",
/* EMNOITEM 162 */      "No item is left in GKS Metafile input",
/* EMITMINV 163 */      "Metafile item is invalid",
/* ENOTGKSI 164 */      "Item type is not a valid GKS item",
/* EBADCNTS 165 */      "Content of item data record is invalid for the specified item type",
/* EEBDMXDR 166 */      "Maximum item data record length is invalid",
/* EINTERPT 167 */      "User item cannot be interpreted",
/* ENOFUNCT 168 */      "Specified function is not supported in this level of GKS",
/*
 * Escape
 */
/* 169 */               (char *) NULL,
/* 170 */               (char *) NULL,
/* 171 */               (char *) NULL,
/* 172 */               (char *) NULL,
/* 173 */               (char *) NULL,
/* 174 */               (char *) NULL,
/* 175 */               (char *) NULL,
/* 176 */               (char *) NULL,
/* 177 */               (char *) NULL,
/* 178 */               (char *) NULL,
/* 179 */               (char *) NULL,
/* ENOESCFN 180 */      "Specified escape function is not supported",
/* ESCIDINV 181 */      "Specified escape function identification is invalid",
/* EESCDATA 182 */      "Contents of escape data record are invalid",
/* 
 * Miscellaneous
*/
/* 183 */               (char *) NULL,
/* 184 */               (char *) NULL,
/* 185 */               (char *) NULL,
/* 186 */               (char *) NULL,
/* 187 */               (char *) NULL,
/* 188 */               (char *) NULL,
/* 189 */               (char *) NULL,
/* 190 */               (char *) NULL,
/* 191 */               (char *) NULL,
/* 192 */               (char *) NULL,
/* 193 */               (char *) NULL,
/* 194 */               (char *) NULL,
/* 195 */               (char *) NULL,
/* 196 */               (char *) NULL,
/* 197 */               (char *) NULL,
/* 198 */               (char *) NULL,
/* 199 */               (char *) NULL,
/* EBDERRFL 200 */      "Specified error file is invalid",
/* 
 * System
*/
};
char *GKSErrorMessages300to309[] = {
/* EMEMSPAC 300 */      "Storage overflow has occurred in GKS",
/* ESEGSPAC 301 */      "Storage overflow has occurred in segment storage",
/* EIO_READ 302 */      "Input/Output error has occurred while reading",
/* EIOWRITE 303 */      "Input/Output error has occurred while writing",
/* EIOSENDD 304 */      "Input/Output error has occurred while sending data to a workstation",
/* EIORECDA 305 */      "Input/Output error has occurred while receiving data from a workstation",
/* EIOLIBMG 306 */      "Input/Output error has occurred during program library management",
/* EIORDWDT 307 */      "Input/Output error has occurred while reading workstation description table",
/* EMATHERR 308 */      "Arithmetic error has occurred",
/* ESTRGERR 309 */      "Character string does not match any known value",
/*
 * Language Binding Specific
 */
};
char *GKSErrorMessages2000to2000[] = {
/* EBADENUM 2000 */     "Enumeration type out of range",
/* EOUTSIZE 2001 */     "Output parameter size insufficient",
/* EBADSMEM 2002 */     "List element or set member not available",
/* EBADPKDR 2003 */     "Invalid data record",
};

#else /* GKS_ERROR_C */
extern char *GKSErrorMessages0to200[];
extern char *GKSErrorMessages300to309[];
extern char *GKSErrorMessages2000to2000[];

#endif

#endif
