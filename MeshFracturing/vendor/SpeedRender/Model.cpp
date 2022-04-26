#include "Model.h"

Model::Model(std::string path) {
    LoadModel(path);
    velocity = glm::vec3(0.0f);
}

Model::Model(Mesh mesh) {
    meshes.push_back(mesh);
    velocity = glm::vec3(0.0f);
}

void Model::LoadModel(std::string path) {
    cy::TriMesh t;
    t.LoadFromFileObj(path.c_str());

    // loop over vertices
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int index = 0;
    for (int i = 0; i < t.NF(); i++) {
        cy::TriMesh::TriFace f = t.F(i);
        cy::TriMesh::TriFace fn;
        if (t.HasNormals()) fn = t.FN(i);
        cy::TriMesh::TriFace ft;
        if (t.HasTextureVertices()) ft = t.FT(i);

        for (int j = 0; j < 3; j++) {
            cy::Vec3f pos = t.V(f.v[j]); // position
            cy::Vec3f norm(0.0f);
            cy::Vec3f uv(0.0f);

            if (t.HasNormals()) norm = t.VN(fn.v[j]);       // normal
            if (t.HasTextureVertices()) uv = t.VT(ft.v[j]); // uv

            vertices.push_back(
                {
                    glm::vec3(pos.x, pos.y, pos.z),
                    glm::vec3(norm.x, norm.y, norm.z),
                    glm::vec2(uv.x, uv.y)
                }
            );

            indices.push_back(index++);
        }
    }

	std::vector<Texture> textures;
	Mesh mesh(vertices, indices, textures, t.HasNormals(), t.HasTextureVertices());
    meshes.push_back(mesh);

}

void Model::Draw(Shader &shader) {
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}  