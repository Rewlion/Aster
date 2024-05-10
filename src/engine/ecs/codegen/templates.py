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

def generate_query_registration(query_name):
  return "{}_queryReg".format(query_name)

def generate_query_id(query_name):
  return "{}_queryId".format(query_name)

def generate_query_id_declaration(query_name, components):
  return """
const static DirectQueryRegistration {query_reg_name}{{
  {{
{query_components}
  }},
  "{query_name}"}};
const static query_id_t {query_id_name} = {query_reg_name}.getId();
""".format(
  query_id_name = generate_query_id(query_name),
  query_reg_name = generate_query_registration(query_name),
  query_components = generate_query_components_description(components, 2),
  query_name = query_name
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
  componentAccessTemplate = """{space}{is_const}{param_type}{is_ref} {param_name} = accessor.get<{param_type}>(compile_ecs_name_hash("{param_name}"));"""
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
  {ecs_namespace}::get_registry().query({query_id_t}, [&](ComponentsAccessor& accessor)
  {{
{components_access}
    cb({callback_execute_args});
  }});
}}
""".format(
  query_name = name,
  functor_params = generate_functor_params(components, 2),
  ecs_namespace = consts.ECS_NAMESPACE,
  query_id_t = generate_query_id(name),
  components_access = generate_components_access(components, 2),
  callback_execute_args = ",".join([c.name for c in components])
)

def generate_direct_query_body(eid, name, components):
  return """
void {query_name} (EntityId {eid_name}, eastl::function<
  void(
{functor_params})> cb)
{{
  {ecs_namespace}::get_registry().query({eid_name}, [&](ComponentsAccessor& accessor)
  {{
{components_access}
    cb({callback_execute_args});
  }});
}}
""".format(
  query_name = name,
  eid_name = eid,
  functor_params = generate_functor_params(components, 2),
  ecs_namespace = consts.ECS_NAMESPACE,
  components_access = generate_components_access(components, 2),
  callback_execute_args = ",".join([c.name for c in components])
)

def generate_event_system_internal(name, casted_event_type, components):
  cbArgs = ",".join([c.name for c in components])
  if len(cbArgs) > 0:
    cbArgs = ', ' + cbArgs

  return """
static void {system_name}_internal(Event* event, ComponentsAccessor& accessor)
{{
  {casted_event_type}* casted_event = reinterpret_cast<{casted_event_type}*>(event);
{components_access}
  {system_name}(*casted_event{callback_execute_args});
}}
""".format(
  system_name = name,
  casted_event_type = casted_event_type,
  components_access = generate_components_access(components,1),
  callback_execute_args = cbArgs
)

def generate_event_system_registration(name, event_name, components):
  return """
static EventSystemRegistration {system_name}_registration(
  {system_name}_internal,
  compile_ecs_name_hash("{event_type}"),
  {{
{components_descriptions}
  }},
  "{system_name}"
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
  }},
  "{system_name}"
);
""".format(
  system_name = name,
  components_descriptions = generate_query_components_description(components,2)
)


def generate_fg_order_before(node_name):
  return f"""    reg.orderMeBefore("{node_name}");"""


def generate_fg_order_after(node_name):
  return f"""    reg.orderMeAfter("{node_name}");"""


def generate_fg_create_buffer(name, usage, state, size):
  return f"""    auto {name} = reg.createBuffer(
      "{name}",
      gapi::BufferAllocationDescription{{
        .size = {size},
        .usage = {usage}
      }},
      {state}
    );"""


def generate_fg_import_buffer_producer(name, import_fn):
  return f"""    auto {name} = reg.importBufferProducer("{name}", {import_fn});"""


def generate_fg_modify_buffer(name, name_alias, state):
  return f"""    auto {name_alias if name_alias != "" else name} = reg.modifyBuffer("{name}", {state});"""


def generate_fg_read_optional_buffer(name, name_alias, state, optional):
  return f"""    auto {name_alias if name_alias != "" else name} = reg.readBuffer("{name}", {state}{f", {optional}" if optional != "" else ""});"""


def generate_fg_rename_buffer(from_name, to_name, state):
  return f"""    auto {to_name} = reg.renameBuffer("{from_name}", "{to_name}", {state});"""


def generate_fg_create_texture(
  name, tex_format, extent, mip_levels,
  array_layers, samples_per_pixel,
  usage, init_state):
  return f"""
    auto {name} = reg.createTexture("{name}",
      gapi::TextureAllocationDescription{{
        .format =          {tex_format if tex_format != "" else "gapi::TextureFormat::R8G8B8A8_UNORM"},
        .extent =          {extent if extent != "" else "{0,0,0}"},
        .mipLevels =       {mip_levels if mip_levels != "" else "1"},
        .arrayLayers =     {array_layers if array_layers != "" else "1"},
        .samplesPerPixel = {samples_per_pixel if samples_per_pixel != "" else "gapi::TextureSamples::s1"},
        .usage =           {usage if usage != "" else "gapi::TextureUsage::TEX_USAGE_NONE"}
      }},
      {init_state if init_state != "" else "gapi::TextureState::Undefined"}
    );
"""


def generate_fg_import_texture_producer(name, import_fn):
  return f"""    auto {name} = reg.importTextureProducer("{name}", {import_fn});"""


def generate_fg_modify_texture(name, name_alias, state):
  return f"""    auto {name_alias if name_alias != "" else name} = reg.modifyTexture("{name}", {state});"""


def generate_fg_read_optional_texture(name, name_alias, state, optional):
  return f"""    auto {name_alias if name_alias != "" else name} = reg.readTexture("{name}", {state}{f", {optional}" if optional != "" else ""});"""


def generate_fg_read_timeline_texture(name, name_alias, state, timeline):
  return f"""    auto {name_alias if name_alias != "" else name} = reg.readTexture("{name}", {state}{f", {timeline}" if timeline != "" else ""});"""


def generate_fg_rename_texture(from_name, to_name, state):
  return f"""    auto {to_name} = reg.renameTexture("{from_name}", "{to_name}", {state});"""


def generate_fg_create_blob(name, type):
  return f"""    auto {name} = reg.createBlob<{type}>("{name}");"""


def generate_fg_read_blob(name, type):
  return f"""    auto {name} = reg.readBlob<{type}>("{name}");"""


def generate_fg_modify_blob(name, type):
  return f"""    auto {name} = reg.modifyBlob<{type}>("{name}");"""


def generate_fg_target(name, load_op, store_op, clear_color):
  return f"""      .addTarget({name}, {load_op}, {store_op}, {clear_color})"""


def generate_fg_depth(name):
  return f"""      .addDepth({name})"""
                      

def generate_fg_rodepth(name, load_op):
  return f"""      .addRODepth({name}, {load_op})"""


def generate_fg_rwdepth(name, load_op, store_op):
  return f"""      .addRWDepth({name}, {load_op}, {store_op})"""


def generate_fg_renderpass(targets, depth_stencil):
  res = "\n".join(["    reg.requestRenderPass()"] + targets + [depth_stencil+";\n" if depth_stencil != "" else "    ;\n"])

  return res


def generate_fg_exec_fn_bridge(encoder_name, capture_list, args_list, exec_fn, exec_actions):
  actions = "\n" + exec_actions if exec_actions != "" else ""
  return f"""
    return [{",".join(capture_list)}](gapi::CmdEncoder& {encoder_name})
    {{{actions}
      {exec_fn}({", ".join(args_list)});
    }};"""


def generate_fg_no_exec_fn():
  return "    return [](gapi::CmdEncoder&){};"


def generate_fg_bind_shvar(res, var):
  return f"""      tfx::set_extern("{var}", {res}.get());"""


def generate_fg_render_size_access(var):
  return f"""    fg::dsl::AccessDecorator {var}{{__renderSize__}};"""


def generate_fg_node(name, build_actions, exec_fn_action, has_render_size_access):
  renderSize = "\n    const uint2 __renderSize__ = reg.getRenderSize();\n" if has_render_size_access else ""
  registrationFnName = f"mk_fg_node_{name}"
  return f"""
//Engine::OnFrameGraphInit handler
static
void {registrationFnName}(Event*, ComponentsAccessor&)
{{
  fg::register_node("{name}", FG_FILE_DECL, [](fg::Registry& reg)
  {{ {renderSize}
{build_actions}
{exec_fn_action}
  }});
}}

static
EventSystemRegistration {registrationFnName}_registration(
  {registrationFnName},
  compile_ecs_name_hash("OnFrameGraphInit"),
  {{
  }},
  "{registrationFnName}"
);
"""


def generate_reflection_property(comp_class, member, name, type):
  return f"""ClassField{{
    .offset = offsetof({comp_class}, {member}),
    .name = "{name}",
    .type = {type}
  }},
"""


def generate_reflection_class(comp_class, comp_refl_class, properties):
  genProperties = ''.join([generate_reflection_property(comp_class, p.member, p.name, p.type ) for p in properties])
  return f"""
class {comp_refl_class} : public Class
{{
  public:
    auto getFieldsBegin() const -> const ClassField* override
    {{
      return m_Fields;
    }}
    
    auto getFieldsCount() const -> size_t override
    {{
      return m_FieldsCount;
    }}
    
  private:
    static ClassField m_Fields[];
    static size_t m_FieldsCount;
}};

ClassField {comp_refl_class}::m_Fields[] = {{
  {genProperties}
}};

size_t {comp_refl_class}::m_FieldsCount = std::size({comp_refl_class}::m_Fields);

auto {comp_class}::getClass() -> const Class*
{{
  return getStaticClass();
}}

auto {comp_class}::getStaticClass() -> const Class*
{{
  static {comp_refl_class} c;
  return &c;
}}
"""