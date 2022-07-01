#include <dragon/dragon10.hpp>

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

void Dragon::Video::dgWindow::update() {

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