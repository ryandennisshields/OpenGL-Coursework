#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace glm;
using namespace std;

struct OBJIndex
{
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
    
    bool operator<(const OBJIndex& r) const { return vertexIndex < r.vertexIndex; }
};

class IndexedModel
{
public:
    vector<vec3> positions;
    vector<vec2> texCoords;
    vector<vec3> normals;
    vector<unsigned int> indices;
    
    void CalcNormals();
};

class OBJModel
{
public:
    vector<OBJIndex> OBJIndices;
    vector<vec3> vertices;
    vector<vec2> uvs;
    vector<vec3> normals;
    bool hasUVs;
    bool hasNormals;
    
    OBJModel(const string& fileName);
    
    IndexedModel ToIndexedModel();
private:
    unsigned int FindLastVertexIndex(const vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
    void CreateOBJFace(const string& line);
    
    vec2 ParseOBJVec2(const string& line);
    vec3 ParseOBJVec3(const string& line);
    OBJIndex ParseOBJIndex(const string& token, bool* hasUVs, bool* hasNormals);
};

#endif // OBJ_LOADER_H_INCLUDED
