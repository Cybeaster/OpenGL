#include "SmartPtr.hpp"

#include <Path.hpp>
#include <Types.hpp>
#include <cstdint>
#include <string>

namespace RenderAPI
{
class ORenderer;
}

struct OShaderName
{
	OShaderName() = default;

	OShaderName(const TString& Str)
	    : Name(Str)
	{
	}

	OShaderName(const char Str[])
	    : Name(Str)
	{
	}

	OShaderName(const OShaderName& Str)
	    : Name(Str.Name)
	{
	}

	OShaderName& operator=(const TString& Str)
	{
		Name = Str;
		return *this;
	}

	operator TString()
	{
		return Name;
	}

	TString Name;
};

class OApplication
{
public:
	static auto GetApplication()
	{
		if (!application)
		{
			application = RenderAPI::OSharedPtr<OApplication>(new OApplication());
			return application;
		}
		else
		{
			return application;
		}
	}

	static auto GetShaderLocalPathWith(const OShaderName& Name)
	{
		return RootDirPath.string() + ShadersDir.string() + Name.Name;
	}

	static auto GetResourceDirectoryWith(const OPath& Path)
	{
		return RootDirPath.string() + ResourceDirectory.string() + Path.string();
	}

	/**
	 * @brief Programm start.
	 * @details Initializes Renderer class.
	 *
	 */
	void Start(int argc, char** argv);

private:
	OApplication() = default;

	static inline RenderAPI::OSharedPtr<OApplication> application = nullptr;

	static inline OPath DebugPath = current_path();
	static inline OPath RootDirPath = current_path().parent_path();

	static inline OPath ShadersDir = "\\Externals\\Shaders\\";
	static inline OPath ResourceDirectory = "\\Externals\\Resources\\";

	static inline OShaderName SimpleCubeShader = "\\SimpleCube.shader";
	static inline OShaderName SimpleTextureShader = "\\SimpeTexture.shader";
};