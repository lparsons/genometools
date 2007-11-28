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
#include "libgtext/genome_node.h"
#include "libgtlua/genome_node_lua.h"
#include "libgtlua/genome_visitor_lua.h"
#include "libgtlua/helper.h"
#include "libgtlua/range_lua.h"

#define check_genome_node(L, POS) \
              (GenomeNode**) luaL_checkudata(L, POS, GENOME_NODE_METATABLE)

static int genome_feature_lua_new(lua_State *L)
{
  GenomeNode **gf;
  GenomeFeatureType type;
  Range *range;
  Strand strand;
  const char *type_str, *strand_str;
  size_t length;
  Str *filename;
  assert(L);
  /* get/check parameters */
  type_str = luaL_checkstring(L, 1);
  luaL_argcheck(L, !genome_feature_type_get(&type, type_str), 1,
                "invalid feature type");
  range = check_range(L, 2);
  strand_str = luaL_checklstring(L, 3, &length);
  luaL_argcheck(L, length == 1, 3, "strand string must have length 1");
  luaL_argcheck(L, (strand = strand_get(strand_str[0])) != NUM_OF_STRAND_TYPES,
                3, "invalid strand");
  /* construct object */
  gf = lua_newuserdata(L, sizeof (GenomeNode*));
  filename = str_new_cstr("Lua");
  *gf = genome_feature_new(type, *range, strand, filename, 0);
  str_delete(filename);
  assert(*gf);
  luaL_getmetatable(L, GENOME_NODE_METATABLE);
  lua_setmetatable(L, -2);
  return 1;
}

static int genome_node_lua_get_filename(lua_State *L)
{
  GenomeNode **gn = check_genome_node(L, 1);
  lua_pushstring(L, genome_node_get_filename(*gn));
  return 1;
}

static int genome_node_lua_get_range(lua_State *L)
{
  GenomeNode **gn = check_genome_node(L, 1);
  return range_lua_push(L, genome_node_get_range(*gn));
}

static int genome_node_lua_accept(lua_State *L)
{
  GenomeNode **gn;
  GenomeVisitor **gv;
  Error *e;
  gn = check_genome_node(L, 1);
  gv = check_genome_visitor(L, 2);
  e = error_new();
  if (genome_node_accept(*gn, *gv, e))
    return luagt_error(L, e);
  error_delete(e);
  return 0;
}

static int genome_node_lua_is_part_of_genome_node(lua_State *L)
{
  GenomeNode **parent, **child;
  parent = check_genome_node(L, 1);
  child  = check_genome_node(L, 2);
  genome_node_is_part_of_genome_node(*parent, genome_node_rec_ref(*child));
  return 0;
}

static int genome_node_lua_mark(lua_State *L)
{
  GenomeNode **gn = check_genome_node(L, 1);
  genome_node_mark(*gn);
  return 0;
}

static int genome_node_lua_is_marked(lua_State *L)
{
  GenomeNode **gn = check_genome_node(L, 1);
  lua_pushboolean(L, genome_node_is_marked(*gn));
  return 1;
}

static int genome_node_lua_contains_marked(lua_State *L)
{
  GenomeNode **gn;
  gn = check_genome_node(L, 1);
  lua_pushboolean(L, genome_node_contains_marked(*gn));
  return 1;
}

static int genome_node_lua_delete(lua_State *L)
{
  GenomeNode **gn;
  gn = check_genome_node(L, 1);
  genome_node_rec_delete(*gn);
  return 0;
}

static const struct luaL_Reg genome_node_lib_f [] = {
  { "genome_feature_new", genome_feature_lua_new },
  { NULL, NULL }
};

static const struct luaL_Reg genome_node_lib_m [] = {
  { "get_filename", genome_node_lua_get_filename },
  { "get_range", genome_node_lua_get_range },
  { "accept", genome_node_lua_accept },
  { "is_part_of_genome_node", genome_node_lua_is_part_of_genome_node },
  { "mark", genome_node_lua_mark },
  { "is_marked", genome_node_lua_is_marked },
  { "contains_marked", genome_node_lua_contains_marked },
  { NULL, NULL }
};

int luaopen_genome_node(lua_State *L)
{
  assert(L);
  luaL_newmetatable(L, GENOME_NODE_METATABLE);
  /* metatable.__index = metatable */
  lua_pushvalue(L, -1); /* duplicate the metatable */
  lua_setfield(L, -2, "__index");
  /* set its _gc field */
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, genome_node_lua_delete);
  lua_settable(L, -3);
  /* register functions */
  luaL_register(L, NULL, genome_node_lib_m);
  luaL_register(L, "gt", genome_node_lib_f);
  return 1;
}

void genome_node_lua_push(lua_State *L, GenomeNode *gn)
{
  GenomeNode **gn_lua;
  assert(L && gn);
  gn_lua = lua_newuserdata(L, sizeof (GenomeNode**));
  *gn_lua = gn;
  luaL_getmetatable(L, GENOME_NODE_METATABLE);
  lua_setmetatable(L, -2);
}
