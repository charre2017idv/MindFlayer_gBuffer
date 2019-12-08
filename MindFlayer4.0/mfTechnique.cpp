#include "mfTechnique.h"
#include "mfDepthStencilView.h"
#include "mfTransform.h"

mfTechnique::mfTechnique()
{
}


mfTechnique::~mfTechnique()
{
}

void mfTechnique::Init(mfBasePassDesc _PassDesc, int _Index)
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Init(_PassDesc);
  }
}

void mfTechnique::Update(mfDepthStencilView & _DepthStencilView, mfTransform & _Transform, const void * _Resource, float _Time)
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Update(_DepthStencilView, _Transform, _Resource, _Time);
  }
}

void mfTechnique::Render(mfDepthStencilView & _DepthStencilView)
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Render(_DepthStencilView);
  }
}

void mfTechnique::Destroy()
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    m_Passes[i].Destroy();
  }
}

void mfTechnique::CreatePasses()
{
  mfPass tmpPass;
  m_Passes.push_back(tmpPass);
}

mfPass & mfTechnique::GetPass(mfPassTypeID _PassType)
{
  for (int i = 0; i < m_Passes.size(); i++)
  {
    if (m_Passes[i].getDescriptor().PassID == _PassType)
    {
      return m_Passes[i];
    }
  }
}
