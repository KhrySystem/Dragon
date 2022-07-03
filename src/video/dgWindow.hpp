#ifndef VIDEO__DG_WINDOW_HPP
#define VIDEO__DG_WINDOW_HPP

/* dgWindow.hpp

defines Dragon::Video::dgWindow and assorted methods. 

*/
#include <dragon/internal.hpp>
#include <dragon_include_lib.hpp>
#include <object/dgObject.hpp>

namespace Dragon
{
	namespace Video
	{
		struct dgWindow
		{
			/*
			dgWindow - Dragon's wrapper around GLFWwindow to encompass Vulkan. 
			Produces the root parent for dgObjects.
			*/
			dgWindow(int width, int height, const char* title, bool IFS, bool IBL);

			void updateWindow();
			void updateScreen();

			void startTiming();
			DG_TIME_UNIT getTime();
			DG_TIME_UNIT getFPS();

			bool shouldWindowClose();

			void getEvents();

			void setWindowBorderless(bool IBL);
			void setWindowFullscreen(bool IFS);

			void close();

			int width;
			int height;
			char* title;

			GLFWwindow* window;

			DG_TIME_UNIT pFrameTime;
			DG_TIME_UNIT cFrameTime;

			bool isFullscreen;
			bool isBorderless;

			std::vector<Dragon::Object::dgObject*> gameObjects();
		};
		
	} // namespace Video
	
} // namespace Dragon

#endif // VIDEO__DG_WINDOW_HPP