#pragma once
#include "Buffer.hpp"
#include "VertexData/VertexContext.hpp"

#include <SmartPtr.hpp>
#include <Types.hpp>

namespace RAPI
{
class OVertexAttribBuffer
{
public:
	OVertexAttribBuffer(/* args */) = default;
	OVertexAttribBuffer(const OVertexAttribBuffer& Context) = default;

	OVertexAttribBuffer(OVertexAttribBuffer&& Context) noexcept
	    : VertexContext(Move(Context.VertexContext))
	{
	}

	OVertexAttribBuffer& operator=(const OVertexAttribBuffer& Other) = default;
	OVertexAttribBuffer& operator=(OVertexAttribBuffer&& Other) noexcept
	{
		VertexContext = Move(Other.VertexContext);
		return *this;
	}

	OVertexAttribBuffer(const SVertexContext& Context);
	OVertexAttribBuffer(SVertexContext&& Context) noexcept;

	void EnableVertexAttribPointer();
	void BindBuffer();

private:
	void RegisterBuffer();

	SVertexContext VertexContext;
};
} // namespace RAPI
