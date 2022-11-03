from package_dragon import *
from os import *

print(getcwd())
cd = getcwd().split("\\")
cd.remove("tools")
cd.remove("installer")
print(cd)
path = ""
for c in cd:
    path += c + "\\"
DRAGON_ROOT = path
del path
print(DRAGON_ROOT)
PATH = getenv("PATH")
print(PATH)
PATH_ARRAY = PATH.split(";")
print(PATH_ARRAY)

for i in PATH_ARRAY:
    print(i)
    if(os.path.exists(i + "vulkan-1.lib")):
        VULKAN_BIN_PATH = i

print(VULKAN_BIN_PATH)