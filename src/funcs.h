#ifndef YAMBDA_FUNCS
#define YAMBDA_FUNCS

#include "types.h"

/* static const char F_DEF[] = "def"; */
/* static const char F_REF[] = "ref"; */

/* static const char F_SUM[] = "sum"; */

/* static const char F_Q[] = "`"; */
/* static const char F_A[] = "a?"; */
/* static const char F_EQ[] = "=?"; */
/* static const char F_CAR[] = "car"; */
/* static const char F_CDR[] = "cdr"; */
/* static const char F_CONS[] = "cons"; */
/* static const char F_COND[] = "cond"; */

#define F_DEF "def"
#define F_REF "ref"
#define F_SUM "sum"

#define F_Q "`"
#define F_A "a?"
#define F_EQ "=?"
#define F_CAR "car"
#define F_CDR "cdr"
#define F_CONS "cons"
#define F_COND "cond"

void init_funcs(Env *env);
Element *apply(Env *env, Element *lambda, Element *args);
Element *eval(Env *env, Element *head);

#endif
