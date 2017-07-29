/*env.h*/
typedef struct E_enventry_ * E_enventry;

struct E_enventry_
{
	enum
	{
		E_varEntry,
		E_funEntry
	} env_type;
	union
	{
		struct {Tr_access access; Ty_ty ty;} var;
		struct {Tr_level level; Temp_label label;
			 Ty_tyList formals; Ty_ty result;} fun;
	} env_storage;
};

E_enventry E_addVarEntry(Tr_access access, Ty_ty ty);
E_enventry E_addFunEntry(Tr_level level, Temp_label label, Ty_tyList, Ty_ty result);

S_table E_base_tenv(void);
S_table E_base_venv(void);
