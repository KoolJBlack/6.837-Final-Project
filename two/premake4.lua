-- Premake script.
-- Generate Makefile (for Athena, Linux, Cygwin, Mac OS X):
--   premake4 --os=linux gmake
-- Generate Visual Studio (not necessary if you add new files from Visual Studio itself):
--   premake4 --os=windows vs2010
assignment = "a2"

solution(assignment)
  configurations { "Debug", "Release" }
  if not os.is("windows") then
    buildoptions{"-std=c++0x"}
  else
    buildoptions{"/D WIN32"}
  end
  
  configuration { "Debug" }
    targetdir "debug"
 
  configuration { "Release" }
    targetdir "release"
project(assignment)
  language "C++"
  kind     "ConsoleApp"
  files  { "src/**.cpp", "src/**.h" }
  includedirs {"vecmath/include"}
  
  if not os.is("windows") then
    linkoptions {[[`if [ "\`uname\`" = "Darwin" ]; then echo "-framework OpenGL -framework GLUT -framework Cocoa -framework AGL -framework Carbon -lfltk -lfltk_gl"; else echo "-lGL -lGLU -lglut "; fltk-config --use-gl --ldflags; fi;`]]}
  else
    links {"freeglut", "fltk", "fltkgl", "fltkforms", "fltkimages", "fltkzlib", "fltkjpeg", "fltkpng"}
    includedirs {"include"}
    libdirs {"lib"}
  end
  links {"vecmath"}
  
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
    if not os.is("windows") then
      postbuildcommands {"cp debug/" .. assignment .. " " .. assignment}
    end

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" } 
    if not os.is("windows") then
      postbuildcommands {"cp release/" .. assignment .. " " .. assignment}
    end

include("vecmath")