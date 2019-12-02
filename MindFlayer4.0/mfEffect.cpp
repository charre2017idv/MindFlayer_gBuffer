#include "mfEffect.h"
#include "mfDepthStencilView.h"
#include "mfPass.h"

mfEffect::mfEffect()
{
}


mfEffect::~mfEffect()
{
}

void mfEffect::Init(mfBasePassDesc _PassDesc)
{
  mfTechnique tmpTechnique;
  m_Techniques.push_back(tmpTechnique);

  for (int i = 0; i < m_Techniques.size(); i++)
  {
    m_Techniques[i].Init(_PassDesc);
  }
}

void mfEffect::Update(mfDepthStencilView & _DepthStencilView, const void * _Resource, float _Time)
{
  for (int i = 0; i < m_Techniques.size(); i++)
  {
    m_Techniques[i].Update(_DepthStencilView, _Resource, _Time);
  }
}

void mfEffect::Render()
{
  for (int i = 0; i < m_Techniques.size(); i++)
  {
    m_Techniques[i].Render();
  }
}

void mfEffect::Destroy()
{
  for (int i = 0; i < m_Techniques.size(); i++)
  {
    m_Techniques[i].Destroy();
  }
}
