import requests
import subprocess
import multiprocessing
import os

from tools.installer.utils import isVulkanInstalled

def download(url):
	r = requests.get(url, allow_redirects=True)
	if not os.path.exists("cache/modules"):
		os.makedirs("cache/modules")
	open("cache/modules/" + url.split("/")[len(url.split("/")) - 1], 'w').write(r.content.decode())

def extract(path):
	f = open(path)
	f.close()

def install(path):
	pass

def install_all(window, values):
	git_version = subprocess.run(["git","--version"], 
		stdout=subprocess.PIPE,
		stderr=subprocess.PIPE, 
		text=True
	)

	if git_version.stdout == "" or git_version.stderr != "":
		print("GIT is either not installed on your system, or was not installed properly. Install GIT and try again.")
		exit(1)

	if not isVulkanInstalled():
		
	count = 0
	
	if values["-DGHI-"]:
		count += 1

	if values["-DGBI-"]:
		count += 1

	if values["-DGSI-"]:
		count += 2
		
	if values["-BSTI-"]:
		count += 1

	current_download = 1
	current_extract = 0
	current_install = 0
	window["-DPBT-"].update("Downloading https://github.com/KhrySystem/Dragon.git (" + str(current_download) + "/" + str(count) + ")")
	window["-EPBT-"].update("Waiting... (" + str(current_extract) + "/" + str(count) + ")")
	window["-IPBT-"].update("Waiting... (" + str(current_extract) + "/" + str(count) + ")")