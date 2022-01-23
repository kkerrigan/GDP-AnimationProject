#include "cSkinnedMeshComponent.h"

#include "sModelDrawInfo.h"
#include "cVAOMeshManager.h"
#include "cShaderManager.h"
#include "cMesh.h"

// c'tor
cSkinnedMeshComponent::cSkinnedMeshComponent() : cComponent() {

	this->pAniState = nullptr;
	this->mType = "Skin Mesh";

	return;
}

void cSkinnedMeshComponent::sVertexBoneData::addBoneData(unsigned int BoneID, float Weight) {

	for (unsigned int Index = 0; Index < sizeof(this->ids) / sizeof(this->ids[0]); Index++) {

		if (this->weights[Index] == 0.0f) {

			this->ids[Index] = (float)BoneID;
			this->weights[Index] = Weight;

			return;
		}
	}
}

void cSkinnedMeshComponent::setBasePath(std::string basepath) {

	this->mBasePath = basepath;
	return;
}

bool cSkinnedMeshComponent::loadMeshFromFile() {

	unsigned int Flags = aiProcess_Triangulate |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace;

	std::string fileName = this->meshName;
	if (this->mBasePath != "") {
		fileName = this->mBasePath + "/" + this->meshName;
	}

	this->mScene = this->mImporter.ReadFile(fileName.c_str(), Flags);

	if (this->mScene) {

		this->friendlyName = this->meshName;

		this->mGlobalInverseTransformation = AIMatrixToGLMMatrix(mScene->mRootNode->mTransformation);
		this->mGlobalInverseTransformation = glm::inverse(this->mGlobalInverseTransformation);

		// Calcualte all the bone things
		if (!this->mInitialize()) return false;
	}

	return true;
}

bool cSkinnedMeshComponent::loadMeshAnimation(const std::string& friendlyName, const std::string& filename) {

	// Already loaded this ?
	std::map<std::string, sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameToCharacter.find(friendlyName);

	// Found it? 
	if (itAnimation != this->mapAnimationFriendlyNameToCharacter.end()) return false; // Already loaded

	unsigned int Flags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices;

	Assimp::Importer* pImporter = new Assimp::Importer();

	std::string fullFileName = filename;
	if (this->mBasePath != "") {
		fullFileName = this->mBasePath + "/" + filename;
	}

	sAnimationInfo animInfo;
	animInfo.friendlyName = friendlyName;
	animInfo.fileName = fullFileName;
	animInfo.pAIScene = pImporter->ReadFile(animInfo.fileName.c_str(), Flags);

	if (!animInfo.pAIScene) return false;

	// Animation scene is loaded (we assume)
	// Add it to the map
	this->mapAnimationFriendlyNameToCharacter[animInfo.friendlyName] = animInfo;

	return true;
}

cMesh* cSkinnedMeshComponent::createMeshObjectFromCurrentModel(unsigned int meshIndex) {

	if (this->mScene->mNumMeshes < meshIndex) {	// Doesn't have this mesh

		return NULL;
	}

	// Assume there is a valid mesh there
	cMesh* pTheMesh = new cMesh();

	aiMesh* pAIMesh = this->mScene->mMeshes[meshIndex];

	pTheMesh->numberOfVertices = pAIMesh->mNumVertices;

	pTheMesh->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[pTheMesh->numberOfVertices];

	for (int vertIndex = 0; vertIndex != pTheMesh->numberOfVertices; vertIndex++) {

		sVertex_xyz_rgba_n_uv2_bt_4Bones* pCurVert = &(pTheMesh->pVertices[vertIndex]);

		aiVector3D* pAIVert = &(pAIMesh->mVertices[vertIndex]);

		pCurVert->x = pAIVert->x;
		pCurVert->y = pAIVert->y;
		pCurVert->z = pAIVert->z;

		// Colours
		if (pAIMesh->GetNumColorChannels() > 0) {

			pCurVert->r = this->mScene->mMeshes[0]->mColors[vertIndex]->r;
			pCurVert->g = this->mScene->mMeshes[0]->mColors[vertIndex]->g;
			pCurVert->b = this->mScene->mMeshes[0]->mColors[vertIndex]->b;
			pCurVert->a = this->mScene->mMeshes[0]->mColors[vertIndex]->a;
		}
		else {
			pCurVert->r = pCurVert->g = pCurVert->b = pCurVert->a = 1.0f;
		}

		// Normals
		if (pAIMesh->HasNormals()) {

			pCurVert->nx = pAIMesh->mNormals[vertIndex].x;
			pCurVert->ny = pAIMesh->mNormals[vertIndex].y;
			pCurVert->nx = pAIMesh->mNormals[vertIndex].z;
		}

		// UVs
		if (pAIMesh->GetNumUVChannels() > 0) {	// Assume 1st channel is the 2D UV coordinates

			pCurVert->u0 = pAIMesh->mTextureCoords[0][vertIndex].x;
			pCurVert->v0 = pAIMesh->mTextureCoords[0][vertIndex].y;

			if (pAIMesh->GetNumUVChannels() > 1) {

				pCurVert->u1 = pAIMesh->mTextureCoords[1][vertIndex].x;
				pCurVert->v1 = pAIMesh->mTextureCoords[1][vertIndex].y;
			}
		}

		// Tangents and Bitangents (bi-normals)
		if (pAIMesh->HasTangentsAndBitangents()) {

			pCurVert->tx = pAIMesh->mTangents[vertIndex].x;
			pCurVert->ty = pAIMesh->mTangents[vertIndex].y;
			pCurVert->tz = pAIMesh->mTangents[vertIndex].z;

			pCurVert->bx = pAIMesh->mBitangents[vertIndex].x;
			pCurVert->by = pAIMesh->mBitangents[vertIndex].y;
			pCurVert->bz = pAIMesh->mBitangents[vertIndex].z;
		}

		// Bone IDs are being passed OK
		pCurVert->boneID[0] = this->vecVertexBoneData[vertIndex].ids[0];
		pCurVert->boneID[1] = this->vecVertexBoneData[vertIndex].ids[1];
		pCurVert->boneID[2] = this->vecVertexBoneData[vertIndex].ids[2];
		pCurVert->boneID[3] = this->vecVertexBoneData[vertIndex].ids[3];

		// Weights are being passed OK
		pCurVert->boneWeights[0] = this->vecVertexBoneData[vertIndex].weights[0];
		pCurVert->boneWeights[1] = this->vecVertexBoneData[vertIndex].weights[1];
		pCurVert->boneWeights[2] = this->vecVertexBoneData[vertIndex].weights[2];
		pCurVert->boneWeights[3] = this->vecVertexBoneData[vertIndex].weights[3];


	}//for ( int vertIndex

	// Triangles
	pTheMesh->numberOfTriangles = pAIMesh->mNumFaces;
	pTheMesh->pTriangles = new cTriangle[pTheMesh->numberOfTriangles];

	for (unsigned int triIndex = 0; triIndex != pTheMesh->numberOfTriangles; triIndex++) {

		aiFace* pAIFace = &(pAIMesh->mFaces[triIndex]);

		pTheMesh->pTriangles[triIndex].vertex_ID_0 = pAIFace->mIndices[0];
		pTheMesh->pTriangles[triIndex].vertex_ID_1 = pAIFace->mIndices[1];
		pTheMesh->pTriangles[triIndex].vertex_ID_2 = pAIFace->mIndices[2];

	}//for ( unsigned int triIndex...

	pTheMesh->name = this->friendlyName;
	pTheMesh->calculateExtents();

	return pTheMesh;
}

glm::mat4 cSkinnedMeshComponent::AIMatrixToGLMMatrix(const aiMatrix4x4& mat) {

	return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1,
		mat.a2, mat.b2, mat.c2, mat.d2,
		mat.a3, mat.b3, mat.c3, mat.d3,
		mat.a4, mat.b4, mat.c4, mat.d4);
}

void cSkinnedMeshComponent::boneTransform(float TimeInSeconds, std::string animationName,
	std::vector<glm::mat4>& FinalTransformation, std::vector<glm::mat4>& Globals, std::vector<glm::mat4>& Offsets) {

	glm::mat4 Identity(1.0f);

	float TicksPerSecond = this->findAnimationTicks(animationName);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, this->findAnimationTotalTime(animationName));

	// use the "animation" file to look up these nodes
	// (need the matOffset information from the animation file)
	this->readNodeHeirarchy(AnimationTime, animationName, this->mScene->mRootNode, Identity);

	FinalTransformation.resize(this->mNumBones);
	Globals.resize(this->mNumBones);
	Offsets.resize(this->mNumBones);

	for (unsigned int BoneIndex = 0; BoneIndex < this->mNumBones; BoneIndex++) {

		FinalTransformation[BoneIndex] = this->mBoneInfo[BoneIndex].FinalTransformation;
		Globals[BoneIndex] = this->mBoneInfo[BoneIndex].ObjectBoneTransformation;
		Offsets[BoneIndex] = this->mBoneInfo[BoneIndex].BoneOffset;
	}

	return;
}

void cSkinnedMeshComponent::readNodeHeirarchy(float AnimationTime, std::string animationName, const aiNode* pNode, const glm::mat4& parentTransformMatrix) {

	aiString NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = this->mScene->mAnimations[0];

	// Search for the animation we want... 
	std::map< std::string, sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameToCharacter.find(animationName);

	// Did we find it? 
	if (itAnimation != this->mapAnimationFriendlyNameToCharacter.end()) {

		// Yes, there is an animation called that...
		// ...replace the animation with the one we found
		pAnimation = reinterpret_cast<const aiAnimation*>(itAnimation->second.pAIScene->mAnimations[0]);
	}

	// Transformation of the node in bind pose
	glm::mat4 NodeTransformation = AIMatrixToGLMMatrix(pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = this->findNodeAnimationChannel(pAnimation, NodeName);

	if (pNodeAnim) {

		// Get interpolated scaling
		glm::vec3 scale;
		this->calcGLMInterpolatedScaling(AnimationTime, pNodeAnim, scale);
		glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), scale);

		// Get interpolated rotation (quaternion)
		glm::quat ori;
		this->calcGLMInterpolatedRotation(AnimationTime, pNodeAnim, ori);
		glm::mat4 RotationM = glm::mat4_cast(ori);

		// Get interpolated position 
		glm::vec3 pos;
		this->calcGLMInterpolatedPosition(AnimationTime, pNodeAnim, pos);
		glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), pos);


		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 ObjectBoneTransformation = parentTransformMatrix * NodeTransformation;

	std::map<std::string, unsigned int>::iterator it = this->mMapBoneNameToBoneIndex.find(std::string(NodeName.data));
	if (it != this->mMapBoneNameToBoneIndex.end()) {

		unsigned int BoneIndex = it->second;
		this->mBoneInfo[BoneIndex].ObjectBoneTransformation = ObjectBoneTransformation;
		this->mBoneInfo[BoneIndex].FinalTransformation = this->mGlobalInverseTransformation
			* ObjectBoneTransformation
			* this->mBoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int ChildIndex = 0; ChildIndex != pNode->mNumChildren; ChildIndex++) {

		this->readNodeHeirarchy(AnimationTime, animationName,
			pNode->mChildren[ChildIndex], ObjectBoneTransformation);
	}

	return;
}

const aiNodeAnim* cSkinnedMeshComponent::findNodeAnimationChannel(const aiAnimation* pAnimation, aiString nodeOrBoneName) {

	for (unsigned int ChannelIndex = 0; ChannelIndex != pAnimation->mNumChannels; ChannelIndex++) {

		if (pAnimation->mChannels[ChannelIndex]->mNodeName == nodeOrBoneName) {
			return pAnimation->mChannels[ChannelIndex];
		}
	}
	return 0;
}

float cSkinnedMeshComponent::findAnimationTotalTime(std::string animationName) {

	std::map<std::string, sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameToCharacter.find(animationName);

	// Found it? 
	if (itAnimation == this->mapAnimationFriendlyNameToCharacter.end()) {	// Nope.

		return 0.0f;
	}

	// This is scaling the animation from 0 to 1
	return (float)itAnimation->second.pAIScene->mAnimations[0]->mDuration;
}

float cSkinnedMeshComponent::findAnimationTicks(std::string animationName) {

	std::map<std::basic_string<char>, sAnimationInfo>::iterator itAnimation = this->mapAnimationFriendlyNameToCharacter.find(animationName);

	// Found it? 
	if (itAnimation == this->mapAnimationFriendlyNameToCharacter.end()) {	// Nope.

		return 0.0f;
	}

	// This is scaling the animation from 0 to 1
	return (float)itAnimation->second.pAIScene->mAnimations[0]->mTicksPerSecond;
}

float cSkinnedMeshComponent::findAnimationDuration(std::string animationName) {

	std::map< std::string, sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameToCharacter.find(animationName);

	// Found it? 
	if (itAnimation == this->mapAnimationFriendlyNameToCharacter.end()) {	// Nope.

		return 0.0f;
	}

	// This is scaling the animation from 0 to 1
	return (float)itAnimation->second.pAIScene->mAnimations[0]->mDuration / (float)itAnimation->second.pAIScene->mAnimations[0]->mTicksPerSecond;
}

unsigned int cSkinnedMeshComponent::findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim) {

	for (unsigned int RotationKeyIndex = 0; RotationKeyIndex != pNodeAnim->mNumRotationKeys - 1; RotationKeyIndex++) {

		if (AnimationTime < (float)pNodeAnim->mRotationKeys[RotationKeyIndex + 1].mTime) {
			return RotationKeyIndex;
		}
	}

	return 0;
}

unsigned int cSkinnedMeshComponent::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim) {

	for (unsigned int PositionKeyIndex = 0; PositionKeyIndex != pNodeAnim->mNumPositionKeys - 1; PositionKeyIndex++) {

		if (AnimationTime < (float)pNodeAnim->mPositionKeys[PositionKeyIndex + 1].mTime) {
			return PositionKeyIndex;
		}
	}

	return 0;
}

unsigned int cSkinnedMeshComponent::findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim) {

	for (unsigned int ScalingKeyIndex = 0; ScalingKeyIndex != pNodeAnim->mNumScalingKeys - 1; ScalingKeyIndex++) {

		if (AnimationTime < (float)pNodeAnim->mScalingKeys[ScalingKeyIndex + 1].mTime) {
			return ScalingKeyIndex;
		}
	}

	return 0;
}

void cSkinnedMeshComponent::calcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out) {

	if (pNodeAnim->mNumRotationKeys == 1) {
		out.w = pNodeAnim->mRotationKeys[0].mValue.w;
		out.x = pNodeAnim->mRotationKeys[0].mValue.x;
		out.y = pNodeAnim->mRotationKeys[0].mValue.y;
		out.z = pNodeAnim->mRotationKeys[0].mValue.z;
		return;
	}

	unsigned int RotationIndex = this->findRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	if (Factor < 0.0f) Factor = 0.0f;
	if (Factor > 1.0f) Factor = 1.0f;

	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

	glm::quat StartGLM = glm::quat(StartRotationQ.w, StartRotationQ.x, StartRotationQ.y, StartRotationQ.z);
	glm::quat EndGLM = glm::quat(EndRotationQ.w, EndRotationQ.x, EndRotationQ.y, EndRotationQ.z);

	out = glm::slerp(StartGLM, EndGLM, Factor);

	out = glm::normalize(out);

	return;
}

void cSkinnedMeshComponent::calcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out) {

	if (pNodeAnim->mNumPositionKeys == 1) {
		out.x = pNodeAnim->mPositionKeys[0].mValue.x;
		out.y = pNodeAnim->mPositionKeys[0].mValue.y;
		out.z = pNodeAnim->mPositionKeys[0].mValue.z;
		return;
	}

	unsigned int PositionIndex = this->findPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	if (Factor < 0.0f) Factor = 0.0f;
	if (Factor > 1.0f) Factor = 1.0f;

	const aiVector3D& StartPosition = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& EndPosition = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;

	glm::vec3 start = glm::vec3(StartPosition.x, StartPosition.y, StartPosition.z);
	glm::vec3 end = glm::vec3(EndPosition.x, EndPosition.y, EndPosition.z);

	out = (end - start) * Factor + start;

	return;
}

void cSkinnedMeshComponent::calcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out) {

	if (pNodeAnim->mNumScalingKeys == 1) {
		out.x = pNodeAnim->mScalingKeys[0].mValue.x;
		out.y = pNodeAnim->mScalingKeys[0].mValue.y;
		out.z = pNodeAnim->mScalingKeys[0].mValue.z;
		return;
	}

	unsigned int ScalingIndex = this->findScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);

	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	if (Factor < 0.0f) Factor = 0.0f;
	if (Factor > 1.0f) Factor = 1.0f;

	const aiVector3D& StartScale = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& EndScale = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;

	glm::vec3 start = glm::vec3(StartScale.x, StartScale.y, StartScale.z);
	glm::vec3 end = glm::vec3(EndScale.x, EndScale.y, EndScale.z);
	out = (end - start) * Factor + start;

	return;
}

bool cSkinnedMeshComponent::mInitialize(void) {

	this->mNumberOfVertices = this->mScene->mMeshes[0]->mNumVertices;

	// This is the vertex information for JUST the bone stuff
	this->vecVertexBoneData.resize(this->mNumberOfVertices);

	this->mLoadBones(this->mScene->mMeshes[0], this->vecVertexBoneData);

	return true;
}

void cSkinnedMeshComponent::mLoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData>& vertexBoneData) {

	for (unsigned int boneIndex = 0; boneIndex != Mesh->mNumBones; boneIndex++) {

		unsigned int BoneIndex = 0;
		std::string BoneName(Mesh->mBones[boneIndex]->mName.data);

		std::map<std::string, unsigned int>::iterator it = this->mMapBoneNameToBoneIndex.find(BoneName);
		if (it == this->mMapBoneNameToBoneIndex.end()) {

			BoneIndex = this->mNumBones;
			this->mNumBones++;

			sBoneInfo bi;
			this->mBoneInfo.push_back(bi);

			this->mBoneInfo[BoneIndex].BoneOffset = AIMatrixToGLMMatrix(Mesh->mBones[boneIndex]->mOffsetMatrix);
			this->mMapBoneNameToBoneIndex[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = it->second;
		}

		for (unsigned int WeightIndex = 0; WeightIndex != Mesh->mBones[boneIndex]->mNumWeights; WeightIndex++) {

			unsigned int VertexID = /*mMeshEntries[MeshIndex].BaseVertex +*/ Mesh->mBones[boneIndex]->mWeights[WeightIndex].mVertexId;
			float Weight = Mesh->mBones[boneIndex]->mWeights[WeightIndex].mWeight;
			vertexBoneData[VertexID].addBoneData(BoneIndex, Weight);
		}
	}
	return;

}
