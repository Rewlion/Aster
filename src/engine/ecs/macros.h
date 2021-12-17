#pragma once

#ifdef __clang__
  #define ECS_QUERY() [[clang::annotate("ecs_query")]]
  #define ECS_SYSTEM() [[clang::annotate("ecs_system")]]
  #define ECS_EVENT_SYSTEM() [[clang::annotate("ecs_event_system")]]
#else
  #define ECS_QUERY()
  #define ECS_SYSTEM()
  #define ECS_EVENT_SYSTEM()
#endif
