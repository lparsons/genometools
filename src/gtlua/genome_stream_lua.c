/*
  Copyright (c) 2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "lauxlib.h"
#include "core/fileutils.h"
#include "extended/node_stream.h"
#include "extended/gff3_in_stream.h"
#include "extended/gff3_out_stream.h"
#include "extended/luahelper.h"
#include "gtlua/genome_node_lua.h"
#include "gtlua/genome_stream_lua.h"

static int gff3_in_stream_lua_new_sorted(lua_State *L)
{
  GtNodeStream **gs;
  const char *filename;
  assert(L);
  /* get/check parameters */
  filename = luaL_checkstring(L, 1);
  luaL_argcheck(L, file_exists(filename), 1, "file does not exist");
  /* construct object */
  gs = lua_newuserdata(L, sizeof (GtNodeStream*));
  *gs = gff3_in_stream_new_sorted(filename, false);
  assert(*gs);
  luaL_getmetatable(L, GENOME_STREAM_METATABLE);
  lua_setmetatable(L, -2);
  return 1;
}

static int gff3_out_stream_lua_new(lua_State *L)
{
  GtNodeStream **out_stream, **in_stream = check_genome_stream(L, 1);
  assert(L);
  /* construct object */
  out_stream = lua_newuserdata(L, sizeof (GtNodeStream*));
  *out_stream = gff3_out_stream_new(*in_stream, NULL);
  assert(*out_stream);
  luaL_getmetatable(L, GENOME_STREAM_METATABLE);
  lua_setmetatable(L, -2);
  return 1;
}

static int gt_node_stream_lua_next_tree(lua_State *L)
{
  GtNodeStream **gs = check_genome_stream(L, 1);
  GtGenomeNode *gn;
  GtError *err = gt_error_new();
  if (gt_node_stream_next(*gs, &gn, err))
    return lua_gt_error(L, err); /* handle error */
  else if (gn)
    gt_lua_genome_node_push(L, gn);
  else
    lua_pushnil(L);
  gt_error_delete(err);
  return 1;
}

static int gt_node_stream_lua_delete(lua_State *L)
{
  GtNodeStream **gs = check_genome_stream(L, 1);
  gt_node_stream_delete(*gs);
  return 0;
}

static const struct luaL_Reg gt_node_stream_lib_f [] = {
  { "gff3_in_stream_new_sorted", gff3_in_stream_lua_new_sorted },
  { "gff3_out_stream_new", gff3_out_stream_lua_new },
  { NULL, NULL }
};

static const struct luaL_Reg gt_node_stream_lib_m [] = {
  { "next_tree", gt_node_stream_lua_next_tree },
  { NULL, NULL }
};

int luaopen_genome_stream(lua_State *L)
{
  assert(L);
  luaL_newmetatable(L, GENOME_STREAM_METATABLE);
  /* metatable.__index = metatable */
  lua_pushvalue(L, -1); /* duplicate the metatable */
  lua_setfield(L, -2, "__index");
  /* set its _gc field */
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, gt_node_stream_lua_delete);
  lua_settable(L, -3);
  /* register functions */
  luaL_register(L, NULL, gt_node_stream_lib_m);
  luaL_register(L, "gt", gt_node_stream_lib_f);
  return 1;
}
