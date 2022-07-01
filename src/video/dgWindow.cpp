#include "dgWindow.hpp"

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

void Dragon::Video::dgWindow::getEvents() {
	glfwPollEvents();
}

bool Dragon::Video::dgWindow::shouldWindowClose() {
	return glfwWindowShouldClose(window);
}