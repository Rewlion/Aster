import argparse
from utils import generate_ecs_impl
import os.path

parser = argparse.ArgumentParser()

parser.add_argument('-D' , nargs=1, help='set c++ define', action="append")
parser.add_argument('-I' , nargs=1, help='Include path', action="append")
parser.add_argument('src', help='Source file')
print(os. getcwd())

args = parser.parse_args()

clangIncludes = []
if args.I != None:
  for i in args.I:
    clangIncludes = clangIncludes + ['-I', i[0]]

clangDefines = []
if args.D != None:
  for d in args.D:
    clangDefines = clangDefines + ['-D', d[0]]

if not os.path.isfile(args.src):
  raise ValueError('%s does not exist' % args.src)

generatedSrc = generate_ecs_impl(args.src, clangIncludes, clangDefines)

f = open(args.src.replace('.ecs.cpp', '.gen.cpp'), 'w')
f.write(generatedSrc)

