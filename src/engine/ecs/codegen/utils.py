import clang.cindex
import pprint
import os

ES_SYSTEM_TAG = "ecs_system"
ES_EVENT_SYSTEM_TAG = "ecs_event_system"
ES_QUERY_TAG = "ecs_query"
ECS_EVENT_BASE_STRUCT = "ecs_event_base_struct"
ECS_NAMESPACE = "Engine::ECS"

VALID_PARAM_TYPES = {
  'int',
  'int2',
  'int3',
  'int4',
  'float',
  'float2',
  'float3',
  'float4',
  "string"
}

def print_node(node):
  print("kind:{} spelling:{}".format(node.kind, node.spelling))

def is_eastl_functor(params):
  namespace = next(params)
  paramType = next(params)

  if namespace.kind == clang.cindex.CursorKind.NAMESPACE_REF and namespace.spelling == "eastl"\
    and paramType.kind == clang.cindex.CursorKind.TEMPLATE_REF and paramType.spelling == "function":
    return True

  return False

def is_const_type(arg):
  clangType = arg.get_pointee()
  return clangType.is_const_qualified()

def is_ref_type(arg):
  return arg.kind == clang.cindex.TypeKind.LVALUEREFERENCE

def get_type_name(arg):
  t = arg.get_pointee() if arg.kind == clang.cindex.TypeKind.LVALUEREFERENCE else arg
  if t.kind == clang.cindex.TypeKind.TYPEDEF:
    return t.get_typedef_name()
  elif t.kind == clang.cindex.TypeKind.INT:
    return "int"
  elif t.kind == clang.cindex.TypeKind.FLOAT:
    return "float"
  else:
    raise ('{} is unknown type'.format(arg.spelling))

def parse_query_params(params):
  paramsCount = len(params)
  if paramsCount == 0:
    return []

  if paramsCount != 1:
    print("ERROR: query has invalid number params:{}, 1 expected [{}]".format(paramsCount, params.location))

  if not is_eastl_functor(params[0].get_children()):
    print("ERROR: query has invalid param:{}, eastl::function expected or None [{}]".format(params[0].spelling, params[0].location))
    exit(-1)

  # eastl::function<void(const Float2& n, Float3 k)>& cb
  # here cb == params[0]
  # cb has 4 childrens:
  #   eastl
  #   function
  #   n
  #   k

  templateParamNames = []
  for templateParamName in params[0].get_children():
    if templateParamName.kind == clang.cindex.CursorKind.PARM_DECL:
      templateParamNames = templateParamNames + [templateParamName.spelling]

  # if typekind is lvaluereference, get real type by get_pointee
  cbType = params[0].type if params[0].type.kind == clang.cindex.TypeKind.ELABORATED else params[0].type.get_pointee()
  template = cbType.get_template_argument_type(0)

  queryParamInfos = []
  for arg in template.argument_types():
    typeName = arg.spelling
    queryParamInfos = queryParamInfos + [{
      'type_name': get_type_name(arg),
      'const': "const" if is_const_type(arg) else "",
      'ref': "&" if is_ref_type(arg) else ""
    }]

  for i in range(len(templateParamNames)):
    queryParamInfos[i]['name'] = templateParamNames[i]

  return queryParamInfos

def parse_query(function, paramsNodes):
  return {
    'name': function.spelling,
    'query_params': parse_query_params(paramsNodes)
  }

def gen_query(parsedQuery):
  queryName = parsedQuery['name']
  queryParamsInfo = parsedQuery['query_params']

  queryComponentTemplate = """DESCRIBE_QUERY_COMPONENT("{component_name}", {component_type})"""
  queryComponents = ""
  for p in queryParamsInfo:
    queryComponents = "{} {},".format(
      queryComponents,
      queryComponentTemplate.format(component_name = p['name'], component_type= p['type_name'])
    )
  queryComponents = queryComponents[:-1]

  queryIdName = "{}_queryId".format(parsedQuery['name'])
  queryIdDeclaration = """
  const static query_id {query_id_name} = Registry::register_direct_query(
    DirectQueryDescription{{
      .components = {{
        {query_components}
      }}
    }}
  );
  """.format(query_id_name = queryIdName, query_components = queryComponents )

  functorParamTemplate = "{is_const} {param_type}{is_ref} {param_name}"
  functorParams = ""
  for p in queryParamsInfo:
    functorParams = "{} {},".format(
      functorParams,
      functorParamTemplate.format(
        is_const = p['const'],
        param_type = p['type_name'],
        is_ref = p['ref'],
        param_name = p['name']
      )
    )
  functorParams = functorParams[:-1]

  componentAccessTemplate = """{is_const} {param_type}{is_ref} {param_name} = accessor.Get<{param_type}>(str_hash("{param_name}")) """
  componentsAccessors = ""
  for p in queryParamsInfo:
    componentsAccessors = "{} {};\n".format(
      componentsAccessors,
      componentAccessTemplate.format(
        is_const = p['const'],
        param_type = p['type_name'],
        is_ref = p['ref'],
        param_name = p['name']
      )
    )
  componentsAccessors = componentsAccessors[:-1]

  queryCallbackParams = ""
  for p in queryParamsInfo:
    queryCallbackParams = queryCallbackParams + p['name'] + ","
  queryCallbackParams = queryCallbackParams[:-1]

  queryDeclaration = """
  static void {query_name} (eastl::function<void({functor_params})> cb)
  {{
    {ecs_namespace}::manager.query({query_id}, [&](ComponentsAccessor& accessor)
    {{
      {components_accessors}
      cb({callback_execute_params});
    }});
  }}
  """.format(
    query_name = queryName,
    functor_params = functorParams,
    ecs_namespace = ECS_NAMESPACE,
    query_id = queryIdName,
    components_accessors = componentsAccessors,
    callback_execute_params = queryCallbackParams
  )

  gen_src = '\n'.join([queryIdDeclaration, queryDeclaration])

  return gen_src

def get_param_info(param):
  paramType = param.type
  return {
    'name':param.spelling,
    'type_name': get_type_name(paramType),
    'const': "const" if is_const_type(paramType) else "",
    'ref': "&" if is_ref_type(paramType) else ""
  }

def parse_function_params(paramsNodes):
  paramInfos = []
  for param in paramsNodes:
    paramInfos = paramInfos + [get_param_info(param)]

  return paramInfos

def parse_system(function, paramNodes):
  return {
    'name': function.spelling,
    'function_params': parse_function_params(paramNodes)
  }

def get_event_name(event):
  type = event.type.get_pointee() if is_ref_type(event.type) else event.type
  return type.get_declaration().spelling

def parse_event_system(function, paramNodes):
  return {
    'name': function.spelling,
    'event_name': get_event_name(paramNodes[0]),
    'function_params': parse_function_params(paramNodes[1:])
  }

def gen_event_system(parsedFunction):
  funcParamsInfo = parsedFunction['function_params']
  eventName = parsedFunction['event_name']

  componentAccessTemplate = """{is_const} {param_type}{is_ref} {param_name} = accessor.Get<{param_type}>(str_hash("{param_name}")) """
  componentsAccessors = ""
  for p in funcParamsInfo:
    componentsAccessors = "{} {};\n".format(
      componentsAccessors,
      componentAccessTemplate.format(
        is_const = p['const'],
        param_type = p['type_name'],
        is_ref = p['ref'],
        param_name = p['name']
      )
    )
  componentsAccessors = componentsAccessors[:-1]

  callbackParams = ""
  for p in funcParamsInfo:
    callbackParams = callbackParams + p['name'] + ","
  callbackParams = callbackParams[:-1]

  if len(funcParamsInfo) > 0:
    callbackParams = ', ' + callbackParams

  systemName = parsedFunction['name']
  systemDeclaration = """
static void {system_name}_internal(Event* event, ComponentsAccessor& accessor)
{{
  {event_type}* casted_event = reinterpret_cast<{event_type}*>(event);
  {component_accessors}
  {system_name}(*casted_event {callback_execute_params});
}}
  """.format(
    system_name = systemName,
    event_type = eventName,
    component_accessors = componentsAccessors,
    callback_execute_params = callbackParams
  )

  queryComponentTemplate = """DESCRIBE_QUERY_COMPONENT("{component_name}", {component_type})"""
  queryComponents = ""
  for p in funcParamsInfo:
    queryComponents = "{} {},".format(
      queryComponents,
      queryComponentTemplate.format(component_name = p['name'], component_type= p['type_name'])
    )
  queryComponents = queryComponents[:-1]

  systemRegistration = """
static const bool {system_name}_desc = Registry::register_cpp_query(
  QueryDescription{{
    .components = {{
      {components_descriptions}
    }},
    .eventCb = {system_name}_internal,
    .event = str_hash("{event_type}")
  }}
);
  """.format(
    system_name = systemName,
    components_descriptions = queryComponents,
    event_type = eventName
  )

  return '\n'.join([systemDeclaration, systemRegistration])

def gen_system(parsedFunction):
  funcParamsInfo = parsedFunction['function_params']

  componentAccessTemplate = """{is_const} {param_type}{is_ref} {param_name} = accessor.Get<{param_type}>(str_hash("{param_name}")) """
  componentsAccessors = ""
  for p in funcParamsInfo:
    componentsAccessors = "{} {};\n".format(
      componentsAccessors,
      componentAccessTemplate.format(
        is_const = p['const'],
        param_type = p['type_name'],
        is_ref = p['ref'],
        param_name = p['name']
      )
    )
  componentsAccessors = componentsAccessors[:-1]

  callbackParams = ""
  for p in funcParamsInfo:
    callbackParams = callbackParams + p['name'] + ","
  callbackParams = callbackParams[:-1]

  systemName = parsedFunction['name']
  systemDeclaration = """
static void {system_name}_internal(ComponentsAccessor& accessor)
{{
  {component_accessors}
  {system_name}({callback_execute_params});
}}
  """.format(
    system_name = systemName,
    component_accessors = componentsAccessors,
    callback_execute_params = callbackParams
  )

  queryComponentTemplate = """DESCRIBE_QUERY_COMPONENT("{component_name}", {component_type})"""
  queryComponents = ""
  for p in funcParamsInfo:
    queryComponents = "{} {},".format(
      queryComponents,
      queryComponentTemplate.format(component_name = p['name'], component_type= p['type_name'])
    )
  queryComponents = queryComponents[:-1]

  systemRegistration = """
static const bool {system_name}_desc = Registry::register_cpp_query(
  QueryDescription{{
    .cb = {system_name}_internal,
    .components = {{
      {components_descriptions}
    }}
  }}
);
  """.format(
    system_name = systemName,
    components_descriptions = queryComponents
  )

  return '\n'.join([systemDeclaration, systemRegistration])
#######################################################

def generate_ecs_impl(from_src, includeArgs):
  srcIncludes = [
    "#include <engine/ecs/registry.h>",
    "#include <engine/ecs/components_accessor.h>",
    "#include <EASTL/functional.h>",
    "",
    """#include "{}" """.format(os.path.basename(from_src))
  ]

  generated_srs = ["// this file is generated by ecs codegen", 
                   "// do not edit"]
  generated_srs = generated_srs + srcIncludes
  generated_srs = generated_srs + ["", "using namespace Engine::ECS;"]

  index = clang.cindex.Index.create()
  #clangArgs = ["-x", "c++", "-I", "../../../", "-I", "../../../../deps/eastl-3.17.06/include", "-I", "../../../../deps/glm-0.9.9.8/include", "--std=c++20"]
  clangArgs = ["-x", "c++", "--std=c++20"] + includeArgs

  translation_unit = index.parse(from_src, clangArgs)

  errors = []
  for d in translation_unit.diagnostics:
    if d.severity >= clang.cindex.Diagnostic.Error:
      errors = errors + ["error({}): {} [at {}]".format(d.severity, d.spelling, d.location)]

  if len(errors) > 0:
    print("\n".join(map(str,errors)))
    exit(-1)

  for function in translation_unit.cursor.get_children():
    if function.kind == clang.cindex.CursorKind.FUNCTION_DECL:
      name = function.spelling
      paramsNodes = []
      isEsSystem = False
      isEsEventSystem = False
      isEsQuery = False

      for child in function.get_children():
        if child.kind == clang.cindex.CursorKind.COMPOUND_STMT:
          continue
        elif child.kind == clang.cindex.CursorKind.PARM_DECL:
          paramsNodes = paramsNodes + [child]
        elif child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
          if child.spelling == ES_SYSTEM_TAG:
            isEsSystem = True
          if child.spelling == ES_EVENT_SYSTEM_TAG:
            isEsEventSystem = True
          if child.spelling == ES_QUERY_TAG:
            isEsQuery = True

      if isEsSystem and isEsQuery:
        print("ERROR: {} can't be es_function and es_query in the same time [{}]".format(name, function.location))
        exit(-1)

      if isEsQuery:
        parsedQuery = parse_query(function, paramsNodes)
        code = gen_query(parsedQuery)
        generated_srs = generated_srs + [code]

      if isEsSystem:
        parsedSystem = parse_system(function, paramsNodes)
        code = gen_system(parsedSystem)
        generated_srs = generated_srs + [code]

      if isEsEventSystem:
        parsedSystem = parse_event_system(function, paramsNodes)
        code = gen_event_system(parsedSystem)
        generated_srs = generated_srs + [code]

  final_src = '\n'.join(generated_srs)

  return final_src
