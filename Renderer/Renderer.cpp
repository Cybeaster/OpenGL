#include "Renderer.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <iostream>

#define DEBUG_FPS false

namespace RenderAPI
{

int ORenderer::ScreenWidth = 900;
int ORenderer::ScreenHeight = 700;

float ORenderer::Aspect{ 0 };
float ORenderer::DeltaTime{ 0 };
float ORenderer::LastFrame{ 0 };
float ORenderer::CurrentFrame{ 0 };
float ORenderer::Fovy{ 1.0472F };

OMat4 ORenderer::VMat{};
OMat4 ORenderer::PMat{};

OVec2 ORenderer::PressedMousePos{ 0, 0 };
OMat4 ORenderer::MouseCameraRotation{ OMat4(1.F) };

bool ORenderer::RightMousePressed{ false };

/// @brief Mouse Rotation Speed Divide Factor
float ORenderer::MRSDivideFactor{ 100.F };

// std::unique_ptr<Renderer> Renderer::SingletonRenderer = nullptr;

ORenderer::~ORenderer()
{
	glfwDestroyWindow(Window);
	glfwTerminate();
}

void ORenderer::Init()
{
	SetInput();

	// Post Init has to be called after everything
	PostInit();
}

void ORenderer::PostInit()
{
	VertexArray.AddVertexArray();
}

void ORenderer::SetInput()
{
	InputHandler.InitHandlerWith(this);
}

#pragma region GLFW

GLFWwindow*
ORenderer::GLFWInit()
{
	/* Initialize the library */
	assert(glfwInit());

	/* Create a windowed mode window and its OpenGL context */
	Window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Renderer", NULL, NULL);

	if (!Window)
		glfwTerminate();

	/* Make the window's context current */
	glfwMakeContextCurrent(Window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error with glewInit()" << std::endl;

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	SetInput();
	return Window;
}

void ORenderer::MoveCamera(const OVec3& Delta)
{
	Camera.Translate(Delta);
}

void ORenderer::GLFWRendererStart(const float currentTime)
{
	CleanScene();
	CalcScene();
	GLFWCalcPerspective(Window);
	CalcDeltaTime(currentTime);
	PrintDebugInfo();
}

void ORenderer::CalcScene()
{
	PMat = glm::perspective(1.0472f, Aspect, 0.01f, 1000.F);
	VMat = MouseCameraRotation * glm::translate(OMat4(1.0f), GetCameraPosition() * -1.F);
}

void ORenderer::GLFWRendererEnd()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(Window);
	glfwPollEvents();
}

void ORenderer::GLFWRenderTickStart()
{
	while (!glfwWindowShouldClose(Window))
	{
		GLFWRendererStart(glfwGetTime());
		for (auto* const test : Tests)
			if (test)
				test->OnUpdate(DeltaTime, Aspect, GetCameraPosition(), PMat, VMat);
		GLFWRendererEnd();
	}
}
void ORenderer::PrintDebugInfo()
{
	if (DEBUG_FPS)
	{
		std::cout << "Current FPS is " << 1 / DeltaTime << '\n';
	}
}

void ORenderer::GLFWCalcPerspective(GLFWwindow* window)
{
	glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);
	Aspect = static_cast<float>(ScreenWidth / ScreenHeight);
	PMat = glm::perspective(1.0472f, Aspect, 0.1f, 1000.F);

	VMat = glm::lookAt(GetCameraPosition() * -1.F, GetCameraPosition(), { 0, 1, 0 });
}

void ORenderer::TranslateCameraLocation(const glm::mat4& Transform)
{
	// CameraPos *= Transform;
}

void ORenderer::LookAtCamera(const OVec3& Position)
{
	glm::lookAt(GetCameraPosition(), Position, OVec3(0, 0, 1));
}

#pragma endregion GLFW

void ORenderer::CleanScene()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_CULL_FACE));
}

void ORenderer::CalcDeltaTime(const float currentTime)
{
	CurrentFrame = currentTime;
	DeltaTime = CurrentFrame - LastFrame;
	LastFrame = CurrentFrame;
}

void ORenderer::AddTest(Test::OTest* testPtr)
{
	if (testPtr)
	{
		testPtr->Init(PMat);
		Tests.push_back(testPtr);
	}
}
SBufferAttribVertexHandle ORenderer::AddAttributeBuffer(const SVertexContext& Context)
{
	return VertexArray.AddAttribBuffer(Context);
}

void ORenderer::EnableBufferAttribArray(const SDrawVertexHandle& Handle)
{
	VertexArray.EnableBufferAttribArray(Handle);
}

void ORenderer::EnableBufferAttribArray(const SBufferAttribVertexHandle& Handle)
{
	VertexArray.EnableBufferAttribArray(Handle);
}

SBufferAttribVertexHandle ORenderer::AddAttribBuffer(const OVertexAttribBuffer& Buffer)
{
	return VertexArray.AddAttribBuffer(Buffer);
}

SBufferAttribVertexHandle ORenderer::AddAttribBuffer(OVertexAttribBuffer&& Buffer)
{
	return VertexArray.AddAttribBuffer(Move(Buffer));
}
SBufferHandle ORenderer::AddBuffer(const void* Data, size_t Size)
{
	return VertexArray.AddBuffer(Data, Size);
}
void ORenderer::BindBuffer(const SBufferHandle& Handle)
{
	VertexArray.BindBuffer(Handle);
}
SBufferHandle ORenderer::AddBuffer(SBufferContext&& Context)
{
	return VertexArray.AddBuffer(Move(Context));
}

} // namespace RenderAPI
