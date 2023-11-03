import clang.cindex
import cursor_utils as utils
import templates
import re


class FgParsingContext:
  def __init__(self, exec_functions_with_params):
    self.execFunctionsWithParams = exec_functions_with_params
    self.generateRenderSizeAccess = False
    self.accessedResources = set()

  def getExecFunction(self, name):
    if name in self.execFunctionsWithParams:
      return self.execFunctionsWithParams[name][0]
    else:
      raise ValueError(f"Unknown execution function {name}")

  
  def markResourceAccess(self, name):
    self.accessedResources.add(name)


  def hasResourceAccess(self, name):
    return name in self.accessedResources


  def getResourceAccess(self, name):
    return f"\"{name}\"" if not self.hasResourceAccess(name) else name


  def markRenderSizeAccess(self):
    self.generateRenderSizeAccess = True


  def hasRenderSizeAccess(self):
    return self.generateRenderSizeAccess


class TemplateParamExtractor:
  def extractParams(self, field_or_type):
    fieldType = field_or_type if type(field_or_type).__name__ == "Type" else field_or_type.type
    paramsCount = fieldType.get_num_template_arguments()

    for i in range(paramsCount):
      param = fieldType.get_template_argument_type(i)
      paramDecl = param.get_declaration()
      
      paramName = paramDecl.spelling
      extractMethod = f"extract{paramName}"

      if hasattr(self, extractMethod):
        getattr(self, extractMethod)(param)
      else:
        raise ValueError(f"Parameter `{paramName}` doesn't have an extraction method")
  

  def extractTemplateArgumentsAsIs(self, type):
      return re.findall(r'\<([^>]*)\>', type.spelling)[0]
  

  def extractStub(self, tmpl_arg_type):
    pass
    

  def extractNameAlias(self, tmpl_arg_type):
    self.nameAlias = self.extractTemplateArgumentsAsIs(tmpl_arg_type).replace('\"', '')


  def extractName(self, tmpl_arg_type):
      self.name = self.extractTemplateArgumentsAsIs(tmpl_arg_type).replace('\"', '')

  
  def extractNameFrom(self, tmpl_arg_type):
    self.fromName = self.extractTemplateArgumentsAsIs(tmpl_arg_type).replace('\"', '')

  
  def extractNameTo(self, tmpl_arg_type):
    self.toName = self.extractTemplateArgumentsAsIs(tmpl_arg_type).replace('\"', '')


  def extractBufferUsage(self, tmpl_arg_type):
    self.bufferUsage = self.extractTemplateArgumentsAsIs(tmpl_arg_type)

  
  def extractBufferState(self, tmpl_arg_type):
    self.bufferState = self.extractTemplateArgumentsAsIs(tmpl_arg_type)


  def extractBufferSize(self, tmpl_arg_type):
    self.bufferSize = self.extractTemplateArgumentsAsIs(tmpl_arg_type)


  def extractTextureFormat(self, tmpl_arg_type):
    self.textureFormat = self.extractTemplateArgumentsAsIs(tmpl_arg_type)


  def extractTextureState(self, tmpl_arg_type):
    self.textureState = self.extractTemplateArgumentsAsIs(tmpl_arg_type)

  
  def extractExtent(self, tmpl_arg_type):
    sizeType = tmpl_arg_type.get_template_argument_type(0)
    sizeTypeName = sizeType.get_declaration().spelling
    if sizeTypeName == "AbsSize":
      self.extent = self.extractTemplateArgumentsAsIs(sizeType)
    elif sizeTypeName == "RelativeSize":
      self.extent = "uint3(__renderSize__, 1)"
      self.context.markRenderSizeAccess()
    elif sizeTypeName == "RelativeSizeMul":
      self.extent = f"uint3(__renderSize__ * {self.extractTemplateArgumentsAsIs(sizeType)}, 1)"
      self.context.markRenderSizeAccess()
    elif sizeTypeName == "RelativeSizeDiv":
      self.extent = f"uint3(__renderSize__ / {self.extractTemplateArgumentsAsIs(sizeType)}, 1)"
      self.context.markRenderSizeAccess()
    else:
      raise ValueError(f"Unknown extent type `{sizeTypeName}`")


  def extractMipLevels(self, tmpl_arg_type):
    self.mipLevels = self.extractTemplateArgumentsAsIs(tmpl_arg_type)


  def extractArrayLayers(self, tmpl_arg_type):
    self.arrayLayers = self.extractTemplateArgumentsAsIs(tmpl_arg_type)
  
  
  def extractSamplesPerPixel(self, tmpl_arg_type):
    self.samplesPerPixel = self.extractTemplateArgumentsAsIs(tmpl_arg_type)
    

  def extractTextureUsage(self, tmpl_arg_type):
    usage = self.extractTemplateArgumentsAsIs(tmpl_arg_type)
    self.textureUsage = self.textureUsage + f" | {usage}" if self.textureUsage != "" else usage


  def extractPersistentStorage(self, tmpl_arg_type):
    self.persistentStorage = "fg::PERSISTENT"
    

  def extractOptional(self, tmpl_arg_type):
    self.optional = "true"


  def extractNotOptional(self, tmpl_arg_type):
    self.optional = "false"


  def extractImportFnName(self, tmpl_arg_type):
      self.importFn = self.extractTemplateArgumentsAsIs(tmpl_arg_type).replace('\"', '')


  def extractTimeline(self, tmpl_arg_type):
    self.timeline = self.extractTemplateArgumentsAsIs(tmpl_arg_type)


  def extractBlobType(self, tmpl_arg_type):
    self.blobType = self.extractTemplateArgumentsAsIs(tmpl_arg_type)

  
  def extractLoadOp(self, tmpl_arg_type):
      self.loadOp = self.extractTemplateArgumentsAsIs(tmpl_arg_type)

  
  def extractStoreOp(self, tmpl_arg_type):
      self.storeOp = self.extractTemplateArgumentsAsIs(tmpl_arg_type)


  def extractClearColorValue(self, tmpl_arg_type):
      color = tmpl_arg_type.get_template_argument_type(0)
      colorType = color.get_declaration().spelling
      if colorType == "UClearColorValue":
        self.clearColor = f"gapi::ClearColorValue{{uint32_t{{{self.extractTemplateArgumentsAsIs(color)}}}}}"
      elif colorType == "FClearColorValue":
        flVal = self.extractTemplateArgumentsAsIs(color).replace("\"", "")
        self.clearColor = f"gapi::ClearColorValue{{float{{{flVal}}}}}"
      else:
        raise ValueError(f"Unknown clear color type `{colorType}`")


BUILD_ACTION = "build_action"
EXEC_ACTION = "exec_action"
EXEC_FN_ACTION = "exec_fn_action"

class BuildAction(TemplateParamExtractor):
  def getActionType(self):
    return BUILD_ACTION


class ExecAction(TemplateParamExtractor):
  def getActionType(self):
    return EXEC_ACTION


class ExecFnAction(TemplateParamExtractor):
  def getActionType(self):
    return EXEC_FN_ACTION


class OrderAfterAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = field_cursor.spelling
  

  def generate(self):
    return templates.generate_fg_order_after(self.name)


class OrderBeforeAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = field_cursor.spelling


  def generate(self):
    return templates.generate_fg_order_before(self.name)


class CreateBufferAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.bufferUsage = ""
    self.bufferState = ""
    self.bufferSize = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)

  def generate(self):
    return templates.generate_fg_create_buffer(self.name, self.bufferUsage, self.bufferState, self.bufferSize)
  

class CreateTextureAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.textureFormat = ""
    self.extent = ""
    self.mipLevels = ""
    self.arrayLayers = ""
    self.samplesPerPixel = ""
    self.textureUsage = ""
    self.textureState = ""
    self.persistentStorage = ""
    self.context = context
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)


  def generate(self):
    return templates.generate_fg_create_texture(
      self.name, self.textureFormat, self.extent, self.mipLevels,
      self.arrayLayers, self.samplesPerPixel,
      self.textureUsage, self.textureState, self.persistentStorage)


class ModifyBufferAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.bufferState = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)


  def generate(self):
    return templates.generate_fg_modify_buffer(self.name, self.bufferState)


class ReadOptionalBufferAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.bufferState = ""
    self.optional = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)


  def generate(self):
    return templates.generate_fg_read_optional_buffer(self.name, self.bufferState, self.optional)



class ImportTextureProducerAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.importFn = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)


  def generate(self):
    return templates.generate_fg_import_texture_producer(self.name, self.importFn)


class ReadOptionalTextureAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.nameAlias = ""
    self.textureState = ""
    self.optional = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name if self.nameAlias == "" else self.nameAlias)


  def generate(self):
    return templates.generate_fg_read_optional_texture(self.name,self.nameAlias, self.textureState, self.optional)


class ReadTimelineTextureAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.nameAlias = ""
    self.textureState = ""
    self.timeline = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name if self.nameAlias == "" else self.nameAlias)


  def generate(self):
    return templates.generate_fg_read_timeline_texture(self.name, self.nameAlias, self.textureState, self.timeline)


class RenameTextureAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.fromName = ""
    self.toName = ""
    self.textureState = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.toName)
    

  
  def generate(self):
    return templates.generate_fg_rename_texture(self.fromName, self.toName, self.textureState)


class ReadBlobAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.blobType = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)


  def generate(self):
    return templates.generate_fg_read_blob(self.name, self.blobType)
  

class ModifyBlobAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.blobType = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)


  def generate(self):
    return templates.generate_fg_modify_blob(self.name, self.blobType)
  

class CreateBlobAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.blobType = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name)


  def generate(self):
    return templates.generate_fg_create_blob(self.name, self.blobType)


class ModifyTextureAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.name = ""
    self.nameAlias = ""
    self.textureState = ""
    self.extractParams(field_cursor)
    context.markResourceAccess(self.name if self.nameAlias == "" else self.nameAlias)


  def generate(self):
    return templates.generate_fg_modify_texture(self.name, self.nameAlias, self.textureState)


class ExecFunctionAction(ExecFnAction):
  def __init__(self, field_cursor, context):
    self.context = context
    self.name = ""
    self.fieldCursor = field_cursor
    self.extractParams(field_cursor)


  def generate(self, exec_actions):
    execFn = self.context.getExecFunction(self.name)
    captureList = []
    argsList = []
    encoderName = ""
    for arg in execFn.get_arguments():
      argTypeName = utils.get_cursor_type_name(arg.type)
      if argTypeName != "gapi::CmdEncoder":
        captureList = captureList + [arg.spelling]
        argsList = argsList + [f"{arg.spelling}.get()"]
      else:
        encoderName = arg.spelling
        argsList = argsList + [encoderName]
    return templates.generate_fg_exec_fn_bridge(encoderName, captureList, argsList, self.name, exec_actions)


class NoExecFunctionAction(ExecFnAction):
  def __init__(self, field_cursor, context):
    pass


  def generate(self, exec_actions):
    return templates.generate_fg_no_exec_fn()


class Target(BuildAction):
  def __init__(self, tmpl_arg_type, context):
    self.name = ""
    self.loadOp = ""
    self.storeOp = ""
    self.clearColor = ""
    self.context = context
    self.extractParams(tmpl_arg_type)


  def generate(self):
    return templates.generate_fg_target(self.context.getResourceAccess(self.name), self.loadOp, self.storeOp, self.clearColor)


class Depth(BuildAction):
  def __init__(self, tmpl_arg_type, context):
    self.name = ""
    self.context = context
    self.extractParams(tmpl_arg_type)

  
  def generate(self):
    return templates.generate_fg_depth(self.context.getResourceAccess(self.name))


class RODepth(BuildAction):
  def __init__(self, tmpl_arg_type, context):
    self.name = ""
    self.loadOp = ""
    self.context = context
    self.extractParams(tmpl_arg_type)

  
  def generate(self):
    return templates.generate_fg_rodepth(self.context.getResourceAccess(self.name), self.loadOp)


class RWDepth(BuildAction):
  def __init__(self, tmpl_arg_type, context):
    self.name = ""
    self.loadOp = ""
    self.storeOp = ""
    self.context = context
    self.extractParams(tmpl_arg_type)


  def generate(self):
    name = f"\"{self.name}\"" if not self.context.hasResourceAccess(self.name) else self.name
    return templates.generate_fg_rwdepth(name, self.loadOp, self.storeOp)

class RenderPassAction(BuildAction):
  def __init__(self, field_cursor, context):
    self.targets = []
    self.depthStencil = None
    self.context = context
    self.extractParams(field_cursor)


  def extractTarget(self, tmpl_arg_type):
    self.targets.append(Target(tmpl_arg_type, self.context))

  
  def extractDepth(self, tmpl_arg_type):
    self.depthStencil = Depth(tmpl_arg_type, self.context)


  def extractRODepth(self, tmpl_arg_type):
    self.depthStencil = RODepth(tmpl_arg_type, self.context)


  def extractRWDepth(self, tmpl_arg_type):
    self.depthStencil = RWDepth(tmpl_arg_type, self.context)

  
  def generate(self):
    targets = [t.generate() for t in self.targets]
    depthStencil = self.depthStencil.generate() if self.depthStencil is not None else ""
    return templates.generate_fg_renderpass(targets, depthStencil)


class FgNode:
  def __init__(self, struct_cursor, exec_functions):
    self.name = struct_cursor.spelling
    self.context = FgParsingContext(exec_functions)
    self.buildActions, self.execActions, self.execFnAction = self.parseActions(struct_cursor)

  def parseActions(self, struct_cursor):
    buildActions = []
    execActions = []
    execFnAction = None
    for field in struct_cursor.get_children():
      if field.kind == clang.cindex.CursorKind.FIELD_DECL:
        actionName = f"{field.type.get_declaration().spelling}Action"
        if actionName in globals():
          action = globals()[actionName](field, self.context)
          actionType = action.getActionType()
          if actionType == BUILD_ACTION:
            buildActions.append(action)
          elif actionType == EXEC_ACTION:
            execActions.append(action)
          elif actionType == EXEC_FN_ACTION:
            execFnAction = action
        
    return buildActions, execActions, execFnAction
  

  def generate(self):
    buildActions = "\n".join([action.generate() for action in self.buildActions])
    execActions = "\n".join([action.generate() for action in self.execActions])
    execFnAction = self.execFnAction.generate(execActions)
    return templates.generate_fg_node(self.name, buildActions, execFnAction, self.context.hasRenderSizeAccess())