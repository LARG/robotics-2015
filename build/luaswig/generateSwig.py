#!/usr/bin/env python

import sys, os, subprocess, re

#basePath = os.path.join(os.getenv('NAO_HOME'),'core')

noCopyConstructors = [
  'VisionCore'
]

sepPatterns = [
  '.*lua_pushvalue\(L,LUA_GLOBALSINDEX\).*',
  '.*SWIG_PropagateClientData.*',
  '.*SWIG_Lua_module_begin*',
  '.*end module.*'
]
sepNames = [
  'initializeModule',
  'beginModule',
  'internal',
  'end'
]
sepBase = '''}
SWIGEXPORT int luaopen_swig_%s_%s(lua_State *L) {
int i;
'''
sepBase2 = '''}
SWIGEXPORT int luaopen_swig_%s_%s(lua_State *L, const char *name) {
int i;
'''

#templates = [
  #['vector2_float', 'Vector2<float>'],
  #['vector3_float', 'Vector3<float>'],
  #['vector_float', 'std::vector<float>'],
  #['vector_double', 'std::vector<double>'],
  #['vector_int', 'std::vector<int>']
#]

copyConstructorString = '%%feature("copyctor","%s");'

TEMPLATE = '''
%%module swig_%s

%%{
#include "%s"
typedef int int32_t; typedef short int16_t;
typedef unsigned int uint32_t; typedef unsigned short uint16_t; typedef unsigned char uint8_t;
%%}

typedef int int32_t; typedef short int16_t;
typedef unsigned int uint32_t; typedef unsigned short uint16_t; typedef unsigned char uint8_t;
%s

%s

%s

'''

TEMPLATE2 = '''
%%module swig_%s

%%{
%s
%%}

%s

%s

%s

'''

def getMangledName(name):
  mangledName = name
  mangledName = mangledName.replace('/','_')
  mangledName = mangledName.replace('.h','')
  mangledName = mangledName.replace('.i','')
  #mangledName += '_wrap'
  return mangledName

#def generateOpenLuaLibs(filename,files):
  #with open(filename,'w') as f:
    #f.write('#include <lua/LuaInterp.h>\n')
    #f.write([>
##ifndef SWIGEXPORT
## if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
##   if defined(STATIC_LINKED)
##     define SWIGEXPORT
##   else
##     define SWIGEXPORT __declspec(dllexport)
##   endif
## else
##   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
##     define SWIGEXPORT __attribute__ ((visibility("default")))
##   else
##     define SWIGEXPORT
##   endif
## endif
##endif

##ifdef __cplusplus
#extern "C" {
##endif
#[>)
    #for name in files:
      #f.write('extern "C" int luaopen_swig_%s(lua_State *L);\n' % (name))
    #f.write('int luaopen_core(lua_State *L) {\n')
    #for name in files:
      #f.write('  luaopen_swig_%s(L);\n' % (name))
    #f.write('  return 1;\n')
    #f.write('}\n')
    #f.write([>
##ifdef __cplusplus
#}
##endif
#[>)

#def generateOpenLuaLibsLua(files):
  #filename = os.path.join(os.getenv('NAO_HOME'),'core','lua','openLuaLink.lua')
  #with open(filename,'w') as f:
    #f.write([>
#function tableMerge(t1, t2)
  #for k,v in pairs(t2) do
    #if type(v) == "table" then
      #if type(t1[k] or false) == "table" then
        #tableMerge(t1[k] or {}, t2[k] or {})
      #else
        #t1[k] = v
      #end
    #else
      #t1[k] = v
    #end
  #end
  
  #mt1 = getmetatable(t1)
  #mt2 = getmetatable(t2)

  #if mt1 then
    #tableMerg(mt1,mt2)
  #else
    #setmetatable(t1,mt2)
  #end

  #return t1
#end
#core = {}
#[>)
    #for name in files:
      #f.write('mergeTable(core,swig_%s)\n' % name)

def generateOpenLuaLibs(filename,files):
  with open(filename,'w') as f:
    f.write('#include <lua/LuaInterp.h>\n')
    for i,name in enumerate(files):
      for sepName in sepNames:
        if sepName == 'beginModule':
          if i == 0:
            f.write('extern "C" int luaopen_swig_%s_%s(lua_State *L, const char *c);\n' % (name,sepName))
        else:
          if sepName == 'end':
            continue
          f.write('extern "C" int luaopen_swig_%s_%s(lua_State *L);\n' % (name,sepName))
    name = files[0]
    start = 'luaopen_swig_%s' % name
    beginModule = 'luaopen_swig_%s_%s' % (name,sepNames[1])
    end = 'luaopen_swig_%s_end' % name
    f.write('extern "C" int %s(lua_State *L);\n' % start)
    f.write('extern "C" int %s(lua_State *L);\n' % end)
    f.write('\n')
    f.write('''
#ifndef SWIGEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define SWIGEXPORT
#   else
#     define SWIGEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define SWIGEXPORT __attribute__ ((visibility("default")))
#   else
#     define SWIGEXPORT
#   endif
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
''')
    f.write('SWIGEXPORT int luaopen_core(lua_State *L) {\n')
    f.write('  %s(L);\n' % start)
    for name in files:
      f.write('  luaopen_swig_%s_%s(L);\n' % (name,sepNames[0]))
    f.write('  %s(L,"core");\n' % beginModule)
    for name in files:
      f.write('  luaopen_swig_%s_%s(L);\n' % (name,sepNames[2]))
    f.write('  return %s(L);\n' % end)
    f.write('}\n')
    f.write('''
#ifdef __cplusplus
}
#endif
''')


def getSourceName(line):
  print getMangledName(line)

def getSourceNames(inFile):
  with open(inFile,'r') as f:
    lines = [x.strip() for x in f.readlines()]
  for line in lines:
    if len(line) > 0:
      print getMangledName(line)

def generateI(name,path,outputFile,names):
  path = os.path.normpath(path)
  importStr = ''
  for n in names:
    if n == name:
      continue
    importStr += '%%import "lua_%s.i"\n' % n
    #importStr += '%%import "%s.h"\n' % n
  importStr += '%import std_vector.i\n'
  importStr += '%import std_string.i\n'
  includeStr = '%%include "%s"\n' % path
  copyStr = copyConstructorString % (0 if (name in noCopyConstructors) else 1)
  contents = TEMPLATE % (name,path,importStr,copyStr,includeStr)
  with open(outputFile,'w') as f:
    f.write(contents)

def generateTemplateI(name,typeName,outputFile):
  basePath = os.path.join(os.getenv('NAO_HOME'),'core')
  with open(outputFile,'w') as f:
    if typeName.startswith('Vector2'):
      #importStr =  '%include math_Vector2.i\n'
      importStr = '%%include "%s"\n' % os.path.join(basePath,'math','Vector2.h')
      include = '#include "%s"\n' % os.path.join(basePath,'math','Vector2.h')
    elif typeName.startswith('Vector3'):
      #importStr += '%include math_Vector3.i\n'
      importStr = '%%include "%s"\n' % os.path.join(basePath,'math','Vector3.h')
      include = '#include "%s"\n' % os.path.join(basePath,'math','Vector3.h')
    else:
      importStr = '%include std_vector.i\n'
      include = ''
    templateStr = '%%template(%s) %s;' % (name,typeName)
    copyStr = copyConstructorString % (0 if (name in noCopyConstructors) else 1)
    f.write(TEMPLATE2 % (name,include,importStr,copyStr,templateStr))
  #subprocess.check_call('swig2.0 -lua -c++ -o %s %s' % (cppFile,iFile),shell=True)

#def generateI(outDir,names,files):
  #names = [n.strip() for n in names]
  #files = [f.strip() for f in files]
  #imports = []
  #for n in names:
    #imports.append('%%import "%s.i"\n' % n)
  #basePath = os.path.join(os.getenv('NAO_HOME'),'core')
  #for n,f in zip(names,files):
    #path = os.path.join(basePath,f)
    #ind = names.index(n)
    #importStr = ''.join(imports[:ind]+imports[ind+1:])
    #importStr += '%import std_vector.i\n'
    #importStr += '%import std_string.i\n'
    #includeStr = '%%include "%s"\n' % path
    #contents = TEMPLATE % (n,path,importStr,includeStr)
    #outputFile = os.path.join(outDir,'%s.i' % (n))
    #with open(outputFile,'w') as f:
      #f.write(contents)

def generateCpp(name,iFile,cppFile):
  assert False, "\n\
\n\
Lua support was discontinued after RoboCup2015 with the move from Ubuntu \
12.04 to Ubuntu 14.04. To re-enable support you must either use SWIG 2.04 \
or update this function to correctly substitute in the VisionCore initialization \
functions.\n\
\n\
"

  subprocess.check_call('swig2.0 -w401,503 -lua -c++ -o %s %s' % (cppFile,iFile),shell=True)

  with open(cppFile,'r') as f:
    lines = f.readlines()
  
  inds = []
  patternInd = 0
  for i,line in enumerate(lines):
    if re.match(sepPatterns[patternInd],line):
      inds.append(i+1)
      patternInd += 1
      if patternInd >= len(sepPatterns):
        break
  assert(len(inds) == len(sepPatterns))
  newLines = []
  lastInd = 0
  for ind,sepName in zip(inds,sepNames):
    if sepName == 'beginModule':
      i = lines.index('  SWIG_Lua_module_begin(L,SWIG_name);\n')
      lines[i] = '  SWIG_Lua_module_begin(L,name);\n'
      newLines += lines[lastInd:ind] + [sepBase2 % (name,sepName)]
    else:
      newLines += lines[lastInd:ind] + [sepBase % (name,sepName)]
    lastInd = ind
  newLines += lines[lastInd:]

  with open(cppFile,'w') as f:
    f.writelines(newLines)

if __name__ == '__main__':
  args = sys.argv[1:]
  if '--getSourceName' in args:
    args.remove('--getSourceName')
    line = args[0]
    getSourceName(line)
  elif '--getSourceNames' in args:
    args.remove('--getSourceNames')
    inFile = args[0]
    getSourceNames(inFile)
  elif '--generateI' in args:
    args.remove('--generateI')
    name = args[0]
    path = args[1]
    outputFile = args[2]
    names = args[3:]
    generateI(name,path,outputFile,names)
    #outDir = args[0]
    #ind = args.index('--')
    #names = args[1:ind]
    #files = args[ind+1:]
    #assert(len(names) == len(files))
    #generateI(outDir,names,files)
  elif '--generateTemplateI' in args:
    args.remove('--generateTemplateI')
    name = args[0]
    template = args[1]
    outputFile = args[2]
    generateTemplateI(name,template,outputFile)
  elif '--generateCpp' in args:
    args.remove('--generateCpp')
    name = args[0]
    iFile = args[1]
    outputFile = args[2]
    generateCpp(name,iFile,outputFile)
  elif '--generateOpenLuaLibs' in args:
    args.remove('--generateOpenLuaLibs')
    filename = args[0]
    names = args[1:]
    generateOpenLuaLibs(filename,names)
    #generateOpenLuaLibsLua(names)
  else:
    print >>sys.stderr,args
    raise ValueError
  #else:
    #main(args[0],args[1])
