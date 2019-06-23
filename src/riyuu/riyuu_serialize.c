
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <riyuu/riyuu.h>
#include <riyuu/riyuu_obj.h>

riyuu_obj *riyuu_serialize(void *data, uint64_t len)
{
	riyuu_obj *obj = (riyuu_obj *)malloc(sizeof(riyuu_obj) + len);
	obj->len = len;
	memcpy(obj->data, data, len);
	return obj;
}

void riyuu_serialize_cmd(char *file)
{
	FILE *fp;
	riyuu_obj *obj;
	char buf[8096];
	size_t insize;
	insize = fread(buf, 1, 8096, stdin);
	obj = riyuu_serialize(buf, insize);
	fp = fopen(file, "wb");
	fwrite(obj, 1, sizeof(riyuu_obj) + insize, fp);
	fclose(fp);
}
