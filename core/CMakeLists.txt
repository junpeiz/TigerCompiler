cmake_minimum_required(VERSION 2.8)
project(Tiger-Compiler)

set(SOURCE_FILES main.c absyn.h absyn.c env.c env.h errormsg.h errormsg.c prirtree.h prirtree.c semant.c semant.h symbol.c symbol.h table.c table.h temp.c temp.h translate.c translate.h tree.c tree.h types.h types.c frame.h frame.c util.c util.h assem.c assem.h canon.c canon.h codegen.c codegen.h y.tab.c y.tab.h lex.yy.c
prabsyn.c prabsyn.h)

add_executable(TigerCompiler ${SOURCE_FILES})
