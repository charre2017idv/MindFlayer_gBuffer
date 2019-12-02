#include "mfTechnique.h"
#include "mfDepthStencilView.h"


mfTechnique::mfTechnique()
{
}


mfTechnique::~mfTechnique()
{
}

void mfTechnique::Init(mfBasePassDesc _PassDesc)
{
  mfPass tmpPass;
  m_Passes.push_back(tmpPass);
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Init(_PassDesc);
  }
}

void mfTechnique::Update(mfDepthStencilView & _DepthStencilView, const void * _Resource, float _Time)
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Update(_DepthStencilView, _Resource, _Time);
  }
}

void mfTechnique::Render()
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Render();
  }
}

void mfTechnique::Destroy()
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Destroy();
  }
}