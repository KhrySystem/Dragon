#include <dragon/internal.hpp>
#include <dragon_include_lib.hpp>
#include <object/dgObject.hpp>
#include <video/dgWindow.hpp>

namespace Dragon
{
	namespace Video
	{
		/*
		Optional wrapper of dgWindow that allows for asynchronous and non-blocking calls to window methods
		*/
		struct dgAsyncWindow : public dgWindow {

			void asyncUpdateWindow();
			void asyncUpdateScreen();
			
			void asyncSetWindowBorderless();
			void asyncSetWindowFullscreen();

			void waitForAsyncCompletion();
			bool isAsyncCompleted();

			private:
				bool asyncComplete; // True if no async processes are running
		};
	} // namespace Video
	
} // namespace Dragon
