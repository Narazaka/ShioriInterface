/**
 * @file
 * @brief SHIORI/3.x DLLインターフェース
 * @author Narazaka: http://narazaka.net/
 *
 * (C) 2014 Narazaka : Licensed under The MIT License - http://narazaka.net/license/MIT?2014
 **/
#include <stdlib.h>
#include <string.h>
#include "shiori.h"

#include <msclr/marshal_cppstd.h>
using namespace msclr::interop;

SHIORI_EXPORT BOOL SHIORI_CALL load(const MEMORY_HANDLE h, long len) {
	char* str = (char*)malloc(sizeof(char) * (len + 1));
	System::String^ csstr;
	strncpy(str, (const char*)h, len);
	str[len] = '\0';
	SHIORI_FREE(h);
	bool ret;
	csstr = marshal_as<System::String^>(str);
	free(str);
	ret = ShioriInterface::ShioriInterface::Load(csstr);
	return ret;
}

#define SHIORI_LINES_BUFFER_STEP 10
SHIORI_EXPORT MEMORY_HANDLE SHIORI_CALL request(const MEMORY_HANDLE h, long* len) {
	char* str = (char*)malloc(sizeof(char) * (*len + 1));
	const char* resstr;
	MEMORY_HANDLE reth;
	System::String^ csstr;
	strncpy(str, (const char*)h, *len);
	str[*len] = '\0';
	SHIORI_FREE(h);
	csstr = marshal_as<System::String^>(str);
	free(str);
	csstr = ShioriInterface::ShioriInterface::Request(csstr);
	{
		marshal_context ctx;
		resstr = ctx.marshal_as<const char*>(csstr);
		*len = strlen(resstr);
		reth = (MEMORY_HANDLE)SHIORI_MALLOC(*len);
		memcpy(reth, resstr, *len);
	}
	return reth;
}

SHIORI_EXPORT BOOL SHIORI_CALL unload(void) {
	return ShioriInterface::ShioriInterface::Unload();
}

