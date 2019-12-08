#include "mfCamera.h"



mfCamera::mfCamera()
{
}


mfCamera::~mfCamera()
{
}

void mfCamera::Init(mfCameraDesc _Desc)
{
  // Set Descriptor
  m_descriptor = _Desc;
  // Initialize constant Buffer
  m_CameraBuffer.Init(_Desc.CameraBufferDesc);

  UpdateViewMatrix();

  UpdateProjMatrix();
}

void mfCamera::Update()
{
//   Set View buffer
//     m_CameraVP.mView = XMMatrixTranspose(m_view);
//     // Set Projection Buffer
//     m_CameraVP.mProjection = XMMatrixTranspose(m_proj);
//     m_CameraBuffer.Update(&m_CameraVP);
}

void mfCamera::Render()
{
//   m_Front = XMVector4Normalize(m_LookAt - m_Eye);
//   m_Right = XMVector4Normalize(XMVector3Cross(m_Up, m_Front));
//   m_Up = XMVector4Normalize(XMVector3Cross(m_Front, m_Right));
//   m_LookAt = m_Front + m_Eye;

  // Set view Const Buffer
  m_CameraBuffer.Render(0, 1, false);
}


void mfCamera::Destroy()
{
  m_CameraBuffer.Destroy();
}

XMMATRIX  mfCamera::getViewMatrix()
{
  return m_view;
}

XMMATRIX mfCamera::getProjMatrix()
{
  return m_proj;
}

void mfCamera::UpdateViewMatrix()
{
  // Set Vectors
  m_Eye = XMVectorSet
  (
    m_descriptor.Eye.x, 
    m_descriptor.Eye.y, 
    m_descriptor.Eye.z,
    m_descriptor.Eye.w
  );
  m_Up = XMVectorSet
  (
    m_descriptor.Up.x, 
    m_descriptor.Up.y, 
    m_descriptor.Up.z, 
    m_descriptor.Up.w
  );
  m_LookAt = XMVectorSet
  (
    m_descriptor.LookAt.x,
    m_descriptor.LookAt.y,
    m_descriptor.LookAt.z,
    m_descriptor.LookAt.w
  );
  m_Front = XMVectorSet
  (
    m_descriptor.Front.x,
    m_descriptor.Front.y, 
    m_descriptor.Front.z, 
    m_descriptor.Front.w
  );
  m_Right = XMVectorSet
  (
    m_descriptor.Right.x,
    m_descriptor.Right.y, 
    m_descriptor.Right.z, 
    m_descriptor.Right.w
  );

  m_view = XMMatrixLookAtLH(m_Eye, m_LookAt, m_Up);
  // Set View buffer
  m_CameraVP.mView = XMMatrixTranspose(m_view);
  m_CameraBuffer.Update(&m_CameraVP);
}

void mfCamera::UpdateProjMatrix()
{
  m_proj = XMMatrixPerspectiveFovLH
  (
    m_descriptor.FOV,
    m_descriptor.Width / m_descriptor.Height,
    m_descriptor.Near,
    m_descriptor.Far
  );
  m_CameraVP.mProjection = XMMatrixTranspose(m_proj);
  m_CameraBuffer.Update(&m_CameraVP);
}



void mfCamera::moveLeft()
{
  m_Eye -= m_Right * m_Magnitude;
  m_LookAt -= m_Right * m_Magnitude;

}

void mfCamera::moveRight()
{
  m_Eye += m_Right * m_Magnitude;
  m_LookAt += m_Right * m_Magnitude;

}

void mfCamera::moveUp()
{
  m_Eye += m_Up * m_Magnitude;
  m_LookAt += m_Up * m_Magnitude;
}

void mfCamera::moveDown()
{
  m_Eye -= m_Up * m_Magnitude;
  m_LookAt -= m_Up * m_Magnitude;

}

void mfCamera::moveForward()
{
  m_Eye += m_Front * m_Magnitude;
  m_LookAt += m_Front * m_Magnitude;
}

void mfCamera::moveBack()
{
  m_Eye -= m_Front * m_Magnitude;
  m_LookAt -= m_Front * m_Magnitude;

}

void mfCamera::rotate(POINT* vec)
{
  m_Eye += m_Right * m_lookRotationMagnitude * (vec->x);
  Render();
  m_Eye -= m_Up * m_lookRotationMagnitude * (vec->y);
}



void mfCamera::move(Vector3 * mat)
{
  if (mat->x > 0)
    moveRight();
  if (mat->x < 0)
    moveLeft();
  if (mat->y > 0)
    moveUp();
  if (mat->y < 0)
    moveDown();
  if (mat->z > 0)
    moveForward();
  if (mat->z < 0)
    moveBack();

  //Render();
  UpdateViewMatrix();
}

mfCameraDesc & mfCamera::getDescriptor()
{
  return m_descriptor;
}
