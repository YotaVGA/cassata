# -*- coding: utf-8 -*-

# Copyright ® 2008-2009 Fulvio Satta
#
# If you want contact me, send an email to Yota_VGA@users.sf.net
#
# This file is part of Cassata
#
# Cassata is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Cassata is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

import os

src = ["main.cpp", "Float.cpp", "Window.cpp", "Render.cpp", "Scene.cpp",
       "SceneRegistrations.cpp", "SceneElement.cpp", "SceneImage.cpp"]

opts = Variables('options.py')
opts.Add(BoolVariable('DEBUG', 'If true the targets are build in debug mode', \
        False))
opts.Add('CXXFLAGS', 'C++ compiler flags', '-O2')
opts.Add('LINKFLAGS', 'Linker flags', '')
opts.Add('BUILDDIR', 'Build directory', 'build')
opts.Add('CPPPATH', 'Header directorys', '/usr/include/eigen2')

env = Environment(options  = opts, \
                  tools    = ['default', 'colors', 'defmessages', 'qt4'], \
                  toolpath = ['tools'],
                  LIBS     = ['m', 'crlibm'],
                  QT4_MOC  = 'moc')

Help("""Use one or more of the subsequent targets:
\tcassata\t\tbuild the cassata executables [this is the default if no targets \
are specified on the command line]

For example:
\tscons cassata

Use the subesequent options for change the default beavoir of the script \
in the format OPTION=VALUE on the command line, or write many lines in the \
format OPTION = 'VALUE' on options.py (create it in the main dir if it don't \
exist).

For example:
\tscons BUILDDIR=build/debug cassata

Options:
""" + opts.GenerateHelpText(env))

env.EnableQt4Modules(['QtCore', 'QtGui', 'QtXml'])

env['CXXFLAGS'] = Split(env['CXXFLAGS'])

if env['DEBUG']:
    env.Append(CXXFLAGS = '-g')

#env.Append(CXXFLAGS = '-ffloat-store') #GCC only
env.Append(CXXFLAGS = '-std=gnu++0x') #GCC only

d = lambda s: os.path.join(env['BUILDDIR'], s)

env.VariantDir(env['BUILDDIR'], 'src')

cassata = env.Program(os.path.join(env['BUILDDIR'], 'cassata'), map(d, src))

Default(cassata)
env.Alias('cassata', cassata)
