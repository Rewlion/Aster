import clang.cindex

def is_ref_type(cursor_type):
  return cursor_type.kind == clang.cindex.TypeKind.LVALUEREFERENCE

def is_const_type(cursor_type):
  realType = cursor_type.get_pointee()
  return realType.is_const_qualified()

def get_real_type(cursor_type):
  return cursor_type.get_pointee() if is_ref_type(cursor_type) else cursor_type

def get_template_real_type(cursor_type):
  return cursor_type if cursor_type.kind == clang.cindex.TypeKind.ELABORATED else cursor_type.get_pointee()

def get_real_type_name(real_type):
  if real_type.kind == clang.cindex.TypeKind.TYPEDEF:
    return real_type.get_typedef_name()
  elif real_type.kind == clang.cindex.TypeKind.INT:
    return "int"
  elif real_type.kind == clang.cindex.TypeKind.FLOAT:
    return "float"
  elif real_type.kind == clang.cindex.TypeKind.BOOL:
    return "bool"
  else:
    raise Exception('{} is unknown type'.format(real_type.spelling))

def get_cursor_type_name(cursor_type):
  realType = get_real_type(cursor_type)
  return get_real_type_name(realType)

def is_eastl_functor(param_cursor):
  it = param_cursor.get_children()
  namespaceType = next(it)
  paramType = next(it)
  isEastlNamespace = namespaceType.kind == clang.cindex.CursorKind.NAMESPACE_REF\
    and namespaceType.spelling == "eastl"
  isEastlFunction = paramType.kind == clang.cindex.CursorKind.TEMPLATE_REF\
    and paramType.spelling == "function"

  return isEastlNamespace and isEastlFunction

def get_template_param_names(param_cursor):
  templateParamNames = []
  for templateParamName in param_cursor.get_children():
    if templateParamName.kind == clang.cindex.CursorKind.PARM_DECL:
      templateParamNames = templateParamNames + [templateParamName.spelling]
  return templateParamNames