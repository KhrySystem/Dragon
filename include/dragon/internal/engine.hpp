#pragma once

struct Engine {
	std::string name;

	Audio::Engine audio;
	Graphics::Engine graphics;
	Message::Engine message;
};