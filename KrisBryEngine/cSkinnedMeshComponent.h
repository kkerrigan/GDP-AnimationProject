#ifndef _cSkinnedMeshComponent_HG_
#define _cSkinnedMeshComponent_HG_

#include <gameMath.h>
#include <string>
#include <map>
#include <vector>
#include <array>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "cComponent.h"
#include "cMesh.h"
#include "cAnimationState.h"

class cSkinnedMeshComponent : public cComponent {
public:
	static const int MAX_BONES_PER_VERTEX = 4;

private:
	// Data structs
	struct sVertexBoneData {

		std::array<float, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> weights;

		void addBoneData(unsigned int BoneID, float Weight);
	};

	struct sBoneInfo {

		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
		glm::mat4 ObjectBoneTransformation;
	};

	struct sAnimationInfo {
		std::string friendlyName;
		std::string fileName;
		const aiScene* pAIScene;
	};

public:
	cAnimationState* pAniState;
	std::string meshName;
	std::string friendlyName;
	const aiScene* mScene;

	// Extent Values for skinned mesh
	glm::vec3 minXYZ_from_SM_Bones;
	glm::vec3 maxXYZ_from_SM_Bones;

	std::vector<sVertexBoneData> vecVertexBoneData;

	// Store all the bones for this model, buing updated per frame
	std::vector<glm::mat4x4> vecObjectBoneTransformation;

	// Animations for this model
	std::map<std::string, sAnimationInfo> mapAnimationFriendlyNameToCharacter;

	cSkinnedMeshComponent();
	virtual ~cSkinnedMeshComponent(void){}

	void setBasePath(std::string basepath);

	bool loadMeshFromFile();
	bool loadMeshAnimation(const std::string& friendlyName, const std::string& filename);

	// Returns NULL (0) if there is no mesh at that index
	cMesh* createMeshObjectFromCurrentModel(unsigned int meshIndex = 0);

	glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4& mat);
	void boneTransform(float TimeInSeconds,
		std::string animationName,		
		std::vector<glm::mat4>& FinalTransformation,
		std::vector<glm::mat4>& Globals,
		std::vector<glm::mat4>& Offsets);
	
	void readNodeHeirarchy(float AnimationTime,
		std::string animationName,		
		const aiNode* pNode,
		const glm::mat4 &parentTransformMatrix);

	// Animation Finders
	const aiNodeAnim* findNodeAnimationChannel(const aiAnimation* pAnimation, aiString nodeOrBoneName);
	float findAnimationTotalTime(std::string animationName);
	float findAnimationTicks(std::string animationName);
	float findAnimationDuration(std::string animationName);

	// Node functions
	unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	// Interpolation functions
	void calcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out);
	void calcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);
	void calcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);

private:
	std::string mType;
	std::string mBasePath;

	// Assimp veriables
	Assimp::Importer mImporter;
	glm::mat4 mGlobalInverseTransformation;
	std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> mMapBoneNameToBoneIndex;
	unsigned int mNumBones;
	std::vector<sBoneInfo> mBoneInfo;

	// Model variables
	unsigned int mNumberOfVertices;

	bool mInitialize(void);
	void mLoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &Bones);
};


#endif
