#include "mfEffect.h"
#include "mfCamera.h"
#include "mfDepthStencilView.h"
#include "mfPass.h"
#include "mfTransform.h"
mfEffect::mfEffect()
{
}


mfEffect::~mfEffect()
{
}

void mfEffect::Init(mfBasePassDesc _PassDesc, int _Index)
{
  m_Techniques[_Index].Init(_PassDesc, _Index);
}

void mfEffect::Update(mfDepthStencilView & _DepthStencilView, mfCamera & _CameraRef, mfTransform & _Transform, const void * _Resource, float _Time)
{
  // Set Camera Reference
  m_Camera = _CameraRef;

  for (int i = 0; i < m_Techniques.size(); i++)
  {
    m_Techniques[i].Update(_DepthStencilView,_Transform, _Resource, _Time);
  }
}

void mfEffect::Render(mfDepthStencilView & _DepthStencilView)
{
  m_Camera.Render();
  for (int i = 0; i < m_Techniques.size(); i++)
  {
    m_Techniques[i].Render(_DepthStencilView);
  }
}

void mfEffect::Destroy()
{
  for (int i = 0; i < m_Techniques.size(); i++)
  {
    m_Techniques[i].Destroy();
  }
}

void mfEffect::CreateTechniques(int _NumTechniques)
{
  for (int i = 0; i < _NumTechniques; i++)
  {
    mfTechnique tmpTechnique;
    tmpTechnique.CreatePasses();
    m_Techniques.push_back(tmpTechnique);
  }
}

void mfEffect::getTechnique(mfPassTypeID _PassType)
{
  for (int j = 0; j < m_Techniques[0].GetPass(_PassType).getInterface().RenderTargetsVec.size(); j++)
  {
    m_gBufferRT.push_back(m_Techniques[0].GetPass(_PassType).getInterface().RenderTargetsVec[j]);
  
  }
  for (int i = 0; i < m_gBufferRT.size(); i++)
  {
    // Create Shader Resources from Render Targets
    mfGraphic_API::getSingleton().CreateShaderResourceView
    (
      m_gBufferRT[i]
    );

  }
  m_posRT.push_back(m_gBufferRT[0]);
  m_norRT.push_back(m_gBufferRT[1]);
  m_albRT.push_back(m_gBufferRT[2]);
  m_speRT.push_back(m_gBufferRT[3]);
}


void mfEffect::getRenderTargets(mfPassTypeID _PassType)
{
  for (int j = 0; j < m_Techniques[1].GetPass(_PassType).getInterface().RenderTargetsVec.size(); j++)
  {
    m_LightRT.push_back(m_Techniques[1].GetPass(_PassType).getInterface().RenderTargetsVec[j]);

  }
  for (int i = 0; i < m_LightRT.size(); i++)
  {
    // Create Shader Resources from Render Targets
    mfGraphic_API::getSingleton().CreateShaderResourceView
    (
      m_LightRT[i]
    );

  }
}