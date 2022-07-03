#include <dragon/dragon10.hpp>

using namespace std::chrono;

Dragon::Video::dgWindow::dgWindow(int w, int h, const char* t, bool IFS, bool IBL) 
{
	width = w;
	height = h;
	title = (char*)title;

	updateWindow();
}

void Dragon::Video::dgWindow::updateWindow() {
	window = glfwCreateWindow(width, height, title, NULL, NULL);
}

void Dragon::Video::dgWindow::updateScreen() {

}

void Dragon::Video::dgWindow::startTiming() {

	cFrameTime = duration_cast<microseconds>();
}

DG_TIME_UNIT Dragon::Video::dgWindow::getTime() {
	cFrameTime = 
}

DG_TIME_UNIT Dragon::Video::dgWindow::getFPS() {
	return;
}

bool Dragon::Video::dgWindow::shouldWindowClose() {
	return glfwWindowShouldClose(window);
}

void Dragon::Video::dgWindow::getEvents() {
	glfwPollEvents();
}

void Dragon::Video::dgWindow::setWindowBorderless(bool IBL) {

}

void Dragon::Video::dgWindow::setWindowFullscreen(bool IFS) {

}

void Dragon::Video::dgWindow::close() {
	glfwDestroyWindow(window);
}