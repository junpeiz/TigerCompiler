#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "table.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "translate.h"
#include "env.h"

E_enventry E_addVarEntry(Tr_access a, Ty_ty ty_res)
{
	E_enventry res = (E_enventry)malloc(sizeof(*res));
	res->env_type = E_varEntry;
	res->env_storage.var.ty = ty_res;
	res->env_storage.var.access = a;
	return res;
}

E_enventry E_addFunEntry(Tr_level level, Temp_label label, Ty_tyList formals, Ty_ty ty_res)
{
	E_enventry res = (E_enventry)malloc(sizeof(*res));
	res->env_type = E_funEntry;
	res->env_storage.fun.formals = formals;
	res->env_storage.fun.label = label;
	res->env_storage.fun.result  = ty_res;
	res->env_storage.fun.level = level;

	return res;
}

S_table E_base_tenv()
{
	S_table res = S_empty();
	S_enter(res, S_Symbol("int"), Ty_Int());
	S_enter(res, S_Symbol("string"), Ty_String());
	return res;
}

S_table E_base_venv()
{
  	S_table res = S_empty();
	return res;
}
