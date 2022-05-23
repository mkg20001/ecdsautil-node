{
  'targets': [
    {
      'target_name': 'ecdsautil',
      'sources': [
        'binding.cc'
      ],
      'libraries': [
        '<!@(pkg-config ecdsautil --libs)'
      ],
           'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS',
                        'NODE_ADDON_API_ENABLE_MAYBE' ],
      'include_dirs': [
        '<!@(pkg-config ecdsautil --cflags-only-I | sed "s|-I||g")',
        '<!(node -p "require(\'node-addon-api\').include_dir")'
      ]
    }
  ]
}
