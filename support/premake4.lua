solution "exhaust_editor"
   debugdir "../debug"

   -- Create default solution
   platforms { "x32" }
   configurations { "Debug", "Release" }

   -- Setup environment
   location (_WORKING_DIR.."/"..(_ACTION or ""))
   targetdir (_WORKING_DIR.."/../bin")
   vpaths { ["**"]  = {"../source/**" } }
   
   -- Debug/Release configurations and correct debug suffix
   configuration "Debug*"
      defines { "DEBUG" }
      flags { "Symbols" }
   configuration "Release*"
      flags { "Optimize", "Symbols" }
   
   -- Generate suffixes
   configuration { "Debug*" }
      targetsuffix "d"
   configuration { "Release*" }
      targetsuffix ""
   
   -- Linux specific
   configuration { "not windows" }
      links { "m", "pthread" }
      linkoptions { "-lstdc++" }

-- Create working directory
if not os.isdir("../debug") then os.mkdir("../debug") end




--------------------------------------------------------------------------------
-- Qt-related code and commands
--------------------------------------------------------------------------------
newoption {
   trigger     = "qtpath",
   description = "Path to Qt (used for foxworks_editor)",
   value       = "PATH"
}

newaction {
   trigger     = "moc",
   description = "Generate MOC files for Qt",
}

-- Default Qt paths
local QtDefault = "C:/Qt/4.8.4"
local QtMOCPath = "/bin/moc.exe"
local QtRCCPath = "/bin/rcc.exe"
local QtGenPath = "../qtmoc"

-- Set new default paths for Linux
if os.get() ~= "windows" then
  QtDefault = "/usr/share/qt4"
  QtMOCPath = "/bin/moc"
  QtRCCPath = "/bin/rcc"
end

-- Set possible Qt path
local QtPath = _OPTIONS["qtpath"] or QtDefault

-- Check if Qt is installed (assume installed under windows)
if not os.isfile(QtPath..QtMOCPath) then
  print("Qt not found at paths:")
  print("",QtDefault)
  if _OPTIONS["qtpath"] then print("",_OPTIONS["qtpath"]) end
  QtPath = nil
end

-- Generate MOC files
GENERATED_MOCs = false
function generate_moc(files,do_warn,extrapath)
  local moc_files = os.matchfiles(files)
  for _,file in pairs(moc_files) do
    local file_ext = path.getextension(file)
    local out_file = QtGenPath..(extrapath or "").."/moc_"..path.getbasename(file)..".cpp"
    if file_ext == ".cpp" then
      out_file = QtGenPath..(extrapath or "").."/"..path.getbasename(file)..".moc"
    end

    local warnings = " -nw "
    if do_warn then warnings = " " end

    -- Get last modify date
    local mname = path.getdirectory(out_file).."/"..path.getbasename(out_file)..".tmp"
    local mtime = 0
    local mtest = io.open(mname,"r")
    if mtest then
      mtime = tonumber(mtest:read("*a"))
    end

    -- If applies, generate new file
    if mtime ~= os.stat(file).mtime then
      if GENERATED_MOCs == false then -- Only show message when generating
        print("Building MOC's...")
        GENERATED_MOCs = true
      end

      os.execute(QtPath..QtMOCPath.." "..file..warnings.."-o "..out_file)
    end

    -- Write last modify date
    mtest = io.open(mname,"w+")
    if mtest then
      mtest:write(os.stat(file).mtime)
    end
  end
end

-- Generate RCC files
GENERATED_RCCs = false
function generate_rcc(files,extrapath)
  local rcc_files = os.matchfiles(files)
  for _,file in pairs(rcc_files) do
    local file_ext = path.getextension(file)
    local out_file = QtGenPath..(extrapath or "").."/rcc_"..path.getbasename(file)..".cpp"

    -- Get last modify date
    local mname = path.getdirectory(out_file)..(extrapath or "").."/"..path.getbasename(out_file)..".tmp"
    local mtime = 0
    local mtest = io.open(mname,"r")
    if mtest then
      mtime = mtest:read("*n")
    end

    -- If applies, generate new file
    if mtime ~= os.stat(file).mtime then
      if GENERATED_RCCs == false then -- Only show message when generating
        print("Building RCC's...")
        GENERATED_RCCs = true
      end

      os.execute(QtPath..QtRCCPath.." -name "..path.getbasename(file).." -o "..out_file.." "..file)
    end

    -- Write last modify date
    mtest = io.open(mname,"w+")
    if mtest then
      mtest:write(os.stat(file).mtime)
    end
  end
end

-- Generate MOC's for Qt
if QtPath and (_ACTION ~= "clean") then
  if not os.isdir(QtGenPath) then
    os.mkdir(QtGenPath)
  end

  -- Foxworks editor
  generate_moc("../source/**.h",true)
  generate_rcc("../resources/**.qrc")
end

if _ACTION == "moc" then return end


--------------------------------------------------------------------------------
-- Exhaust Editor
--------------------------------------------------------------------------------
project "exhaust_editor"
   uuid "891F65A4-62D3-4A4B-AA2D-3B1CB760BBD6"
   kind "ConsoleApp"
   language "C++"

   includedirs {

   }
   files {
     "../source/**.cpp",
     "../source/**.h",
     "../source/**.qrc",
   }
   links { }
   
   
   -- Additional Qt stuff
   includedirs {
     QtPath.."/include",
     QtPath.."/include/QtCore",
     QtPath.."/include/QtGui",
     QtPath.."/include/QtUiTools",
     QtPath.."/include/QtOpenGL",
     QtGenPath
   }
   libdirs { QtPath.."/lib" }
   files {
--     QtGenPath.."/rcc_resources.cpp",
     QtGenPath.."/moc_*.cpp"
   }

   configuration { "windows", "Debug*" }
      links { "QtCored4", "QtGuid4", "QtUiToolsd", "QtOpenGLd4", "opengl32" }
   configuration { "not windows", "Debug*" }
      links { "QtCored", "QtGuid", "QtUiToolsd", "QtOpenGLd" }
   configuration { "windows", "Release*" }
      links { "QtCore4", "QtGui4", "QtUiTools", "QtOpenGL4", "opengl32" }
   configuration { "not windows", "Release*" }
      links { "QtCore", "QtGui", "QtUiTools", "QtOpenGL" }
