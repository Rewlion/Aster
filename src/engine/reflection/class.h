#pragma once

struct ClassField;

class Class
{
  public:
    virtual
    auto getFieldsBegin() const -> const ClassField* = 0;

    virtual
    auto getFieldsCount() const -> size_t = 0;

    void setFieldValue(const ClassField*, void* field_owner, const void* value) const;
    void getFieldValue(const ClassField*, const void* field_owner, void* value) const;
};

template<class T>
auto get_static_class() -> const Class*
{
  static_assert(!"unknown class");
}
