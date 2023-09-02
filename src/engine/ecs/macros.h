#pragma once

#ifdef __clang__
  #define ECS_QUERY() [[clang::annotate("ecs_query")]]
  #define ECS_SYSTEM() [[clang::annotate("ecs_system")]]
  #define ECS_EVENT_SYSTEM() [[clang::annotate("ecs_event_system")]]

  #define ECS_DESCRIBE_QUERY(query_name, params) \
    ECS_QUERY()\
    static\
    void query_name(eastl::function<void(params)>);
#else
  #define ECS_QUERY()
  #define ECS_SYSTEM()
  #define ECS_EVENT_SYSTEM()
  #define ECS_DESCRIBE_QUERY(query_name, params) 
#endif
