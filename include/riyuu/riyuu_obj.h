
#ifndef riyuu__riyuu_obj_h
#define riyuu__riyuu_obj_h

#include <stdint.h>

typedef struct _riyuu_obj {
	uint64_t len;
	char data[1];
} riyuu_obj;

riyuu_obj *riyuu_serialize(void *data, uint64_t len);

#endif
