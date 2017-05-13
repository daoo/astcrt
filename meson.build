project('astcrt', 'cpp', default_options: ['cpp_std=c++1z'])

astc = static_library(
    'astc',
    'astc/compress_block.cc',
    'astc/compress_texture.cc',
    'astc/endpoints_principal_components.cc',
    'astc/matrix.cc'
)

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
