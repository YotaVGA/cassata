# vim: expandtab:shiftwidth=4:fileencoding=utf-8:filetype=python :

# Copyright ® 2007 Fulvio Satta
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
import re, sys

#Check if the curses is ok (elsewere disable the colors)
use_curses = True
try:
    import curses
except:
    use_curses = False

if use_curses:
    use_curses = sys.stdout.isatty()
if use_curses:
    try:
        curses.setupterm()
        if curses.tigetnum('colors') < 8:
            raise Exception
        if curses.tigetstr('sgr0') == None:
            raise Exception
    except:
        use_curses = False

#Code emitted for a color format
def color_code(i):
    if not use_curses:
        return ''
    info = curses.tigetstr('setf')
    if info == None:
        info = curses.tigetstr('setaf')
    if info == None:
        return ''
    try:
        return curses.tparm(re.sub(r'\$<\d+>[/*]?', '', \
                info), i) or ''
    except:
        return ''

#Code emitted for a normal format
def normal_code():
    if not use_curses:
        return ''
    info = curses.tigetstr('sgr0')
    try:
        return curses.tparm(re.sub(r'\$<\d+>[/*]?', '', \
                info)) or ''
    except:
        return ''

#functions for write a colored text
def textBlack(text):
    return color_code(0) + text + normal_code()

def textBlue(text):
    return color_code(1) + text + normal_code()

def textGreen(text):
    return color_code(2) + text + normal_code()

def textCyan(text):
    return color_code(3) + text + normal_code()

def textRed(text):
    return color_code(4) + text + normal_code()

def textMagenta(text):
    return color_code(5) + text + normal_code()

def textYellow(text):
    return color_code(6) + text + normal_code()

def textWhite(text):
    return color_code(7) + text + normal_code()

def generate(env, **kw):
    #Color functions
    env['BLACK']   = textBlack
    env['BLUE']    = textBlue
    env['GREEN']   = textGreen
    env['CYAN']    = textCyan
    env['RED']     = textRed
    env['MAGENTA'] = textMagenta
    env['YELLOW']  = textYellow
    env['WHITE']   = textWhite
    #default colors
    env['TEXTACTION'] = textGreen
    env['TEXTSOURCE'] = textCyan
    env['TEXTTARGET'] = textMagenta
    env['TEXTSEP']    = textMagenta

def exists(env):
  return 1
