#pragma once
#include <exception>

namespace RAPI
{
namespace Exceptions
{
class OException : std::exception
{
public:
	explicit OException(const char* Msg)
	    : Message(Msg) {}
	explicit OException(const OString& Msg)
	    : Message(std::move(Msg)){};

	virtual ~OException() {}

	virtual const char* What() const noexcept
	{
		return Message.c_str();
	}

private:
	OString Message;
};

} // namespace Exceptions

} // namespace RAPI
