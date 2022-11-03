import os
import shutil

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
		try:
			copy_headers_in_path(src + d + "\\", dest + d + "\\")
		except:
			pass

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
		try:
			copy_source_in_path(src + d + "\\", dest + d + "\\")
		except:
			pass
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
			copy_binaries_in_path(src + d + "\\", dest + d + "\\")
		except:
			pass
