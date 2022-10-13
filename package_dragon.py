import os
#from pathlib import Path
import shutil
import sys

PATH = os.getenv("Path").split(";")

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

def copy_all(new_path : str, rel_type: str="RelWithDebInfo") -> bool:
	shutil.rmtree(new_path)
	copy_headers_in_path(os.getcwd() + "\\headers\\include\\dragon\\", new_path + "\\include\\dragon\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\glfw\\include\\glfw\\", new_path + "\\include\\glfw\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\glm\\glm\\", new_path + "\\include\\glm\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\include\\al\\", new_path + "\\include\\al\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\glfw\\deps\\", new_path + "\\include\\glfw\\deps\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\glfw\\src\\", new_path + "\\include\\glfw\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\al\\", new_path + "\\include\\al\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\alc\\", new_path + "\\include\\alc\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\common\\", new_path + "\\include\\common\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\core\\", new_path + "\\include\\core\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\router\\", new_path + "\\include\\router\\")
	copy_headers_in_path(os.getcwd() + "\\headers\\openal\\utils\\", new_path + "\\include\\utils\\")
	copy_headers_in_path(os.getcwd() + "\\extensions\\ironbreath\\include\\", new_path + "\\include\\")
	copy_headers_in_path(os.getcwd() + "\\extensions\\streambreath\\include\\", new_path + "\\include\\")
	copy_headers_in_path(os.getcwd() + "\\extensions\\lightbreath\\include\\", new_path + "\\include\\")
	
	copy_source_in_path(os.getcwd() + "\\headers\\glfw\\src\\", new_path + "\\src\\glfw\\")
	copy_source_in_path(os.getcwd() + "\\headers\\glfw\\deps\\", new_path + "\\src\\glfw\\deps\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\al\\", new_path + "\\src\\al\\al\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\alc\\", new_path + "\\src\\al\\alc\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\common\\", new_path + "\\src\\al\\common\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\core\\", new_path + "\\src\\al\\core\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\router\\", new_path + "\\src\\al\\router\\")
	copy_source_in_path(os.getcwd() + "\\headers\\openal\\utils\\", new_path + "\\src\\al\\utils\\")
	copy_source_in_path(os.getcwd() + "\\extensions\\ironbreath\\src\\", new_path + "\\src\\extensions\\iron\\")
	copy_source_in_path(os.getcwd() + "\\extensions\\streambreath\\src\\", new_path + "\\src\\extensions\\stream\\")
	copy_source_in_path(os.getcwd() + "\\extensions\\lightbreath\\src\\", new_path + "\\src\\extensions\\light\\")
	copy_source_in_path(os.getcwd() + "\\tools\\tests\\", new_path + "\\tests\\")

	copy_binaries_in_path(os.getcwd() + "\\bin\\headers\\GLFW\\src\\" + rel_type + "\\", new_path + "\\bin\\glfw\\")
	copy_binaries_in_path(os.getcwd() + "\\bin\\headers\\openal\\" + rel_type + "\\", new_path + "\\bin\\al\\")
	copy_binaries_in_path(os.getcwd() + "\\bin\\src\\" + rel_type + "\\", new_path + "\\bin\\")
	copy_binaries_in_path(os.getcwd() + "\\bin\\extensions\\ironbreath\\" + rel_type + "\\", new_path + "\\bin\\")
	copy_binaries_in_path(os.getcwd() + "\\bin\\extensions\\streambreath\\" + rel_type + "\\", new_path + "\\bin\\")
	copy_binaries_in_path(os.getcwd() + "\\bin\\extensions\\lightbreath\\" + rel_type + "\\", new_path + "\\bin\\")
	copy_binaries_in_path(os.getcwd() + "\\bin\\" + rel_type + "\\", new_path + "\\bin\\tests\\")

	return True

if __name__ == "__main__":
	copy_all(sys.argv[1], sys.argv[2])
