#pragma once
#include <Test.hpp>
#include <Texture.hpp>

class GLFWwindow;
namespace Test
{
    class TestTexture : public Test
    {
    public:
        TestTexture(String filePath);
        ~TestTexture();

        void OnUpdate(GLFWwindow *window, float currentTime);

    private:
        Texture texture;
    };

} // namespace test
