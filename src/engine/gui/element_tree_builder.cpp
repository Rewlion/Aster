#include "element_tree_builder.h"
#include "behavior.h"
#include "constants.h"
#include "react_state.h"

#include <engine/log.h>
#include <engine/qjs/array.h>
#include <engine/qjs/exception.h>
#include <engine/qjs/function.h>
#include <engine/qjs/object.h>
#include <engine/qjs/value.hpp>

#include <optional>

namespace Engine::gui
{
  ElementTreeBuilder::ElementTreeBuilder(BehaviorsStorage& behaviors)
    : m_Behaviors(behaviors)
  {
  }

  Element::Ptr ElementTreeBuilder::buildFromRootUi(const qjs::Value& root_ui)
  {
    if (!root_ui.isObject())
    {
      logerror("ui: rootUI is not type of object.");
      return nullptr;
    }

    return buildElem(root_ui, 0);
  }

  Element::Ptr ElementTreeBuilder::buildStaticElem(const qjs::Value& v, const size_t z_order)
  {
    static size_t id = 0;

    auto params = buildParams(v);

    eastl::vector<Element::Ptr> childs = buildChilds(v, z_order+1);
    Element::Ptr elem {new Element{
      .id = id++,
      .zOrder = z_order,
      .params = std::move(params),
      .object = v.duplicate(),
      .childs = std::move(childs)
    }};

    return elem;
  }

  Element::Ptr ElementTreeBuilder::buildDynamicElem(const qjs::Value& constructor, const size_t z_order)
  {
    const auto buildElem = constructor.as<qjs::FunctionView>();
    if (buildElem.getArgsCount() == 0)
    {
      qjs::Value staticElem = buildElem({}, 0, nullptr);
      if (staticElem.isException())
        qjs::logerror_exception(std::move(staticElem));
      else if (!staticElem.isPlainObject())
        logerror("ui: invalid element: function has to return an object");
      else
      {
        Element::Ptr elem = buildStaticElem(staticElem, z_order);

        if (elem)
          elem->dynamicParams = buildDynamicParams(staticElem, constructor, elem.get());

        return elem;
      }
    }
    else
      logerror("ui: invalid element: function shouldn't have any arguments");

    return nullptr;
  }

  Element::Ptr ElementTreeBuilder::buildElem(const qjs::Value& v, const size_t z_order)
  {
    if (v.isPlainObject())
      return buildStaticElem(v, z_order);

    if (v.isFunction())
      return buildDynamicElem(v, z_order);
    
    logerror("ui: invalid element description: it's not static(Object) nor dynamic(Function)");
    return nullptr;
  }

  Element::Params ElementTreeBuilder::buildParams(const qjs::Value& v) const
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
    params.clipChilds = obj.getBoolOr("clipChilds", false);
    params.text = obj.getTextOr("text", "");
    params.textHalign = obj.getIntEnumOr("textHalign", HorAlignType::None);
    params.textValign = obj.getIntEnumOr("textValign", VerAlignType::None);
    params.fontSize = getFontSize(obj);
    params.cursor = obj.getBoolOr("cursor", false);

    return params;
  }

  Element::DynamicParams ElementTreeBuilder::buildDynamicParams(const qjs::Value& static_elem, const qjs::Value& constructor, Element* elem) const
  {
    auto obj = static_elem.as<qjs::ObjectView>();
    Element::DynamicParams params;

    params.constructor = constructor.duplicate();
    params.observes = getObservedReactStates(static_elem);
    params.observeBtnState = obj.getProperty("observeBtnState");
    params.behaviors = getBehaviors(obj, elem);
    params.onClick = getOnClick(obj);

    return params;
  }

  eastl::vector<ReactStateRegistration> ElementTreeBuilder::getObservedReactStates(const qjs::Value& v) const
  {
    eastl::vector<ReactStateRegistration> states;
    auto obj = v.as<qjs::ObjectView>();

    const qjs::Value observes = obj.getProperty("observe");
    if (observes.isUndefined())
    {
    }
    if (observes.isArray())
    {
      const auto array = observes.as<qjs::ArrayView>();
      const size_t len = array.length();
      for (size_t i = 0; i < len; ++i)
      {
        const qjs::Value val = array[i];
        if (val.isObject())
          states.emplace_back(val);
        else
          logerror("ui: invalid observed state: not an object");
      }
    }
    
    return states;
  }

  PointValue ElementTreeBuilder::getPointValue(const qjs::Value& v) const
  {
    if (v.isNumber())
    {
      return PointValue{ v.as<float>(), PointValue::Type::General };
    }
    else if (v.isPlainObject())
    {
      auto obj = v.as<qjs::ObjectView>();
      const float value = obj.getFloatOr("value", 0.0f);
      PointValue::Type type = obj.getIntEnumOr("type", PointValue::Type::General);

      return PointValue{ value, type };
    }

    logerror("ui: invalid size valie");
    return PointValue{ 0.0f, PointValue::Type::General };
  }

  SizeParam ElementTreeBuilder::getSize(qjs::ObjectView& obj) const
  {
    SizeParam size;

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
          size[i] = getPointValue(sizeVal);
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

  BehaviorsArray ElementTreeBuilder::getBehaviors(qjs::ObjectView& v, Element* elem) const
  {
    BehaviorsArray result;
    qjs::Value behaviors = v.getProperty("behaviors");

    const auto addBehavior = [&result, &elem, this](const BehaviorType type){
      if (type != BehaviorType::None)
      {
        IBehavior* bhv = m_Behaviors.getBehavior(type);
        result.emplace_back(bhv, elem);
      }
      else
        logerror("ui: invalid behavior");
    };

    if (behaviors.isUndefined())
    {
    }
    else if (behaviors.isInt())
    {
      const auto type = v.getIntEnumOr("behaviors", BehaviorType::None);
      addBehavior(type);
    }
    else if (behaviors.isArray())
    {
      qjs::ArrayView array = behaviors.as<qjs::ArrayView>();
      const size_t length = array.length();
      for (size_t i = 0; i < length; ++i)
      {
        qjs::Value v = array[i];
        if (v.isInt())
        {
          const auto type = (BehaviorType) v.asRanged<int>((int)BehaviorType::None, (int)BehaviorType::LAST);
          addBehavior(type);
        }
        else
          logerror("ui: invalid behavior [{}]", i);
      }
    }

    return result;
  }

  qjs::Value ElementTreeBuilder::getOnClick(qjs::ObjectView& obj) const
  {
    qjs::Value onClick = obj.getProperty("onClick");
    if (onClick.isUndefined())
    {}
    else if (!onClick.isFunction())
      logerror("ui: onClick is not a function");
    else if (onClick.getLength() != 0)
      logerror("ui: onClick should not accept any arg");
    else
      return std::move(onClick);

    return {onClick.getContext(), JS_UNDEFINED};
  }

  PointValue ElementTreeBuilder::getFontSize(qjs::ObjectView& obj) const
  {
    qjs::Value fontSize = obj.getProperty("fontSize");
    if (!fontSize.isUndefined())
      return getPointValue(fontSize);
    
    return PointValue{1.0, PointValue::Type::General};
  }

  eastl::vector<Element::Ptr> ElementTreeBuilder::buildChilds(const qjs::Value& v, const size_t z_order)
  {
    eastl::vector<Element::Ptr> childsElems;
    const qjs::Value childs = v.as<qjs::ObjectView>().getProperty("childs");
    
    if (childs.isPlainObject())
    {
      if (Element::Ptr elem = buildElem(childs, z_order))
        childsElems.push_back(std::move(elem));
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
          if (Element::Ptr optElem = buildElem(child, z_order))
            childsElems.push_back(std::move(optElem));
        }
      }
    }

    return childsElems;
  }
}
