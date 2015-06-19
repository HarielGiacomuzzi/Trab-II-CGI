//
//  objLoader.cpp
//  TrabCG
//
//  Created by Hariel Giacomuzzi on 6/11/15.
//  Copyright (c) 2015 Hariel Giacomuzzi. All rights reserved.
//

#include "objLoader.h"

FILE* objLoader::openFile(char *fileName){
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        fputs("Can't open File", stdout);
    }
    
    return f;
}

bool objLoader::loadModel(char *fileName, std::vector<glm::vec3> *out_vectors, std::vector<glm::vec3> *out_uvs, std::vector<glm::vec3> *out_normals){
    FILE *f = openFile(fileName);
    return readFile(f, out_vectors, out_uvs, out_normals);
}

bool objLoader::readFile(FILE *f, std::vector< glm::vec3 > *out_vectors, std::vector< glm::vec3 > *out_uvs, std::vector< glm::vec3 > *out_normals){
    if (f == NULL) {
        fputs("Can't open File", stdout);
        return false;
    }else{
        char line[500];
        while (! feof(f)) {
            fscanf(f, "%s", line);
            if (strcmp(line, "v") == 0) {
                glm::vec3 vertex;
                fscanf(f, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                temp_vertices.push_back(vertex);
            }else if ( strcmp( line, "vt" ) == 0 ){
                glm::vec3 uv;
                fscanf(f, "%f %f %f\n", &uv.x, &uv.y, &uv.z );
                temp_uvs.push_back(uv);
            }else if ( strcmp( line, "vn" ) == 0 ){
                glm::vec3 normal;
                fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                temp_normals.push_back(normal);
            }else if ( strcmp( line, "f" ) == 0 ){
//                std::string *vertex1;
//                std::string *vertex2;
//                std::string *vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by me : ( Try exporting with other options\n");
                    return false;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
            
        }
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
            glm::vec3 uv = temp_uvs[ vertexIndex-1 ];
            glm::vec3 normal = temp_normals[ vertexIndex-1 ];
            out_vectors->push_back(vertex);
            out_uvs->push_back(uv);
            out_normals->push_back(normal);
        }
        
        return true;
    }
}