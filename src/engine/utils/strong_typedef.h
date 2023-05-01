#define STRONG_SCALAR_TYPEDEF(BASE_TYPE, NEW_TYPE)\
class NEW_TYPE\
{\
  public:\
    constexpr explicit NEW_TYPE(const BASE_TYPE v)\
     : m_Value(v)\
    {\
    }\
    \
    NEW_TYPE() = default;\
    NEW_TYPE(const NEW_TYPE& v) = default;\
    NEW_TYPE(NEW_TYPE&& v) = default;\
    NEW_TYPE& operator= (NEW_TYPE&& rvl) = default;\
    NEW_TYPE& operator= (const NEW_TYPE& rvl) = default;\
    NEW_TYPE& operator= (const BASE_TYPE& rvl)\
    {\
      m_Value = rvl;\
      return *this;\
    }\
    \
    constexpr operator const BASE_TYPE&() const\
    {\
      return m_Value;\
    }\
    constexpr operator BASE_TYPE&()\
    {\
      return m_Value;\
    }\
    \
    std::strong_ordering operator <=> (const BASE_TYPE& rvl) const\
    {\
      return m_Value < rvl ? std::strong_ordering::less :\
             m_Value > rvl ? std::strong_ordering::greater :\
                             std::strong_ordering::equal;\
    }\
    std::strong_ordering operator <=> (const NEW_TYPE& rvl) const\
    {\
      return m_Value < rvl.m_Value ? std::strong_ordering::less :\
             m_Value > rvl.m_Value ? std::strong_ordering::greater :\
                                     std::strong_ordering::equal;\
    }\
  private:\
    BASE_TYPE m_Value;\
}
