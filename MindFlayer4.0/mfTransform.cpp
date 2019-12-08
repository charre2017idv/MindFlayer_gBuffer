#include "mfTransform.h"



mfTransform::mfTransform()
{
}


mfTransform::~mfTransform()
{
}

void mfTransform::setModelMatrix(XMFLOAT3 & _Translate, XMFLOAT3 & _Rotation, XMFLOAT3 & _Scale, float _Angle)
{
  m_Matrix = XMMATRIX
  (
    _Scale.x, 0.0f, 0.0f, _Translate.x,
    0.0f, _Scale.y, 0.0f, _Translate.y,
    0.0f, 0.0f, _Scale.z, _Translate.z,
    0.0f, 0.0f, 0.0f, 1.0f
  );

   m_Matrix *= XMMatrixRotationX(_Rotation.x);
   m_Matrix *= XMMatrixRotationY(_Rotation.y);
   m_Matrix *= XMMatrixRotationZ(_Rotation.z);
}

XMMATRIX & mfTransform::getMatrix()
{
  return m_Matrix;
}

// void mfTransform::setModelMatrix(glm::vec3 & _Translate, glm::vec3 & _Rotation, glm::vec3 & _Scale, float _Time, float _Velocity)
// {
//   m_ModelMatrix = glm::translate(glm::mat4(1.0f), _Translate);
//   m_ModelMatrix = glm::scale(m_ModelMatrix, _Scale);
//   m_ModelMatrix = glm::rotate(m_ModelMatrix,  _Velocity, _Rotation);
//   m_ModelMatrix = glm::transpose(m_ModelMatrix);
// }
// 
// glm::mat4 & mfTransform::getModelMatrix()
// {
//   return m_ModelMatrix;
// }
