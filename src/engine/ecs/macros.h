#pragma once

#ifdef __clang__
  #define ECS_QUERY() [[clang::annotate("ecs_query")]]
  #define ECS_SYSTEM() [[clang::annotate("ecs_system")]]
  #define ECS_EVENT_SYSTEM() [[clang::annotate("ecs_event_system")]]

  #define ECS_DESCRIBE_QUERY(query_name, params) \
    ECS_QUERY()\
    static\
    void query_name(eastl::function<void(params)>);

  #define ECS_COMP_GETTER_(type, name)\
    static type* get_ ## name()\
    {\
      type* comp = nullptr;\
      query_ ## name([&](type& val)\
      {\
        comp = &val;\
      });\
      return comp;\
    }

  #define ECS_COMP_GETTER(type, name)\
    ECS_DESCRIBE_QUERY(query_ ## name, (type& name));\
    ECS_COMP_GETTER_(type, name)


#else
  #define ECS_QUERY()
  #define ECS_SYSTEM()
  #define ECS_EVENT_SYSTEM()
  #define ECS_DESCRIBE_QUERY(query_name, params) 
  #define ECS_COMP_GETTER_(type, name) 
#endif
