#include "element_tree_builder.h"
#include "constants.h"

#include <engine/log.h>
#include <engine/qjs/array.h>
#include <engine/qjs/exception.h>
#include <engine/qjs/function.h>
#include <engine/qjs/object.h>
#include <engine/qjs/value.hpp>

#include <optional>

namespace Engine::gui
{
  std::optional<Element> ElementTreeBuilder::buildFromRootUi(const qjs::Value& root_ui, const qjs::Value& global)
  {
    if (!global.isObject())
    {
      logerror("ui: global is not type of object.");
      return std::nullopt;
    }

    if (!root_ui.isObject())
    {
      logerror("ui: rootUI is not type of object.");
      return std::nullopt;
    }

    m_Global = &global;

    return buildElem(root_ui);
  }

  std::optional<Element> ElementTreeBuilder::buildStaticElem(const qjs::Value& v) const
  {
    auto params = buildParams(v);
    if (!params.has_value())
      return std::nullopt;

    eastl::vector<Element> childs = buildChilds(v);
    return Element{
      .params = std::move(params.value()),
      .childs = std::move(childs)
    };
  }

  std::optional<Element> ElementTreeBuilder::buildDynamicElem(const qjs::Value& v) const
  {
    const auto buildElem = v.as<qjs::FunctionView>();
    if (buildElem.getArgsCount() == 0)
    {
      qjs::Value r = buildElem(*m_Global, 0, nullptr);
      if (r.isException())
        qjs::logerror_exception(std::move(r));
      else if (!r.isPlainObject())
        logerror("ui: invalid element: function has to return an object");
      else
        return buildStaticElem(r);
    }
    else
      logerror("ui: invalid element: function shouldn't have any arguments");

    return std::nullopt;
  }

  std::optional<Element> ElementTreeBuilder::buildElem(const qjs::Value& v) const
  {
    if (v.isPlainObject())
      return buildStaticElem(v);

    if (v.isFunction())
      return buildDynamicElem(v);
    
    logerror("ui: invalid element description: it's not static(Object) nor dynamic(Function)");
    return std::nullopt;
  }

  std::optional<Element::Params> ElementTreeBuilder::buildParams(const qjs::Value& v) const
  {
    auto obj = v.as<qjs::ObjectView>();
    Element::Params params;

    params.size = getSize(obj);
    params.pos = getPos(obj);
    params.gap = obj.getIntOr("gap", 0.0);
    params.flex = obj.getFloatOr("flex", 1.0);
    params.flow = obj.getIntEnumOr("flow", Flow::None);
    params.color = getColor(obj);
    params.render = obj.getIntEnumOr("render", RenderType::None);
    params.halign = obj.getIntEnumOr("halign", HorAlignType::None);
    params.valign = obj.getIntEnumOr("valign", VerAlignType::None);

    return params;
  }

  SizeParam ElementTreeBuilder::getSize(qjs::ObjectView& obj) const
  {
    SizeParam size;
    size[0] = 0;
    size[1] = 0;

    const auto getIntValue = [](const qjs::Value& val) {
      if (!val.isInt())
      {
        logerror("ui: invalid size: pixel value isn't INT");
        return 0;
      }

      return val.as<int>();
    };

    const qjs::Value v = obj.getProperty("size");
    if (v.isUndefined())
    {
    }
    else if (v.isArray())
    {
      const auto array = v.as<qjs::ArrayView>();
      if (array.length() == 2)
      {
        for (size_t i = 0; i < 2; ++i)
        {
          const qjs::Value sizeVal = array[i];
          size[i] = getIntValue(sizeVal);
        }
      }
      else
        logerror("ui: invalid size: array length != 2");
    }
    else
      logerror("ui: invalid size: it has to be [number, number]");

    return size;
  }

  int2 ElementTreeBuilder::getPos(qjs::ObjectView& obj) const
  {
    int2 pos = {0,0};

    const qjs::Value v = obj.getProperty("pos");
    if (v.isUndefined())
      return pos;

    if (!v.isArray())
    {
      logerror("ui: pos is not array");
      return pos;
    }

    auto array = v.as<qjs::ArrayView>();
    if (array.length() != 2)
    {
      logerror("ui: pos should be array of 2");
      return pos;
    }
    
    const auto getPosValue = [&array](int i) {
      qjs::Value sizeVal = array[i];
      if (!sizeVal.isInt())
      {
        logerror("ui: pos[{}] isn't INT", i);
        return 0;
      }

      return sizeVal.as<int>();
    };

    pos[0] = getPosValue(0);
    pos[1] = getPosValue(1);
    return pos;
  }

  ColorParam ElementTreeBuilder::getColor(qjs::ObjectView& obj) const
  {
    ColorParam color{255,255,255,255};

    const qjs::Value v = obj.getProperty("color");
    if (v.isUndefined())
    {
    }
    else if (v.isInt())
    {
      color.raw = v.as<int>();
      return color;
    }
    else if (v.isArray())
    {
      auto array = v.as<qjs::ArrayView>();
      
      const auto getValue = [&array](int i) {
        qjs::Value sizeVal = array[i];
        if (!sizeVal.isInt())
        {
          logerror("ui: color[{}] isn't INT", i);
          return 0;
        }
        return sizeVal.as<int>();
      };

      if (array.length() != 3 && array.length() != 4)
        logerror("ui: color has to be array of 3 or 4");
      else
      {
        color.val.r = getValue(0);
        color.val.g = getValue(1);
        color.val.b = getValue(2);
        if (array.length() == 4)
          color.val.a = getValue(3);
        else
          color.val.a = 255;
      }
    }

    return color;
  }

  eastl::vector<Element> ElementTreeBuilder::buildChilds(const qjs::Value& v) const
  {
    eastl::vector<Element> childsElems;
    const qjs::Value childs = v.as<qjs::ObjectView>().getProperty("childs");
    
    if (childs.isPlainObject())
    {
      std::optional<Element> elem = buildElem(childs);
      if (elem.has_value())
        childsElems.push_back(std::move(elem.value()));
    }
    else if (childs.isArray())
    {
      const auto array = childs.as<qjs::ArrayView>();
      const size_t len = array.length();
      for (size_t i = 0; i < len; ++i)
      {
        const qjs::Value child = array[i];
        if (!child.isUndefined())
        {
          const auto optElem = buildElem(child);
          if (optElem.has_value())
            childsElems.push_back(std::move(optElem.value()));
        }
      }
    }

    return childsElems;
  }
}
