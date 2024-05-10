#ifdef __clang__
#define CLASS(...)    class [[clang::annotate("refl_class")]]
#define PROPERTY(...) [[clang::annotate("refl_property")]]

#define REFLECTION_BODY(type)               \
  friend class type ## Class;               \
  public:                                   \
    auto getClass() -> const Class*;        \
    static                                  \
    auto getStaticClass() -> const Class*;  \
  private:
#else
  static_assert(!"unsupported")
#endif
