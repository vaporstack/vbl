#! /usr/bin/env python
# encoding: utf-8

import sys
import os
#arch = os.uname()[4]

from waflib import Options
from waflib import TaskGen
import glob
import subprocess

verbose = False
r4_root = os.path.expanduser("~/art/r4")
print(r4_root)

def options(opt):
	opt.load('compiler_c')
	opt.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_c')
	conf.load('compiler_cxx')
	print('→ configuring the project in ' + conf.path.abspath())
	conf.env.append_unique("CFLAGS", ["-mmacosx-version-min=10.7"])
	conf.env.append_unique("LINKFLAGS", ["-mmacosx-version-min=10.7"])

def read_file(path):
	with open("%s/etc/conf/lib-%s.txt" % (r4_root, path)) as f:
		things = f.read().replace("\n"," ")
	things = things.split(" ")
	things = [x.strip() for x in things if x.strip() != ""]

	return things

def build(bld):
	src = []
	incpath = []
	libpath = []
	incpath += ["../", "./"]

	if "darwin" in sys.platform:
		#	homebrew
		incpath += ['/usr/local/include']
		libpath += ["/usr/local/lib"]
		TaskGen.task_gen.mappings['.mm'] = TaskGen.task_gen.mappings['.cxx']
		TaskGen.task_gen.mappings['.m'] = TaskGen.task_gen.mappings['.cxx']
		incpath.append("/opt/X11/include/freetype2")

		libpath.append("/usr/local/opt")
		incpath.append('/usr/local/Cellar/libxml2/2.9.4_2/include/libxml2')
	elif "linux" in sys.platform:
		libpath += ["/usr/lib/x86_64-linux-gnu"]
		print "Linux libpaths!:", libpath
	elif "win" in sys.platform:
		libpath += ["%s/contrib/platform/win32" % r4_root]

	libpath.append("%s/contrib/platform/%s" % (r4_root, sys.platform))
	target_name = bld.path.abspath().split(os.sep)[-1]
	src += bld.path.ant_glob("src/*.c")

	#src += bld.path.ant_glob("%s/contrib/deps/**/*.c" % r4_root)
	#src += bld.path.ant_glob("%s/contrib/src/*.m" % r4_root)
	incpath.append("%s/contrib" % r4_root)
	incpath.append("%s/contrib/include" % r4_root)
	libs = read_file("common")
	libs += read_file(sys.platform)

	bld.stlib(
		source		=src,
		features	="c cstlib",
		libs		=['glfw3'],
		libpath		= libpath,
		includes     = incpath,
		target 		=  target_name
	)

	bld.shlib(
		source		=src,
		features	="c cshlib",
		libs		= libs,
		libpath		= libpath,
		includes     = incpath,
		target 		=  target_name
	)

