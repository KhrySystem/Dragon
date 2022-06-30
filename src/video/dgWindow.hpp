#include <dragon_include_lib.hpp>

namespace Dragon
{
	namespace Video
	{
		struct dgWindow
		{
			dgWindow(int width, int height, const char* title, bool IFS, bool IBL);

			void update(double dTime);

			bool shouldWindowClose();

			private:
				int width;
				int height;
				char* title;

				GLFWwindow* window;

				bool isFullscreen;
				bool isBorderless;
		};
		
	} // namespace Video
	
} // namespace Dragon
