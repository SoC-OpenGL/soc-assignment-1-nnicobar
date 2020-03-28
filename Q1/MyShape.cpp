#include <iostream>
#include<math.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 700, HEIGHT = 700;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShader1Source = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const GLchar* fragmentShader2Source = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.4f, 0.5f, 0.6f, 1.0f );\n"
"}\n\0";

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit( );
    
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    
    // Build and compile our shader programs
    
    // Vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );
    
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shader Disc
    GLuint fragmentShaderDisc = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShaderDisc, 1, &fragmentShader1Source, NULL );
    glCompileShader( fragmentShaderDisc );
    
    // Check for compile time errors
    glGetShaderiv( fragmentShaderDisc, GL_COMPILE_STATUS, &success );
    
    if ( !success )
    {
        glGetShaderInfoLog( fragmentShaderDisc, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shader BG
    GLuint fragmentShaderBG = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShaderBG, 1, &fragmentShader2Source, NULL );
    glCompileShader( fragmentShaderBG );
    
    // Check for compile time errors
    glGetShaderiv( fragmentShaderBG, GL_COMPILE_STATUS, &success );
    
    if ( !success )
    {
        glGetShaderInfoLog( fragmentShaderBG, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Link shader Disc
    GLuint shaderProgramDisc = glCreateProgram( );
    glAttachShader( shaderProgramDisc, vertexShader );
    glAttachShader( shaderProgramDisc, fragmentShaderDisc );
    glLinkProgram( shaderProgramDisc );
    
    // Check for linking errors
    glGetProgramiv( shaderProgramDisc, GL_LINK_STATUS, &success );
    
    if ( !success )
    {
        glGetProgramInfoLog( shaderProgramDisc, 512, NULL, infoLog );
        std::cout << "ERROR::SHADERDISC::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(fragmentShaderDisc);
    
    // Link shader BG
    GLuint shaderProgramBG = glCreateProgram( );
    glAttachShader( shaderProgramBG, vertexShader );
    glAttachShader( shaderProgramBG, fragmentShaderBG );
    glLinkProgram( shaderProgramBG );
    
    // Check for linking errors
    glGetProgramiv( shaderProgramBG, GL_LINK_STATUS, &success );
    
    if ( !success )
    {
        glGetProgramInfoLog( shaderProgramBG, 512, NULL, infoLog );
        std::cout << "ERROR::SHADERBG::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShaderBG );
    
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    
    //VERTEX DATA FOR OUTER DISC
    GLfloat vertices[1089], theta, radius = 0.5f;
    
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    
    for(int i = 3; i<1089; i+=3)
    {
        GLfloat theta = (((i/3)-1)*3.14)/180;
        vertices[i] = radius* cos(theta);
        vertices[i+1] = radius* sin(theta);
        vertices[i+2] = 0.0f;
    }
    
    //VERTEX DATA FOR INNER DISC
    GLfloat invertices[1089], inradius = 0.4f;
    
    invertices[0] = 0.0f;
    invertices[1] = 0.0f;
    invertices[2] = 0.0f;
    
    for(int i = 3; i<1089; i+=3)
    {
        theta = (((i/3)-1)*3.14)/180;
        invertices[i] = inradius* cos(theta);
        invertices[i+1] = inradius* sin(theta);
        invertices[i+2] = 0.0f;
    }
    
    //INDICES
    GLuint indices[363];
    
    for(int i = 0 ; i<=362; i++)
    {
        indices[i] = i;
    }
 
    //DECLARING ARRAYS
    GLuint VBO[2], VAO[2], EBO[2];
    
    //FOR OUTER DISC
    glGenVertexArrays( 1, &VAO[0]);
    glGenBuffers( 1, &VBO[0]);
    glGenBuffers(1, &EBO[0]);
    glBindVertexArray( VAO[0] );
    glBindBuffer( GL_ARRAY_BUFFER, VBO[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT , GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    glBindVertexArray( 0 );
    
    //FOR INNER DISC
    glGenVertexArrays( 1, &VAO[1]);
    glGenBuffers( 1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);
    glBindVertexArray( VAO[1] );
    glBindBuffer( GL_ARRAY_BUFFER, VBO[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( invertices ), invertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT , GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    glBindVertexArray( 0 );
    
    
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.4f, 0.5f, 0.6f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Draw our outer disc
        glUseProgram( shaderProgramDisc );
        glBindVertexArray( VAO[0] );
        glDrawElements(GL_TRIANGLE_FAN, 1080, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //Draw our inner disc
        glUseProgram(shaderProgramBG);
        glBindVertexArray( VAO[1] );
        glDrawElements(GL_TRIANGLE_FAN, 1080, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO[0] );
    glDeleteVertexArrays( 1, &VAO[1] );
    glDeleteBuffers( 1, &VBO[0] );
    glDeleteBuffers( 1, &VBO[1] );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}
