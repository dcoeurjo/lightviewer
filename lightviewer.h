#pragma once
#include <QGLShaderProgram>
#include <QGLViewer/qglviewer.h>
#include <QGLBuffer>


#include <boost/compute/system.hpp>
#include <boost/compute/algorithm.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/interop/opengl.hpp>
#include <boost/compute/utility/source.hpp>



class Viewer : public QGLViewer
{

public:
  Viewer(): m_vertices_buffer(QGLBuffer::VertexBuffer) {}
  
protected:
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;

  void updateVertices();
  
  

  std::vector<GLfloat> m_vertices;

  QGLShaderProgram shader_program;
  
  boost::compute::context m_context;
  boost::compute::command_queue m_queue;
  boost::compute::program m_program;

  boost::compute::opengl_buffer m_vertices_cl;
  QGLBuffer m_vertices_buffer;

  QGLBuffer m_indices_buffer;
  std::vector<GLushort> m_indices;
  
  boost::compute::kernel m_updatePos_kernel;

  
  uint m_nbVertices;
  
  float m_tick;
  
};
