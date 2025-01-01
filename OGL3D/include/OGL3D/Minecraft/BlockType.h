#pragma once

struct BlockType 
{
public:
	int id;
	bool isSolid;

    int frontFaceTexture;
    int backFaceTexture;
    int topFaceTexture;
    int bottomFaceTexture;
    int leftFaceTexture;
    int rightFaceTexture;

public:
    int getTextureID(int faceIndex)
    {
        switch (faceIndex)
        {
            case 0: return frontFaceTexture;     // Front face
            case 1: return backFaceTexture;      // Back face
            case 2: return topFaceTexture;       // Top face
            case 3: return bottomFaceTexture;    // Bottom face
            case 4: return rightFaceTexture;      // Right face
            case 5: return leftFaceTexture;       // Left face
            default: return 0;
        }
    }
};


