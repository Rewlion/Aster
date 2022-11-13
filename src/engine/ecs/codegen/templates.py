import consts

def generate_spaces(nesting):
  return "".join(["  " for _ in range(nesting)])

def generate_query_components_description(components, nesting):
  query_component_template = """{space}DESCRIBE_QUERY_COMPONENT("{component_name}", {component_type})"""
  queryComponents = ""
  components = [
    query_component_template.format(
      space = generate_spaces(nesting),
      component_name = c.name, 
      component_type = c.typeName)
    for c in components]
  
  queryComponents = ',\n'.join(components)
  return queryComponents

def generate_query_id(query_name):
  return "{}_queryId".format(query_name)

def generate_query_id_declaration(query_name, components):
  return """
const static query_id {query_id_name} = Registry::registerDirectQuery(
  DirectQueryDescription{{
    .components = {{
{query_components}
    }}
  }}
);
""".format(
  query_id_name = generate_query_id(query_name),
  query_components = generate_query_components_description(components, 3)
)

def generate_functor_params(components, nesting):
  functorParamTemplate = "{space}{is_const}{param_type}{is_ref} {param_name}"
  functorParams = [functorParamTemplate.format(
    space = generate_spaces(nesting),
    param_name = c.name,
    is_const = c.constPrefix,
    param_type = c.typeName,
    is_ref = c.refPrefix
  ) for c in components]

  functorParams = ",\n".join(functorParams)
  return functorParams

def generate_components_access(components, nesting):
  componentAccessTemplate = """{space}{is_const}{param_type}{is_ref} {param_name} = accessor.get<{param_type}>(str_hash("{param_name}"));"""
  componentsAccessors = [
    componentAccessTemplate.format(
      space = generate_spaces(nesting),
      param_name = c.name,
      is_const = c.constPrefix,
      param_type = c.typeName,
      is_ref = c.refPrefix
  ) for c in components]
  componentsAccessors = '\n'.join(componentsAccessors)
  return componentsAccessors

def generate_query_body(name, components):
  return """
void {query_name} (eastl::function<
  void(
{functor_params})> cb)
{{
  {ecs_namespace}::manager.query({query_id}, [&](ComponentsAccessor& accessor)
  {{
{components_access}
    cb({callback_execute_args});
  }});
}}
""".format(
  query_name = name,
  functor_params = generate_functor_params(components, 2),
  ecs_namespace = consts.ECS_NAMESPACE,
  query_id = generate_query_id(name),
  components_access = generate_components_access(components, 2),
  callback_execute_args = ",".join([c.name for c in components])
)

def generate_event_system_internal(name, event_name, components):
  cbArgs = ",".join([c.name for c in components])
  if len(cbArgs) > 0:
    cbArgs = ', ' + cbArgs

  return """
static void {system_name}_internal(Event* event, ComponentsAccessor& accessor)
{{
  {event_type}* casted_event = reinterpret_cast<{event_type}*>(event);
{components_access}
  {system_name}(*casted_event{callback_execute_args});
}}
""".format(
  system_name = name,
  event_type = event_name,
  components_access = generate_components_access(components,1),
  callback_execute_args = cbArgs
)

def generate_event_system_registration(name, event_name, components):
  return """
static EventSystemRegistration {system_name}_registration(
  {system_name}_internal,
  str_hash("{event_type}"),
  {{
{components_descriptions}
  }}
);
""".format(
  system_name = name,
  components_descriptions = generate_query_components_description(components,2),
  event_type = event_name
)

def generate_system_internal(name, components):
  return """
static void {system_name}_internal(ComponentsAccessor& accessor)
{{
{component_access}
  {system_name}({callback_execute_args});
}}
""".format(
  system_name = name,
  component_access = generate_components_access(components, 1),
  callback_execute_args = ",".join([c.name for c in components])
)

def generate_system_registration(name, components):
  return """
static SystemRegistration {system_name}_registration(
  {system_name}_internal,
  {{
{components_descriptions}
  }}
);
""".format(
  system_name = name,
  components_descriptions = generate_query_components_description(components,2)
)