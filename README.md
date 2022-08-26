### DragonEngine


## Preprocessor Definitions
DRAGON_USE_VULKAN - Vulkan was found and can be used.
DRAGON_USE_OPENGL - OpenGL was found and can be used. 
DRAGON_BOOST_FOUND - Boost was installed on the system and will be included to speed up certain functions. (```AccelDragon<lib_name>``` is the Dragon Alias for Boost.)

# Vulkan-Specific Preprocessor Definitions
DRAGON_VK_GLSLC_FOUND - Vulkan GLSLC Executable was found and can be used to compile certain files.
DRAGON_VK_GLSLANG_FOUND - Vulkan GLSLang Executable was found and can be used to compile certain files.
DRAGON_VK_GLSLANG_VALIDATOR_FOUND - Vulkan GLSLang Validation Executable was found and will be used in DEBUG compile builds.
DRAGON_VK_SHADERC_COMBINED_FOUND - Vulkan ShaderC Combined Library was found and will be added as an utility library.
DRAGON_VK_SPIRV_TOOLS_FOUND - SPIRV Tools Libraries were found and will be added so Vulkan doesn't break.
DRAGON_VK_MOLTENVK_FOUND - Vulkan MoltenVK Library was found and will be added in an iOS, tvOS, or macOS build.

# OpenGL-Specific Preprocessor Definitions
DRAGON_GL_OPENGL_FOUND - General OpenGL libraries were found (Seems redundant with DRAGON_USE_OPENGL, but suprisingly necessary)
DRAGON_GL_XMESA_FOUND - XMesa OpenGL libraries were found will be included in case a project needs them.
DRAGON_GL_GLU_FOUND - OpenGL Utility Library was found and will be included for backend logic.
DRAGON_GL_GLX_FOUND - OpenGL XWindows Extension was found and Dragon needs to adjust for this.
DRAGON_GL_EGL_FOUND - OpenGL Linux-Specific EGL was found and Dragon needs to rearrange basically everything to adjust.
