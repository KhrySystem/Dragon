#ifndef VIDEO__DG_WINDOW_HPP
#define VIDEO__DG_WINDOW_HPP

/* dgWindow.hpp

defines Dragon::Video::dgWindow and assorted methods. 

*/
#include <dragon_include_lib.hpp>
#include <object/dgObject.hpp>

namespace Dragon
{
	namespace Video
	{
		struct dgWindow
		{
			/*
			dgWindow - Vulkan wrapper around GLFWwindow. 
			Produces the root parent for dgObjects.
			*/
			dgWindow(int width, int height, const char* title, bool IFS, bool IBL);

			void updateWindow();
			void update(double dTime);

			bool shouldWindowClose();

			void getEvents();

			void setWindowBorderless(bool IBL);
			void setWindowFullscreen(bool IFS);

			private:
				int width;
				int height;
				char* title;

				GLFWwindow* window;

				bool isFullscreen;
				bool isBorderless;

				std::vector<Dragon::Object::dgObject*> gameObjects();
		};
		
	} // namespace Video
	
} // namespace Dragon

#endif // VIDEO__DG_WINDOW_HPP