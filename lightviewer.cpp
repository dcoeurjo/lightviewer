#include "lightviewer.h"

using namespace std;
using namespace boost;

const char source[] = BOOST_COMPUTE_STRINGIZE_SOURCE(
                                                     __kernel void updatePosition(__global  float4* position,
                                                                                  float tick)
                                                     {
                                                       uint gid = get_global_id(0);
                                                      
                                                       position[gid].x = position[gid].x * cos(tick);
                                                     }
                                                     );

void Viewer::updateVertices()
{
  // enqueue kernels to update pointels and make sure that the command queue is finished
  compute::opengl_enqueue_acquire_buffer(m_vertices_cl, m_queue);
  m_updatePos_kernel = m_program.create_kernel("updatePosition");
  m_updatePos_kernel.set_arg(0, m_vertices_cl);
  m_updatePos_kernel.set_arg(1, m_tick);
  m_queue.enqueue_1d_range_kernel(m_updatePos_kernel, 0, m_nbVertices, 0).wait();
  m_queue.finish();
  compute::opengl_enqueue_release_buffer(m_vertices_cl, m_queue);
}


// Draws a spiral
void Viewer::draw()
{
  
  m_vertices_buffer.bind();
  glVertexPointer(3, GL_FLOAT,0,0);
  
 // updateVertices();
//  m_tick++;
  
  // Draw the triangle !
  //glDrawArrays(GL_TRIANGLES,0, m_nbVertices*3);
  // Indexes
  m_indices_buffer.bind();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
  
 // m_indices_buffer.release();
  //m_vertices_buffer.release();
}

void Viewer::init() {
  // Restore previous viewer state.
  restoreStateFromFile();
  
  // create context, command queue and program
  m_context = boost::compute::opengl_create_shared_context();
  m_queue = boost::compute::command_queue(m_context, m_context.get_device());
  // create and build the kernels
  m_program = compute::program::create_with_source(source, m_context);
  try
  {
    m_program.build();
  }
  catch(boost::compute::opencl_error &e)
  {
    // program failed to compile, print out the build log
    std::cout << m_program.build_log() << std::endl;
  }
  m_updatePos_kernel = m_program.create_kernel("updatePosition");
  m_updatePos_kernel.set_arg(0, m_vertices_cl);
  m_updatePos_kernel.set_arg(1, m_tick);

  m_tick=0;
  
  // {X Y Z X Y...}
  m_vertices = {-1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    2.0,2.0,0};
  
  m_indices= {0,1,2,3,4,5};
  
  m_nbVertices = m_vertices.size() / 4;
  
  //BUFFER position
  m_vertices_buffer.create();
  m_indices_buffer.create();

  m_vertices_buffer.bind();
  m_vertices_buffer.setUsagePattern(QGLBuffer::StaticDraw);
  m_vertices_buffer.allocate(m_vertices.data(), sizeof(GLfloat)*m_nbVertices*3);
  //m_vertices_buffer.release();

  //BUFFER position
  m_indices_buffer.bind();
  m_indices_buffer.setUsagePattern(QGLBuffer::StaticDraw);
  m_indices_buffer.allocate(m_indices.data(), sizeof(GLushort)*6);
  //m_indices_buffer.release();

  
 // m_vertices_cl = compute::opengl_buffer(m_context, m_vertices_buffer.bufferId());


 // glLineWidth(2);
  glClearColor(0xe8/256.,0xe8/256.,0xe8/256.,1);
 
  
}

QString Viewer::helpString() const {
  QString text("<h2>S i m p l e V i e w e r</h2>");
  return text;
}
