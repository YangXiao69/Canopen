
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef SLAVE_H
#define SLAVE_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 slave_valueRangeTest (UNS8 typeValue, void * value);
const indextable * slave_scanIndexOD (CO_Data *d, UNS16 wIndex, UNS32 * errorCode);

/* Master node data struct */
extern CO_Data slave_Data;
extern UNS16 c1;		/* Mapped at index 0x2000, subindex 0x00*/
extern UNS16 c2;		/* Mapped at index 0x2001, subindex 0x00*/
extern UNS16 c3;		/* Mapped at index 0x2002, subindex 0x00*/
extern UNS16 c4;		/* Mapped at index 0x2003, subindex 0x00*/

#endif // SLAVE_H