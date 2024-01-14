#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


//something like this
//
//            ^
//            |
//            x  0.5
//            |
//            |
//      -0.5  |   0.5
//    --------|--------->
//            |
//       x    |    x
//            |
//            |

//x y z
//in ``normalized device coordinates''
float vertices[] = {
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
  0.0f, 0.5f, 0.0f
};

//super basic vertex shader -- written in GLSL
//allows us to process the vertices, which are then sent to primitive assembly (making shapes)
//
//this needs to be compiled
const char * vertexShaderSrc = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main(){\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

//super basic -- ``make it all orange'' -- fragment shader
//`FragColor` is an RGBA color-vector
const char * fragShaderSrc = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main(){\n"
  "   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
  "}\n";

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
  //NOTE: glfwGetProcAddress is named like a function -- but clearly isn't
  if(!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress)){
    fprintf(stderr, "Failed to initialize GLAD.\n");
    exit(EXIT_FAILURE);
  }
  //now we can do OpenGL stuff
  //storing the vertices
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  /* VERTEX ARRAY OBJECT */
  //used to store the vertex attributes so they don't have to be reset
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //a vertex buffer is a ``GL_ARRAY_BUFFER''
  //telling OpenGL `VBO` is a `GL_ARRAY_BUFFER`
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //fill the buffer
  //we use `GL_STATIC_DRAW` because it doesn't change
  //if we move the triangle -- `GL_DYNAMIC_DRAW` would be more appropriate
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* VERTEX ATTRIBUTES */
  //QUESTION: Is this just the first vertex then? We have three.
  //Or is it the `aPos` vertex inside the vertexShader?
  //We want store in the VAO -- since then we don't have to reset
  //the vertex attributes all the time. See the above!
  //                    loc.     dim          normalize?  stride           start ind.
  glVertexAttribPointer(0,       3, GL_FLOAT, GL_FALSE,   3*sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  //now we have the vertices on the GPU -- let's make them appear
  //    --> vertex shader and fragment shader

  /* VERTEX SHADER */
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
  if(!success){
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    fprintf(stderr, "Vertex shader information log: %s\n", infoLog);
    fprintf(stderr, "Vertex shader did not compile.\n");
  }

  /* FRAGMENT SHADER */
  //the final part --> corresponds to pixels that are put on the screen
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
  if(!success){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    fprintf(stderr, "Fragment shader information log: %s\n", infoLog);
    fprintf(stderr, "Fragment shader did not compile.\n");
  }

  /* LINKING SHADERS */
  //now we link the shaders together, to get a ``shader program''
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  //QUESTION: what if we use the wrong order here?
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  //check for errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "Shader program information log: %s\n", infoLog);
    fprintf(stderr, "Shader program did not link.\n");
  }

  //they're linked, we can kill them
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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
    //if this is not here -- will be a bunch of flickering in the BG
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* handle events and swap buffers */
    //swap front buffer (displayed) with back buffer (rendered-to)
    glfwSwapBuffers(wndw);
    //check for events
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
