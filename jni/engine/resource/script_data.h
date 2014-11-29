/*
 * script_data.h
 *
 *  Created on: 2014-3-24
 *      Author: ryan
 */

#ifndef SCRIPT_DATA_H_
#define SCRIPT_DATA_H_

#include <string.h>
#include <base.h>
#include <ext/liblua/lua.h>
#include <ext/liblua/lualib.h>
#include <ext/liblua/lauxlib.h>
#include <util/aes/aes256.h>

#define script_data_loadLuac(L, buff, size) \
	(luaL_loadbuffer(L, buff, size, buff) || lua_pcall(L, 0, LUA_MULTRET, 0))

typedef struct {
	aes256_context ctx;
} ScriptData;

ScriptData *script_data_create();
void script_data_destroy(ScriptData *script);
unsigned char *script_data_decript(ScriptData *script, const unsigned char *data);

#endif /* SCRIPT_DATA_H_ */
