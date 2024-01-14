#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//called at a ``resize event''
void framebuffer_size_callback(GLFWwindow * wndw, int width, int height){
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow * wndw){
  if(glfwGetKey(wndw, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    printf("Closing window.\n");
    glfwSetWindowShouldClose(wndw, true);
  }
}

int main(){
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow * wndw = glfwCreateWindow(800, 600, "hello_world", NULL, NULL);
  if(wndw == NULL){
    fprintf(stderr, "Failed to create window.\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(wndw);
  if(!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress)){
    fprintf(stderr, "Failed to initialize GLAD.\n");
    exit(EXIT_FAILURE);
  }
  //lower left corner is (0, 0)
  glViewport(0, 0, 800, 600);
  //registering callback for window resize
  glfwSetFramebufferSizeCallback(wndw, framebuffer_size_callback);
  //render loop
  while(!glfwWindowShouldClose(wndw)){
    /* check inputs */
    //check for esc
    process_input(wndw);

    /* render */
    //setter
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //doer
    glClear(GL_COLOR_BUFFER_BIT);

    /* handle events and swap buffers */
    //swap front buffer (displayed) with back buffer (rendered-to)
    glfwSwapBuffers(wndw);
    //check for events
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
