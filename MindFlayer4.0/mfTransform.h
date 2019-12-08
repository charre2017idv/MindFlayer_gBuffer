/**
 * @LC	   : 05/11/2019
 * @file   : mfTransform
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : 05/11/2019
 * @brief  : Class in charge of setting the Model Matrix.
 * @bug	   : No known bugs.
 */

/**
 * Headers 
 */
#include "mfDefines.h"
#pragma once
/**
 * @brief : 
 */
class mfTransform 
{
public:
  mfTransform();
  ~mfTransform();
  /**
   * Members 
   */
private:
  XMMATRIX m_Matrix;
  //glm::mat4 m_ModelMatrix;
  /**
   * Methods 
   */
public:
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
//   void
//   setModelMatrix
//   (
//     glm::vec3 & _Translate,
//     glm::vec3 & _Rotation,
//     glm::vec3 & _Scale, 
//     float _Time,
//     float _Velocity
//   );
//   /**
//     * @brief  : 
//     * @param  : 
//     * @bug    : 
//     */
//   glm::mat4 & getModelMatrix();

  void setModelMatrix(XMFLOAT3 & _Translate, XMFLOAT3 & _Rotation, XMFLOAT3 & _Scale, float _Angle);
  XMMATRIX & getMatrix();
};

