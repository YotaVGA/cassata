def generate(env, **kw):
    env['CXXCOMSTR'] = '${TEXTACTION(\'COMPILE:\')} ' \
            '${TEXTSOURCE(\'$SOURCE\')} ${TEXTSEP(\'->\')} ' \
            '${TEXTTARGET(\'$TARGET\')}'
    env['SHCXXCOMSTR'] = '${TEXTACTION(\'SHARED COMPILE:\')} ' \
            '${TEXTSOURCE(\'$SOURCE\')} ${TEXTSEP(\'->\')} ' \
            '${TEXTTARGET(\'$TARGET\')}'
    env['LINKCOMSTR'] = '${TEXTACTION(\'LINK:\')} ' \
            '${TEXTSOURCE(\'$SOURCES\')} ${TEXTSEP(\'->\')} ' \
            '${TEXTTARGET(\'$TARGET\')}'
    env['SHLINKCOMSTR'] = '${TEXTACTION(\'SHARED LINK:\')} ' \
            '${TEXTSOURCE(\'$SOURCES\')} ${TEXTSEP(\'->\')} ' \
            '${TEXTTARGET(\'$TARGET\')}'
    env['QT4_MOCFROMHCOMSTR'] = '${TEXTACTION(\'MOC:\')} ' \
            '${TEXTSOURCE(\'$SOURCE\')} ${TEXTSEP(\'->\')} ' \
            '${TEXTTARGET(\'$TARGET\')}'

def exists(env):
  return 1
