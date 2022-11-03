import json
import os
import sys

data = open("download_info.json").read()
data = json.loads(data)

print(data)

def is_vulkan_installed():
    return (os.getenv("VK_SDK_PATH") != None) or (os.getenv("VULKAN_SDK") != None)

def vulkan_download():
    print("Downloading Vulkan")

def vulkan_build():
    print("Building Vulkan")

def vulkan_install():
    print("Installing Vulkan")



def main():
    if not is_vulkan_installed():
        vulkan_download()
        vulkan_build()
        vulkan_install()
    else:
        print("Vulkan Already Installed")

if __name__ == "__main__":
    main()