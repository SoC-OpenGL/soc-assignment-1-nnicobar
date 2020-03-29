#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "Shader.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

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
    
    // Build and compile our shader program
    Shader ourShader1( "resources/shaders/vshader", "resources/shaders/fragshaderW" );
    Shader ourShader2( "resources/shaders/vshader", "resources/shaders/fragshaderB" );
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat wvertices[]=
    {
        -0.5f,  -0.5f,  0.0f,
        -0.5f,   0.5f,  0.0f,
         0.5f,  -0.5f,  0.0f,
         0.5f,   0.5f,  0.0f
    };
    
    GLuint windices[]=
    {
        0,1,2,
        1,2,3
    };
    
    
    //BLACK INDICES
    
    GLfloat bvertices[243];
    GLuint bindices[192];
    
 
   int c = 0;
    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
        {
            bvertices[c] = -0.5f+(j*0.125f);
            bvertices[c+1] = -0.5f+(i*0.125f);
            bvertices[c+2] = 0.0f;
            c+=3;
        }
    

    c = 6;
    int ctr = 0;
    int first = 1;
    bindices[0] = 1;
    for(int i = 0 ; i<8; i++)
    {
        if(first)
        { for(int j = 0; j<3; j++)
            {
            bindices[c] = bindices[c-6]+2;
            c+=6;
            }
            ctr = -1; first = 0;
            
        }
       else if(ctr%2==0)
            for(int j = 0; j<4; j++)
            {
                bindices[c] = bindices[c-6]+2;
                c+=6;
            }
        else
        {
            bindices[c] = bindices[c-6]+4;
            c+=6;
            for(int j = 0; j<3; j++)
            {
                bindices[c] = bindices[c-6]+2;
                c+=6;
            }
        }
        ctr++;
    }
    
    c = 0;
    for(int i = 0; i<8; i++)
        for(int j = 0; j<4; j++)
        {
            bindices[c+1] = bindices[c]+1;
            bindices[c+2] = bindices[c]+9;
            bindices[c+3] = bindices[c+1];
            bindices[c+4] = bindices[c+2];
            bindices[c+5] = bindices[c+2]+1;
            c+=6;
        }
  
    /*
    for(int i = 0; i<243; i+=3)
    {
        std::cout<<i/3<<" "<<bvertices[i]<<" "<<bvertices[i+1]<<" "<<bvertices[i+2]<<std::endl;
    }
    
    for(int i = 0; i<192; i+=3)
    {
        std::cout<<bindices[i]<<" "<<bindices[i+1]<<" "<<bindices[i+2]<<std::endl;
    }
    */
    
    //BUFFERS
    GLuint VBO[2], VAO[2], EBO[2];
    
    glGenVertexArrays( 1, &VAO[0] );
    glGenBuffers( 1, &VBO[0] );
    glGenBuffers( 1, &EBO[0] );
    glBindVertexArray( VAO[0] );
    glBindBuffer( GL_ARRAY_BUFFER, VBO[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( wvertices ), wvertices, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO[0] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( windices ), windices, GL_STATIC_DRAW );
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);
    glBindVertexArray( 0 ); // Unbind VAO
    
    glGenVertexArrays( 1, &VAO[1] );
    glGenBuffers( 1, &VBO[1] );
    glGenBuffers( 1, &EBO[1] );
    glBindVertexArray( VAO[1] );
    glBindBuffer( GL_ARRAY_BUFFER, VBO[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( bvertices ), bvertices, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( bindices ), bindices, GL_STATIC_DRAW );
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);
    glBindVertexArray( 0 ); // Unbind VAO
    

    
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.6f, 0.9f, 0.8f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        
        // White Square
        ourShader1.Use();
        glBindVertexArray( VAO[0] );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );
        
        //Black Squares
        ourShader2.Use();
        glBindVertexArray( VAO[1] );
        glDrawElements( GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0 );
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


