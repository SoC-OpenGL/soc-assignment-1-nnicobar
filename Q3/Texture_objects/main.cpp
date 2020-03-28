
#include <iostream>
#include<math.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"

// Window dimensions
const GLuint WIDTH = 700, HEIGHT = 700;

// The MAIN function, from here we start the application and run the game loop
int main( )
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
    
    // enable alpha support
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    // Build and compile our shader program
    Shader ourShader1( "resources/shaders/core.vs", "resources/shaders/core.frag" );
    Shader ourShader2("resources/shaders/core2.vs", "resources/shaders/core2.frag" );
    

    
 
    //VERTEX DATA FOR OUTER DISC
    GLfloat vertices[2904], theta, radius = 0.5f;
    
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    vertices[3] = 1.0f;
    vertices[4] = 1.0f;
    vertices[5] = 1.0f;
    vertices[6] = 0.5f;
    vertices[7] = 0.5f;
    
    for(int i = 8; i<2904; i+=8)
    {
        theta = (((i/3)-1)*3.14)/180;
        vertices[i] = radius* cos(theta);
        vertices[i+1] = radius* sin(theta);
        vertices[i+2] = 0.0f;
        vertices[i+3] = 1.0f;
        vertices[i+4] = 1.0f;
        vertices[i+5] = 1.0f;
        vertices[i+6] = 0.5f + 4*cos(theta);
        vertices[i+7] = 0.5f + 4*sin(theta);
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


   

    GLuint VBO[2], VAO[2], EBO[2];
    
    // Outer disc
    glGenVertexArrays( 1, &VAO[0] );
    glGenBuffers( 1, &VBO[0] );
    glGenBuffers( 1, &EBO[0] );
    
    glBindVertexArray( VAO[0] );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO[0] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
    
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray(1);
    // Texture Coordinate attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    
    glBindVertexArray( 0 ); // Unbind VAO
    
    //Inner disc
    glGenVertexArrays( 1, &VAO[1] );
    glGenBuffers( 1, &VBO[1] );
    glGenBuffers( 1, &EBO[1] );
    
    glBindVertexArray( VAO[1] );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( invertices ), invertices, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);
    glBindVertexArray( 0 ); // Unbind VAO
    
    // Load and create a texture
    GLuint texture;
    
    int width, height;
    
    
    // ===================
    // Texture
    // ===================
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    // Set our texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // Set texture filtering
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    unsigned char *image = SOIL_load_image( "resources/images/image1.png", &width, &height, 0, SOIL_LOAD_RGBA );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.4f, 0.6f, 0.7f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
       
        // Draw
        ourShader1.Use( );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture );
        glUniform1i( glGetUniformLocation( ourShader1.Program, "ourTexture" ), 0 );
        
        // Draw container
        glBindVertexArray( VAO[0] );
        glDrawElements( GL_TRIANGLE_FAN, sizeof(vertices), GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );
        
        //Inner disc
        ourShader2.Use();
        glBindVertexArray(VAO[1]);
        glDrawElements( GL_TRIANGLE_FAN, sizeof(invertices), GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );
        
        
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO[0] );
    glDeleteBuffers( 1, &VBO[0] );
    glDeleteBuffers( 1, &EBO[0] );
    glDeleteVertexArrays( 1, &VAO[1] );
    glDeleteBuffers( 1, &VBO[1] );
    glDeleteBuffers( 1, &EBO[1] );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

