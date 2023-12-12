#pragma once
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "maths/vector.hpp"
#include "GJK.hpp"

vec3 EPA(const std::vector<sf::Vector3f>& p1, vec3 a, vec3 b, vec3 c, vec3 d, const std::function<vec3(const std::vector<sf::Vector3f>&, vec3)>& support){
    std::vector<std::array<vec3, 4>> faces(EPA_MAX_NUM_FACES); //Array of faces, each with 3 verts and a normal
    
    // Init with final simplex from GJK
    faces[0][0] = a;
    faces[0][1] = b;
    faces[0][2] = c;
    faces[0][3] = normalise(crossProduct(b-a, c-a)); //ABC
    faces[1][0] = a;
    faces[1][1] = c;
    faces[1][2] = d;
    faces[1][3] = normalise(crossProduct(c-a, d-a)); //ACD
    faces[2][0] = a;
    faces[2][1] = d;
    faces[2][2] = b;
    faces[2][3] = normalise(crossProduct(d-a, b-a)); //ADB
    faces[3][0] = b;
    faces[3][1] = d;
    faces[3][2] = c;
    faces[3][3] = normalise(crossProduct(d-b, c-b)); //BDC

    int num_faces=4;
    int closest_face;

    for(int iterations=0; iterations<EPA_MAX_NUM_ITERATIONS; iterations++){
        //Find face that's closest to origin
        float min_dist = dotProduct(faces[0][0], faces[0][3]);
        closest_face = 0;
        for(int i=1; i<num_faces; i++){
            float dist = dotProduct(faces[i][0], faces[i][3]);
            if(dist<min_dist){
                min_dist = dist;
                closest_face = i;
            }
        }

        //search normal to face that's closest to origin
        vec3 search_dir = faces[closest_face][3]; 
        vec3 p = support(p1, search_dir) - support(p1, -search_dir);

        if (dotProduct(p, search_dir)-min_dist < 0.0001){ // // 0.0001 is the tolerance, higher is less accurate but faster
            //Convergence (new point is not significantly further from origin)
            return faces[closest_face][3]*dotProduct(p, search_dir); //dot vertex with normal to resolve collision along normal!
        }

        std::vector<std::array<vec3, 2>> loose_edges(EPA_MAX_NUM_LOOSE_EDGES); //keep track of edges we need to fix after removing faces
        int num_loose_edges = 0;

        //Find all triangles that are facing p
        for(int i=0; i<num_faces; i++)
        {
            if(dotProduct(faces[i][3], p-faces[i][0] )>0) //triangle i faces p, remove it
            {
                //Add removed triangle's edges to loose edge list.
                //If it's already there, remove it (both triangles it belonged to are gone)
                for(int j=0; j<3; j++) //Three edges per face
                {
                    std::array<vec3, 2> current_edge = {faces[i][j], faces[i][(j+1)%3]};
                    bool found_edge = false;
                    for(int k=0; k<num_loose_edges; k++) //Check if current edge is already in list
                    {
                        if(loose_edges[k][1]==current_edge[0] && loose_edges[k][0]==current_edge[1]){
                            //Edge is already in the list, remove it
                            //THIS ASSUMES EDGE CAN ONLY BE SHARED BY 2 TRIANGLES (which should be true)
                            //THIS ALSO ASSUMES SHARED EDGE WILL BE REVERSED IN THE TRIANGLES (which 
                            //should be true provided every triangle is wound CCW)
                            loose_edges[k][0] = loose_edges[num_loose_edges-1][0]; //Overwrite current edge
                            loose_edges[k][1] = loose_edges[num_loose_edges-1][1]; //with last edge in list
                            num_loose_edges--;
                            found_edge = true;
                            k=num_loose_edges; //exit loop because edge can only be shared once
                        }
                    }//endfor loose_edges

                    if(!found_edge){ //add current edge to list
                        // assert(num_loose_edges<EPA_MAX_NUM_LOOSE_EDGES);
                        if(num_loose_edges>=EPA_MAX_NUM_LOOSE_EDGES) break;
                        loose_edges[num_loose_edges][0] = current_edge[0];
                        loose_edges[num_loose_edges][1] = current_edge[1];
                        num_loose_edges++;
                    }
                }

                //Remove triangle i from list
                faces[i][0] = faces[num_faces-1][0];
                faces[i][1] = faces[num_faces-1][1];
                faces[i][2] = faces[num_faces-1][2];
                faces[i][3] = faces[num_faces-1][3];
                num_faces--;
                i--;
            }//endif p can see triangle i
        }//endfor num_faces
        
        //Reconstruct polytope with p added
        for(int i=0; i<num_loose_edges; i++)
        {
            // assert(num_faces<EPA_MAX_NUM_FACES);
            if(num_faces>=EPA_MAX_NUM_FACES) break;
            faces[num_faces][0] = loose_edges[i][0];
            faces[num_faces][1] = loose_edges[i][1];
            faces[num_faces][2] = p;
            faces[num_faces][3] = normalise(crossProduct(loose_edges[i][0]-loose_edges[i][1], loose_edges[i][0]-p));

            //Check for wrong normal to maintain CCW winding
            float bias = 0.000001; //in case dot result is only slightly < 0 (because origin is on face)
            if(dotProduct(faces[num_faces][0], faces[num_faces][3])+bias < 0){
                vec3 temp = faces[num_faces][0];
                faces[num_faces][0] = faces[num_faces][1];
                faces[num_faces][1] = temp;
                faces[num_faces][3] = -faces[num_faces][3];
            }
            num_faces++;
        }
    } //End for iterations
    printf("EPA did not converge\n");
    //Return most recent closest point
    return faces[closest_face][3] * dotProduct(faces[closest_face][0], faces[closest_face][3]);
}