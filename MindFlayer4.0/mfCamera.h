/**
 * @LC	   : 24/10/2019
 * @file   : mfCamera.h
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : 24/10/2019
 * @brief  : Class in charge of set the camera.
 * @bug	   : No known bugs.
 */

/**
 * Headers
 */
#include "mfDefines.h"
#include "mfConstBuffer.h"
#pragma once

/**
 * @brief : 
 */
struct mfCameraDesc
{
  mfBufferDesc CameraBufferDesc;
  Vector4 Front;
  Vector4 Right;
  Vector4 view;

  Vector4 Eye;
  Vector4 Up;
  Vector4 LookAt;

  float Width;
  float Height;
  float Near;
  float Far;
  float FOV;
};
/**
 * @brief : Class in charge of set the camera.
 */
class mfCamera
{
public:
  mfCamera();
  ~mfCamera();
  /**
   * Members 
   */
private:
  float m_Magnitude = 0.01f;
  float m_lookRotationMagnitude = 0.01f;
#ifdef mfDIRECTX
  XMVECTOR m_Eye;
  XMVECTOR m_Up;
  XMVECTOR m_Front;
  XMVECTOR m_Right;
  XMVECTOR m_LookAt;
  XMMATRIX m_view;
  XMMATRIX m_proj;
#endif // mfDIRECTX
  mfCameraDesc m_descriptor;
  /**
   * @brief : 
   */
  mfConstBuffer m_CameraBuffer;
  /**
   * @brief :
   */
  CB_CameraBuffer m_CameraVP;
  /**
   * Methods 
   */
  public:
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Init(mfCameraDesc _Desc);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Render();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Destroy();

#ifdef mfDIRECTX
  XMMATRIX getViewMatrix();
  XMMATRIX getProjMatrix();
#endif // mfDIRECTX
  void UpdateViewMatrix();
  void UpdateProjMatrix();

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
  void moveForward();
  void moveBack();

  void rotate(POINT* vec);
  void move(Vector3 * mat);
  
  mfCameraDesc & getDescriptor();
};

