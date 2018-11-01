#include "terrainscene.h"
#include <iostream>

/**
 * @brief TerrainScene::TerrainScene
 * Construct the terrain scene by allocating some memory for our vertex data and triangle indices
 */
TerrainScene::TerrainScene(int xsize, int ysize)
    : Scene(), m_nVerts(xsize*ysize), m_nTris((xsize-1)*(ysize-1)*2), m_xsize(xsize), m_ysize(ysize) {
    // Note that each vertex is made up of 9 attributes:
    // (x,y,z),(nx,ny,nz),(r,g,b)
    m_data = (GLfloat*) malloc(sizeof(GLfloat) * // Size of the data type in bytes
                               m_nVerts * // The number of vertices
                               9); // Consists of vertex position, normal and colour information

    // Create the triangle indices for our scene
    m_tris = (GLuint*) malloc(sizeof(GLuint) * // The size of the data type in bytes
                             (xsize-1) * (ysize-1) * // There is one less row / col in the triangle strip
                             3 * // There are 3 indices for each triangle
                             2); // There are 2 triangles in each 4 vertex patch
}

/**
 * @brief TerrainScene::~TerrainScene
 */
TerrainScene::~TerrainScene() {
    free(m_data);
    free(m_tris);
}

/**
 * @brief TerrainScene::init Initialise the data and opengl stuff needed to display the scene.
 * This is where the normals are created.
 */
void TerrainScene::init() {
    Scene::init();

    // Enable material properties for our mesh
    GLfloat matAmb[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat matDiff[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matSpec[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat matShine = 1000.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShine);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    unsigned int i, j;

    // Build the triangle data
    GLuint *iptr = m_tris;
    for (i = 1; i < m_xsize; ++i) {
        for (j = 1; j < m_ysize; ++j) {
            // Triangles in the patch are defined according to this indexing scheme:
            //   (i-1)+(j-1)*xsz-------i+(j-1)*xsz
            //        |    \             |
            //        |      \           |
            //        |        \         |
            //        |          \       |
            //        |            \     |
            //        |              \   |
            //    (i-1)+j*xsz----------i+j*xsz

            // The first triangle in this patch
            iptr[0] = (i-1)+(j-1)*m_xsize; iptr[1] = i+j*m_xsize; iptr[2] = i+(j-1)*m_xsize;

            // The second triangle in this patch
            iptr[3] = iptr[0]; iptr[4] = (i-1)+j*m_xsize; iptr[5] = iptr[1];

            // Increment our pointer position
            iptr += 6;
        }
    }

    // Build the vertex data
    GLfloat xpart = 2.0f / GLfloat(m_xsize-1);
    GLfloat ypart = 2.0f / GLfloat(m_ysize-1);
    GLfloat *ptr = m_data;
    GLfloat x,y,z,r,g,b;

    for (i = 0; i < m_xsize; ++i) {
        for (j = 0; j < m_ysize; ++j) {
            // Calculate the position of the vertex
            x = i*xpart-1.0; y=j*ypart-1.0;
            z = GLfloat(m_noise.GetValue(x, y, 0.0));
            ptr[0] = x;
            ptr[1] = y;
            ptr[2] = 0.2f * z;

            // Initialise our normals to zero
            ptr[3] = 0.0f;
            ptr[4] = 0.0f;
            ptr[5] = 0.0f;

            // Compute the colour of each vertex
            b = (z+1.0f) * 0.5f;
            g = 1.0f - b;
            r = 0.0f;
            ptr[6] = r;
            ptr[7] = g;
            ptr[8] = b;
            ptr += 9; // Jump to the next load of data
        }
    }

    // Now compute the face normals and store them somewhere temporary
    GLfloat fnormal[3];
    GLuint ai,bi,ci; // The indices of the vertices
    GLfloat *A, *B, *C; // A pointer to the data of each vertex
    GLfloat AB[3], AC[3]; // vectors holding the edges AB and AC

    for (i=0; i<m_nTris; ++i) {
        // Store the indices to each vertex of this triangle
        ai = m_tris[i*3+0];
        bi = m_tris[i*3+1];
        ci = m_tris[i*3+2];

        // Create pointers to our current points
        A = &(m_data[ai*9]);
        B = &(m_data[bi*9]);
        C = &(m_data[ci*9]);

        // Construct the vectors for the triangle
        for (j=0; j<3; ++j) {
            AB[j] = B[j] - A[j];
            AC[j] = C[j] - A[j];
        }

        // Now compute the cross product
        fnormal[0] = AB[1]*AC[2] - AB[2]*AC[1];
        fnormal[1] = AB[2]*AC[0] - AB[0]*AC[2];
        fnormal[2] = AB[0]*AC[1] - AB[1]*AC[0];

        // Add the face normal to the appropriate vertex normal
        for (j=0; j<3; ++j) {
            m_data[ai*9+3+j] += fnormal[j];
            m_data[bi*9+3+j] += fnormal[j];
            m_data[ci*9+3+j] += fnormal[j];
        }
    }

    // Now compute the vertex normals by adding together all the face normals around the vertex
    GLfloat mag, inv_mag;
    for (i = 0; i < m_nVerts; ++i) {
        mag = m_data[i*9+3]*m_data[i*9+3] + m_data[i*9+4]*m_data[i*9+4] + m_data[i*9+5]*m_data[i*9+5];
        if (mag == 0.0f) {
            std::cerr << "Normal has zero magnitude! Throwing toys.\n";
            exit(1);
        } else {
        }
        inv_mag = 1.0f / sqrt(mag);
        for (j=0; j<3; ++j) {
            m_data[i*9+3+j] *= inv_mag;
        }
//        std::cout << "v"<<i<<"=["
//                  << m_data[i*9+0]<<","
//                  << m_data[i*9+1]<<","
//                  << m_data[i*9+2]<<","
//                  << m_data[i*9+3]<<","
//                  << m_data[i*9+4]<<","
//                  << m_data[i*9+5]<<","
//                  << m_data[i*9+6]<<","
//                  << m_data[i*9+7]<<","
//                  << m_data[i*9+8]<<"]\n";
    }
}

/**
 * @brief TerrainScene::resize - this didn't really need to be overloaded - I was just experimenting with other cameras.
 * @param w Width
 * @param h Height
 */
void TerrainScene::resize(int w, int h) {
    if (!m_isInit) return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -5.0, 5.0);
    gluPerspective(60.0, float(w) / float(h), 0.1, 10.0);

    glViewport(0,0,w,h);

    glMatrixMode(GL_MODELVIEW);
}

/**
 * @brief TerrainScene::draw Display the data in triangles and/or points
 */
void TerrainScene::draw() {
    // Draw the parent class draw routine to clear the image
    Scene::draw();

    // Position the object in front of the camera and rotate it
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2.0f);
    glRotatef(m_rotation, 0.0f, 1.0f, 0.0f);

    unsigned int i,j, triIdx;

    // Draw the data as points using pointer arithmetic!
//    GLfloat *ptr = m_data; // For scary pointer arithmetic
//    glPointSize(1.0f); // Scale the points if needed
//    glBegin(GL_POINTS);
//    for (i=0; i<m_nVerts; ++i) {
//        glNormal3fv(ptr+3);
//        glColor3fv(ptr+6);
//        glVertex3fv(ptr);
//        ptr += 9;
//    }
//    glEnd(); //GL_POINTS

    // Draw triangles based on the triangle index
    glBegin(GL_TRIANGLES);
    for (i=0; i<m_nTris; ++i) {
        for (j=0; j<3; ++j) {
            triIdx = m_tris[i*3+j];
            glNormal3fv(&(m_data[triIdx*9]) + 3);
            glColor3fv(&(m_data[triIdx*9]) + 6);
            glVertex3fv(&(m_data[triIdx*9]));
        }
    }
    glEnd(); // GL_TRIANGLES

    glPopMatrix();
}

/**
 * @brief TerrainScene::update rotate the scene based on the elapsed time.
 */
void TerrainScene::update() {
    Scene::update();

    // Use the elapsed time to do some sort of animation
    m_rotation = GLfloat(m_elapsedTime) * 50.0f;
}
