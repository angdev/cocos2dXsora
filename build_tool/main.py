#-*- coding: utf-8 -*-

import os
from jinja2 import Template

SOLUTION_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')

def render_template(dir, tpl_file, data={}):
    real_path = os.path.join(dir, tpl_file)
    linestring = open(real_path, 'r').read()
    linestring = unicode(linestring, 'utf-8')
    
    tpl = Template(linestring)
    content = tpl.render(data)
    return content
    
def openfile(dir, filename):
    real_path = os.path.join(dir, filename)
    try:
        linestring = open(real_path, 'r').read()
        linestring = unicode(linestring, 'utf-8')
        return linestring
    except:
        return u''
    
def get_cpp_list(dir):
    filelist = os.listdir(dir)
    cpp_list = filter(lambda x: True if x.find('cpp') >= 0 else False, filelist)
    cpp_list = filter(lambda x: True if x.find('stdafx') < 0 else False, cpp_list)
    return cpp_list

def build_sora_makefile():
    JNI_PATH = os.path.join(SOLUTION_PATH, 'sora', 'jni')
    CPP_PATH = os.path.join(SOLUTION_PATH, 'sora')
    tpl_file = 'Android.mk.tpl'
    dst_file = 'Android.mk'
    cpp_list = get_cpp_list(CPP_PATH)
    
    data = { 'filelist' : cpp_list }
    next_content = render_template(JNI_PATH, tpl_file, data)
    prev_content = openfile(JNI_PATH, dst_file)
    
    if next_content != prev_content:
        f = open(os.path.join(JNI_PATH, dst_file), 'w')
        f.write(next_content.encode('utf-8'))
        f.close()
        print 'write new sora/Android.mk'
    
    
def build_homura_makefile():
    JNI_PATH = os.path.join(SOLUTION_PATH, 'homura_exe', 'proj.android', 'jni')
    CPP_PATH = os.path.join(SOLUTION_PATH, 'homura')
    tpl_file = 'Android.mk.tpl'
    dst_file = 'Android.mk'
    cpp_list = get_cpp_list(CPP_PATH)
    
    data = { 'filelist' : cpp_list }
    next_content = render_template(JNI_PATH, tpl_file, data)
    prev_content = openfile(JNI_PATH, dst_file)
    
    if next_content != prev_content:
        f = open(os.path.join(JNI_PATH, dst_file), 'w')
        f.write(next_content.encode('utf-8'))
        f.close()
        print 'write new homura/Android.mk'

if __name__ == '__main__':
    build_sora_makefile()
    build_homura_makefile()