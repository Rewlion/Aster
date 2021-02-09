#include "transform.h"

#include "glm/gtc/matrix_transform.hpp"
#include <assert.h>

Transform::Transform()
  : Parent(nullptr)
  , LocalPosition(0.0f, 0.0f, 0.0f)
  , LocalScale(1.0f, 1.0f, 1.0f)

{
}

//TODO when parent component will be removed from the entity,
// one have to remove all links between parent,childs
void Transform::AttachTo(Transform* parent)
{
  Parent = parent;
}

glm::vec3 Transform::GetWorldPosition() const
{
  glm::vec3 worldPosition = LocalPosition;
  Transform* parent = Parent;
  while (parent != nullptr)
  {
    worldPosition += parent->LocalPosition;
    parent = parent->Parent;
  }

  return worldPosition;
}

glm::quat Transform::GetWorldRotation() const
{
  glm::quat worldRotation = LocalRotation;
  Transform* parent = Parent;
  while (parent != nullptr)
  {
    worldRotation *= parent->LocalRotation;
    parent = parent->Parent;
  }

  return worldRotation;
}

glm::vec3 Transform::GetWorldScale() const
{
  glm::vec3 worldScale = LocalScale;
  Transform* parent = Parent;
  while (parent != nullptr)
  {
    worldScale *= parent->LocalScale;
    parent = parent->Parent;
  }

  return worldScale;
}

glm::mat4 Transform::GetTransformationMatrix() const
{
  glm::mat4 mat(1);

  const Transform* parent = this;
  while (parent != nullptr)
  {
    glm::mat4 parentMat(1);
    parentMat = glm::translate(parentMat, parent->LocalPosition);
    parentMat = parentMat * glm::mat4_cast(parent->LocalRotation);
    parentMat = glm::scale(parentMat, parent->LocalScale);

    mat = parentMat* mat;

    parent = parent->Parent;
  }

  return mat;
}

glm::mat4 Transform::GetTransformationMatrixWithoutScale() const
{
  glm::mat4 mat(1);

  const Transform* parent = this;
  while (parent != nullptr)
  {
    glm::mat4 parentMat(1);
    parentMat = glm::translate(parentMat, parent->LocalPosition);
    parentMat = parentMat * glm::mat4_cast(parent->LocalRotation);

    mat = parentMat * mat;

    parent = parent->Parent;
  }

  return mat;
}

glm::mat4 Transform::GetCameraTransformationMatrixWithoutScale() const
{
  glm::mat4 mat(1);

  const Transform* parent = this;
  while (parent != nullptr)
  {
    const glm::vec3 inversePosition = -parent->LocalPosition;
    const glm::quat inverseRotation = glm::inverse(parent->LocalRotation);

    const glm::mat4 parentMat = glm::mat4_cast(inverseRotation) * glm::translate(glm::mat4(1), inversePosition);
    mat = parentMat * mat;

    parent = parent->Parent;
  }

  return mat;
}

//Vulkan orientation
glm::vec3 Transform::GetOrientationVector(OrientationVectorType type) const
{
  const glm::mat4 mat = GetTransformationMatrixWithoutScale();
  switch (type)
  {
  case OrientationVectorType::Forward:
    return glm::vec3{ mat * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f } };

  case OrientationVectorType::Right:
    return glm::vec3{ mat * glm::vec4{ 1.0f, 0.0f, 0.0f, 0.0f } };

  case OrientationVectorType::Up:
    return glm::vec3{ mat * glm::vec4{ 0.0f, -1.0f, 0.0f, 0.0f } };

  default:
    assert(!"Unknown orientation type");
    return {}; //shhhh, warnings
  }
}