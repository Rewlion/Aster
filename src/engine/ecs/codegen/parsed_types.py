import cursor_utils as utils
import templates

class SystemParam:
  def __init__(self, param_cursor):
    cursorType = param_cursor.type
    self.name = param_cursor.spelling
    self.typeName = utils.get_cursor_type_name(cursorType)
    self.constPrefix = "const " if utils.is_const_type(cursorType) else ""
    self.refPrefix = "&" if utils.is_ref_type(cursorType) else ""

class System:
  def parseParams(self, param_cursors):
    params = []
    for c in param_cursors:
      params = params + [SystemParam(c)]
    return params

  def __init__(self, fn_cursor, param_cursors):
    self.name = fn_cursor.spelling
    self.params = self.parseParams(param_cursors)
  
  def generate(self):
    return "\n".join([
      templates.generate_system_internal(self.name, self.params),
      templates.generate_system_registration(self.name, self.params)
    ])

class EventSystem(System):
  def __init__(self, fn_cursor, param_cursors):
    self.name = fn_cursor.spelling
    self.params = self.parseParams(param_cursors[1:])
    
    eventCursor = param_cursors[0]
    self.eventName = utils.get_real_type(eventCursor.type).get_declaration().spelling
  
  def generate(self):
    return "\n".join([
      templates.generate_event_system_internal(self.name, self.eventName, self.params),
      templates.generate_event_system_registration(self.name, self.eventName, self.params)
    ])

class QueryParam:
  def __init__(self, param_type_cursor, name):
    self.name = name
    self.typeName = utils.get_real_type_name(utils.get_real_type(param_type_cursor))
    self.constPrefix = "const " if utils.is_const_type(param_type_cursor) else ""
    self.refPrefix = "&" if utils.is_ref_type(param_type_cursor) else ""
    

class Query:
  # eastl::function<void(const Float2& n, Float3 k)>& cb
  # here cb == params[0]
  # cb has 4 childrens:
  #   eastl
  #   function
  #   n
  #   k
  def parseTemplateParams(self, template_cursor, template_param_names):
    params = []
    for i, param_type_cursor in enumerate(template_cursor.argument_types()):
      params = params + [QueryParam(param_type_cursor, template_param_names[i])]
    return params

  def parseParams(self, param_cursors):
    paramsCount = len(param_cursors)

    if paramsCount != 1:
      raise ValueError("query has invalid number params:{}, 1 expected [{}]"
        .format(paramsCount, param_cursors.location))

    paramCursor = param_cursors[0]
    if not utils.is_eastl_functor(paramCursor):
      raise ValueError("query has invalid param:{}, eastl::function expected [{}]".format(paramCursor.spelling, param_cursor.location))

    templateParamNames = utils.get_template_param_names(paramCursor)
    realType = utils.get_template_real_type(paramCursor.type)
    template = realType.get_template_argument_type(0)

    return self.parseTemplateParams(template, templateParamNames)
   
  def __init__(self, fn_cursor, param_cursors):
    self.name = fn_cursor.spelling
    self.params = self.parseParams(param_cursors)
  
  def generate(self):
    return "\n".join([
      templates.generate_query_id_declaration(self.name, self.params),
      templates.generate_query_body(self.name, self.params)
    ])
