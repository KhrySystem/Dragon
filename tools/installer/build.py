import requests
import subprocess
import multiprocessing
import os
import platform

auto_build = False

# This required Git to be installed.

def is_bit_set(x, n):
    return x & 1 << n != 0

def is_vulkan_installed():
	return subprocess.Popen(['vulkaninfo'], stdout=PIPE, stderr=PIPE)).stderr == ""

def clone(url):
	os.system((f"git clone https://github.com/{}", url))

def main(options, path = ""):
	if path == "":
		if platform.system() == 'Windows':
			path = "C:/Dragon"
		elif platform.system() == 'Darwin':
			path = "
	
	tasks = ["Khrysystem/Dragon.git"]
	if is_bit_set(options, 0): # Vulkan installation
		tasks.append("KhronosGroup/Vulkan-Headers")
		tasks.append("KhronosGroup/Vulkan-ValidationLayers")
	if is_bit_set(options, 1): # Tests, so install GTest and GMock
		tasks.append("Google/GoogleTest")
	if is_bit_set(options, 2): # Automatically build DragonEngine
		auto_build = True
	
	if !os.path.exists(f"{}/cache", path):
		os.makedirs(f"{}/cache", path):
	
	for task in tasks:
		pass
