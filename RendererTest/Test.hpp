#pragma once

// clang-format off
#include "GL.hpp"
#include "Math.hpp"
#include "Shader.hpp"
#include "SmartPtr.hpp"
#include "Types.hpp"
#include "Vector.hpp"
#include "Texture/Texture.hpp"
#include "Vertex/VertexArray.hpp"
#include "Vertex/VertexData/DrawContext.hpp"
#include <Path.hpp>
#include <filesystem>
#include <memory>
#include <stack>
// clang-format on

namespace RAPI
{
class ORenderer;
}
namespace RAPI
{

/**
 * @brief Base class for all tests.
 * @details Each test is an abstract modul, receiving as input base parameters(camera location, frame rate, aspect ration, perspective matrix)
 *
 */
class OTest
{
public:
	OTest(const OPath& shaderPath, RAPI::ORenderer* RendererArg);
	OTest() = default;
	virtual ~OTest() = default;

	void Init(const OMat4& pMatRef)
	{
		pMat = pMatRef;
	}

	virtual void OnUpdate(
	    const float& DeltaTime,
	    const float& Aspect,
	    const OVec3& CameraPos,
	    OMat4& PMat,
	    OMat4& VMat);

	virtual void OnTestEnd() {}

	virtual void InitShader(const OString& shaderPath);

	void EnableVertexArray(const RAPI::OVertexAttribBuffer& VContext);
	void Draw(const SDrawVertexHandle& Handle);
	void BindBuffer(const SBufferHandle& Handle);

	SDrawVertexHandle CreateVertexElement(const SVertexContext& VContext, const SDrawContext& RContext);
	SBufferAttribVertexHandle AddAttribBuffer(const RAPI::OVertexAttribBuffer& AttribBuffer);

	SBufferHandle AddBuffer(const void* Data, size_t Size);
	SBufferHandle AddBuffer(SBufferContext&& Context);

	void EnableAttribArrayBuffer(const SBufferAttribVertexHandle& Handle);
	void EnableAttribArrayBuffer(const SDrawVertexHandle& Handle);

protected:
	OShader& GetShader()
	{
		return Shader;
	}

	std::stack<OMat4>& GetMVStack()
	{
		return mvStack;
	}

	std::stack<OMat4> mvStack;
	OVector<GLuint> vertexArray;

	class ORenderer* Renderer;

private:
	OMat4
	    pMat,
	    mMat,
	    mvMat,
	    tMat,
	    rMat;

	/**
	 * @brief Shader that is used for pipeline.
	 *
	 */
	OShader Shader;
	OVector<SVertexHandle> Handles;
};

} // namespace RAPI