#include "mfGameObject.h"



mfGameObject::mfGameObject()
{
}


mfGameObject::~mfGameObject()
{
}

void mfGameObject::Init(mfLoadModelID _RawData, mfBufferDesc _ModelDesc)
{
  // Initialize Vertex Buffer & Index Buffer
  m_Mesh.Init(_RawData);
  // Initialize Model Const Buffer
  m_ModelBuffer.Init(_ModelDesc);
  // Allocate Index Size of Mesh
  m_IndexSize = _RawData.IndexSize;
}

void mfGameObject::Update(mf_PRIMITIVE_TOPOLOGY _Topology, float _Time)
{
  //m_Matrix = XMMatrixRotationY(45);
  m_Model.mWorld = XMMATRIX
  (
    2.0f, 0.0f,  0.0f,  0.0f,
    0.0f,  2.0f, 0.0f,  0.0f,
    0.0f,  0.0f,  2.0f, 0.0f,
    0.0f,  0.0f,  0.0f,  1.0f
    );
  m_Model.vMeshColor = XMFLOAT4(1, 1, 1, 1.0f);
  // Update Model Data
  m_ModelBuffer.Update(&m_Model);
}

void mfGameObject::Render()
{
  // Set Mesh
  m_Mesh.Render();
  // Set Topology
  mfGraphic_API::getSingleton().IASetPrimitiveTopology(mf_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  // Set Model Buffer
  m_ModelBuffer.Render(2, 1, true);
  mfGraphic_API::getSingleton().DrawIndexed(m_IndexSize, 0, 0);
}

void mfGameObject::Destroy()
{
  m_Mesh.Destroy();
  m_ModelBuffer.Destroy();
}
