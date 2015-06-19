//
//  objLoader.h
//  TrabCG
//
//  Created by Hariel Giacomuzzi on 6/11/15.
//  Copyright (c) 2015 Hariel Giacomuzzi. All rights reserved.
//

#ifndef __TrabCG__objLoader__
#define __TrabCG__objLoader__
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include "glm/glm/glm.hpp"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>

class objLoader {
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec3 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    
private:
    FILE* openFile(char *fileName);
    bool readFile(FILE *f,std::vector< glm::vec3 > *out_vectors, std::vector< glm::vec3 > *out_uvs, std::vector< glm::vec3 > *out_normals);
public:
    bool loadModel(char fileName[],std::vector< glm::vec3 > *out_vectors, std::vector< glm::vec3 > *out_uvs, std::vector< glm::vec3 > *out_normals);
};

#endif /* defined(__TrabCG__objLoader__) */
