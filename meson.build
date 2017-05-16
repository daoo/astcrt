project('astcrt', 'cpp', default_options: ['cpp_std=c++1z'])

codegen = find_program('pybits/codegen.py')
incdir = include_directories('.')

subdir('astc')

image = static_library(
    'image',
    'image/bgra.cc',
    'image/compressed.cc'
)

executable(
    'astcrt',
    'compress/astc.cc',
    link_with: [astc, image]
)

tests = executable(
    'astcrt-tests',
    'test/main.cc',
    'test/astc/bise.cc',
    'test/astc/bitmanip.cc',
    'test/astc/matrix.cc',
    'test/astc/pca.cc',
    link_with: [astc]
)

test('unit tests', tests)
