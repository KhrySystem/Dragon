import os
#from pathlib import Path
import shutil
import sys


def get_files(directory: str) -> list:
	items = os.listdir(directory)
	files = []
	for i in items:
		if "." in i:
			files.append(i)

	return files

def get_dirs(directory: str) -> list:
	items = os.listdir(directory)
	dirs = []
	for i in items:
		if not "." in i:
			dirs.append(i)

	return dirs
def get_items(directory: str) -> tuple:
	return (get_dirs(directory), get_files(directory))

def copy_headers_in_path(src: str, dest: str):
	dirs, f_names = get_items(src)
	try:
		os.makedirs(dest)
	except:
		pass
	for f in f_names:
		print(src + f)
		if ".h" in f:
			shutil.copy(src + f, dest + f)
	for d in dirs:
		copy_headers_in_path(src + d + "\\", dest + d + "\\")

def copy_source_in_path(src: str, dest: str):
	dirs, f_names = get_items(src)
	try:
		os.makedirs(dest)
	except:
		pass
	for f in f_names:
		print(src + f)
		if ".c" in f:
			shutil.copy(src + f, dest + f)
	for d in dirs:
		copy_headers_in_path(src + d + "\\", dest + d + "\\")

def copy_binaries_in_path(src: str, dest: str):
	dirs, f_names = get_items(src)
	try:
		os.makedirs(dest)
	except:
		pass
	for f in f_names:
		print(src + f)
		if ".exe" in f:
			shutil.copy(src + f, dest + f)
		elif ".dll" in f:
			shutil.copy(src + f, dest + f)
		elif ".pdb" in f:
			shutil.copy(src + f, dest + f)
		elif ".exp" in f:
			shutil.copy(src + f, dest + f)
		elif ".lib" in f:
			shutil.copy(src + f, dest + f)

	for d in dirs:
		try:
			copy_headers_in_path(src + d + "\\", dest + d + "\\")
		except:
			pass
		

def copy_all(new_path : str) -> bool:
	shutil.rmtree(new_path)
	copy_headers_in_path(os.getcwd() + "\\headers\\include\\dragon\\", new_path + "\\include\\dragon\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\glfw\\include\\glfw\\", new_path + "\\include\\glfw\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\glm\\glm\\", new_path + "\\include\\glm\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\include\\al\\", new_path + "\\include\\al\\")
	for d in get_dirs(os.getcwd() + "\\headers\\boost\\libs\\"):
		if os.path.exists(os.getcwd() + "\\headers\\boost\\libs\\" + d + "\\include\\boost\\"):
			try:
				copy_headers_in_path(os.getcwd() + "\\headers\\boost\\libs\\" + d + "\\include\\boost\\", new_path + "\\include\\boost\\")
			except:
				pass

	
	copy_headers_in_path(os.getcwd() + "\\headers\\glfw\\src\\", new_path + "\\src\\glfw\\")
	copy_source_in_path(os.getcwd() + "\\headers\\glfw\\src\\", new_path + "\\src\\glfw\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\glfw\\deps\\", new_path + "\\src\\glfw\\deps\\")
	copy_source_in_path(os.getcwd() + "\\headers\\glfw\\deps\\", new_path + "\\src\\glfw\\deps\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\al\\", new_path + "\\src\\al\\al\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\al\\", new_path + "\\src\\al\\al\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\alc\\", new_path + "\\src\\al\\alc\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\alc\\", new_path + "\\src\\al\\alc\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\common\\", new_path + "\\src\\al\\common\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\common\\", new_path + "\\src\\al\\common\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\core\\", new_path + "\\src\\al\\core\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\core\\", new_path + "\\src\\al\\core\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\router\\", new_path + "\\src\\al\\router\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\router\\", new_path + "\\src\\al\\router\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\utils\\", new_path + "\\src\\al\\utils\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\utils\\", new_path + "\\src\\al\\utils\\")


if __name__ == "__main__":
	copy_all(sys.argv[1])
